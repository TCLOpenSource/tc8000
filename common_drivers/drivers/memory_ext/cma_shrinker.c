// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <internal.h>
#include <linux/stddef.h>
#include <linux/compiler.h>
#include <linux/mm.h>
#include <linux/kernel.h>
#include <linux/rmap.h>
#include <linux/kthread.h>
#include <linux/sched/rt.h>
#include <linux/completion.h>
#include <linux/module.h>
#include <linux/swap.h>
#include <linux/migrate.h>
#include <linux/cpu.h>
#include <linux/page-isolation.h>
#include <linux/spinlock_types.h>
#include <linux/amlogic/aml_cma.h>
#include <linux/sched/signal.h>
#include <linux/hugetlb.h>
#include <linux/cma.h>
#include <linux/proc_fs.h>
#include <linux/platform_device.h>
#include <linux/sched/clock.h>
#include <linux/oom.h>
#include <linux/of.h>
#include <linux/shrinker.h>
#include <linux/vmalloc.h>
#include <asm/system_misc.h>
#include <asm/pgtable.h>
#include <linux/page_pinner.h>
#include <trace/events/page_isolation.h>
/* from mm/ path */
#include <internal.h>
#include <linux/amlogic/page_trace.h>

#define PARA_COUNT		6

struct cma_shrinker {
	unsigned int adj[PARA_COUNT];
	unsigned int free[PARA_COUNT];
	unsigned long shrink_timeout;
	unsigned long foreground_timeout;
	unsigned long last_timeout;
	unsigned int last_adj;
	unsigned int enable;
};

struct cma_shrinker *cs;

int filter_adj;
char filter_name[64];
static struct proc_dir_entry *dentry_adj;
static struct proc_dir_entry *dentry_name;

static unsigned long cma_shrinker_count(struct shrinker *s,
					struct shrink_control *sc)
{
	return  global_node_page_state(NR_ACTIVE_ANON) +
		global_node_page_state(NR_ACTIVE_FILE) +
		global_node_page_state(NR_INACTIVE_ANON) +
		global_node_page_state(NR_INACTIVE_FILE);
}

static void show_task_adj(void)
{
#define SHOW_PRIFIX	"score_adj:%5d, rss:%5lu"
	struct task_struct *tsk;
	int tasksize;

	/* avoid print too many */
	if (time_after(cs->foreground_timeout, jiffies))
		return;

	cs->foreground_timeout = jiffies + HZ * 5;
	show_mem(0, NULL);
#ifdef CONFIG_PRINT_MORE_AML
	pr_emerg("Foreground task killed, show all Candidates\n");
#endif
	for_each_process(tsk) {
		struct task_struct *p;
		short oom_score_adj;

		if (tsk->flags & PF_KTHREAD)
			continue;
		p = find_lock_task_mm(tsk);
		if (!p)
			continue;
		oom_score_adj = p->signal->oom_score_adj;
		tasksize = get_mm_rss(p->mm);
#ifdef CONFIG_PRINT_MORE_AML
	#ifdef CONFIG_ZRAM
		pr_emerg(SHOW_PRIFIX ", rswap:%5lu, task:%5d, %s\n",
			 oom_score_adj, get_mm_rss(p->mm),
			 get_mm_counter(p->mm, MM_SWAPENTS),
			 p->pid, p->comm);
	#else
		pr_emerg(SHOW_PRIFIX ", task:%5d, %s\n",
			 oom_score_adj, get_mm_rss(p->mm),
			 p->pid, p->comm);
	#endif /* CONFIG_ZRAM */
#endif
		task_unlock(p);
	}
}

static int swapcache_low(void)
{
	unsigned long free_swap;
	unsigned long total_swap;

	free_swap  = get_nr_swap_pages();
	total_swap = total_swap_pages;
	if ((free_swap <= (total_swap / 16)) && total_swap) {
		/* free swap < 1/16 total */
		pr_debug("swap free is low, free:%ld, total:%ld\n",
			 free_swap, total_swap);
		return 1;
	}
	return 0;
}

static unsigned long cma_shrinker_scan(struct shrinker *s,
				       struct shrink_control *sc)
{
	struct task_struct *tsk;
	struct task_struct *selected = NULL;
	unsigned long rem = 0;
	int tasksize;
	int taskswap = 0;
	int i;
	short min_score_adj = OOM_SCORE_ADJ_MAX + 1;
	int minfree = 0;
	int selected_tasksize = 0;
	short selected_oom_score_adj;
	int other_free = global_zone_page_state(NR_FREE_PAGES);
	int other_file = global_node_page_state(NR_FILE_PAGES) -
			 global_node_page_state(NR_SHMEM) -
			 global_node_page_state(NR_UNEVICTABLE) -
			 total_swapcache_pages();
	int free_cma   = 0;
	int file_cma   = 0;
	int swap_low   = 0;
	int cache_low  = 0;
	int last_idx   = PARA_COUNT - 1;
	int selected_taskswap = 0;

	if (!cs->enable)
		return 0;

	if (time_before_eq(jiffies, cs->shrink_timeout))
		return 0;

	swap_low = swapcache_low();
	if ((!cma_forbidden_mask(sc->gfp_mask) || current_is_kswapd()) &&
	    !swap_low)
		return 0;

	free_cma    = global_zone_page_state(NR_FREE_CMA_PAGES);
	file_cma    = global_zone_page_state(NR_INACTIVE_FILE_CMA) +
		      global_zone_page_state(NR_ACTIVE_FILE_CMA);
	other_free -= free_cma;
	other_file -= file_cma;

	for (i = 0; i < PARA_COUNT; i++) {
		minfree = cs->free[i];
		if (other_free < minfree && other_file < minfree) {
			min_score_adj = cs->adj[i];
			cache_low = 1;
			break;
		}
	}

	if (!cache_low) {
		if (swap_low) {
			/* kill from last prio task */
			min_score_adj = cs->adj[last_idx];
		} else {
			/* nothing to do */
			return 0;
		}
	}
	if (min_score_adj == cs->last_adj && time_before_eq(jiffies, cs->last_timeout))
		return 0;

	pr_debug("%s %lu, %x, ofree %d %d, ma %h\n", __func__,
		 sc->nr_to_scan, sc->gfp_mask, other_free,
		 other_file, min_score_adj);
	cs->last_adj = min_score_adj;

retry:
	selected_oom_score_adj = min_score_adj;
	pr_debug("%s, cachelow:%d, swaplow:%d, adj:%d\n",
		 __func__, cache_low, swap_low, min_score_adj);
	rcu_read_lock();
	for_each_process(tsk) {
		struct task_struct *p;
		short oom_score_adj;

		if (tsk->flags & PF_KTHREAD)
			continue;

		p = find_lock_task_mm(tsk);
		if (!p)
			continue;

		if (task_lmk_waiting(p)) {
			task_unlock(p);
			rcu_read_unlock();
			return 0;
		}
		oom_score_adj = p->signal->oom_score_adj;
		if (oom_score_adj < min_score_adj) {
			task_unlock(p);
			continue;
		}
		tasksize = get_mm_rss(p->mm);
		if (swap_low && !cache_low)
			taskswap = get_mm_counter(p->mm, MM_SWAPENTS);

		task_unlock(p);
		if (tasksize <= 0)
			continue;
		if (swap_low && !cache_low && !taskswap) {
			/* we need free swap but this task don't have swap */
			continue;
		}
		if (selected) {
			if (oom_score_adj < selected_oom_score_adj)
				continue;

			if (swap_low && !cache_low &&
			    taskswap < selected_taskswap)
				continue;

			if (oom_score_adj == selected_oom_score_adj &&
			    tasksize <= selected_tasksize)
				continue;
		}
		if (!strcmp(p->comm, "k.glbenchmark27") ||
			!strcmp(p->comm, "ten.setupwraith") ||
			!strcmp(p->comm, "atom:lmk_victim") ||
			!strcmp(p->comm, "com.tcl.xian.St") ||
			!strcmp(p->comm, "rapps.simpleapp"))
			continue;

		if (oom_score_adj <= 0) {
			pr_debug("ignore top app:'%s' (%d), adj %d, size %d, to kill\n",
				p->comm, p->pid, oom_score_adj, tasksize);
			continue;
		}

		selected = p;
		selected_taskswap = taskswap;
		selected_tasksize = tasksize;
		selected_oom_score_adj = oom_score_adj;
		pr_debug("select '%s' (%d), adj %d, size %d, to kill\n",
			 p->comm, p->pid, oom_score_adj, tasksize);
	}
	/* we try to kill somebody if swap is near full
	 * but too many filecache, this is a coner case
	 */
	if (!selected && !cache_low && swap_low) {
		last_idx--;
		if (last_idx > 0) {	/* don't kill forground */
			min_score_adj = cs->adj[last_idx];
			rcu_read_unlock();
			goto retry;
		} else {
			/* swap full but no one killed */
			show_task_adj();
		}
	}
	if (selected) {
#ifdef CONFIG_PRINT_MORE_AML
		long cache_size = other_file * (long)(PAGE_SIZE / 1024);
		long cache_limit = minfree * (long)(PAGE_SIZE / 1024);
		long free = other_free * (long)(PAGE_SIZE / 1024);
#endif

		if (swap_low && !cache_low) {
			pr_emerg("  Free Swap:%ld kB, Total Swap:%ld kB\n",
				 get_nr_swap_pages() << (PAGE_SHIFT - 10),
				 total_swap_pages << (PAGE_SHIFT - 10));
			pr_emerg("  Task swap:%d, idx:%d\n",
				 selected_taskswap << (PAGE_SHIFT - 10),
				 last_idx);
		}
		pr_debug("filter name: %s, adj: %d\n", filter_name, filter_adj);
		if (!strncmp(filter_name, selected->comm, strlen(selected->comm))) {
			show_mem(0, NULL);
			meminfo_show();
			dump_page_trace();
		} else if (selected_oom_score_adj == filter_adj) {
			show_mem(0, NULL);
			meminfo_show();
			dump_page_trace();
		}

		task_lock(selected);
		send_sig(SIGKILL, selected, 0);
		if (selected->mm)
			task_set_lmk_waiting(selected);
		task_unlock(selected);
		cs->shrink_timeout = jiffies + HZ / 2;
#ifdef CONFIG_PRINT_MORE_AML
		if (swap_low && !cache_low)
			pr_emerg("Killing '%s' (%d) (tgid %d), adj %hd,\n"
				"   to free %ldkB on behalf of '%s' (%d) because\n"
				"   swap low for oom_score_adj %hd\n"
				"   Free memory is %ldkB above reserved\n",
				selected->comm, selected->pid, selected->tgid,
				selected_oom_score_adj,
				selected_tasksize * (long)(PAGE_SIZE / 1024),
				current->comm, current->pid,
				min_score_adj,
				free);
		else
			pr_emerg("Killing '%s' (%d) (tgid %d), adj %hd,\n"
				"   to free %ldkB on behalf of '%s' (%d) because\n"
				"   cache %ldkB is below %ldkB for oom_score_adj %hd\n"
				"   Free memory is %ldkB above reserved\n",
				selected->comm, selected->pid, selected->tgid,
				selected_oom_score_adj,
				selected_tasksize * (long)(PAGE_SIZE / 1024),
				current->comm, current->pid,
				cache_size, cache_limit,
				min_score_adj,
				free);
#endif
		/* kill quickly if can't use cma */
		pr_emerg("Killing '%s' (%d) (tgid %d), adj %hd,\n",
				selected->comm, selected->pid, selected->tgid,
				selected_oom_score_adj);
		rem += selected_tasksize;
		if (!selected_oom_score_adj) /* forgeround task killed */
			show_task_adj();
	} else {
		cs->last_timeout = jiffies + HZ / 50;
	}

	pr_debug("%s %lu, %x, return %lu\n", __func__,
		 sc->nr_to_scan, sc->gfp_mask, rem);
	rcu_read_unlock();
	return rem;
}

static struct shrinker cma_shrinkers = {
	.scan_objects  = cma_shrinker_scan,
	.count_objects = cma_shrinker_count,
	.seeks         = DEFAULT_SEEKS * 16
};

static ssize_t adj_store(struct class *cla,
			 struct class_attribute *attr,
			 const char *buf, size_t count)
{
	int i;
	int adj[6];

	i = sscanf(buf, "%d,%d,%d,%d,%d,%d",
		   &adj[0], &adj[1], &adj[2],
		   &adj[3], &adj[4], &adj[5]);
	if (i != PARA_COUNT) {
		pr_err("invalid input:%s\n", buf);
		return count;
	}

	memcpy(cs->adj, adj, sizeof(adj));
	return count;
}

static ssize_t adj_show(struct class *cla,
			struct class_attribute *attr, char *buf)
{
	int i, sz = 0;
	int other_free = global_zone_page_state(NR_FREE_PAGES) - totalreserve_pages;
	int other_file = global_node_page_state(NR_FILE_PAGES) -
			 global_node_page_state(NR_SHMEM) -
			 global_node_page_state(NR_UNEVICTABLE) -
			 total_swapcache_pages();
	int free_cma   = 0;
	int file_cma   = 0;

	pr_info("------ other free: %d, other: %d, %ld\n",
		other_free, other_file, totalreserve_pages);

	free_cma    = global_zone_page_state(NR_FREE_CMA_PAGES);
	file_cma    = global_zone_page_state(NR_INACTIVE_FILE_CMA) +
		      global_zone_page_state(NR_ACTIVE_FILE_CMA);
	pr_info("------free cma: %d, %d\n", free_cma, file_cma);

	for (i = 0; i < PARA_COUNT; i++)
		sz += sprintf(buf + sz, "%d ", cs->adj[i]);
	sz += sprintf(buf + sz, "\n");
	return sz;
}

static ssize_t free_store(struct class *cla,
			  struct class_attribute *attr,
			  const char *buf, size_t count)
{
	int i;
	int free[6];

	i = sscanf(buf, "%d,%d,%d,%d,%d,%d",
		   &free[0], &free[1], &free[2],
		   &free[3], &free[4], &free[5]);
	if (i != PARA_COUNT) {
		pr_err("invalid input:%s\n", buf);
		return count;
	}

	memcpy(cs->free, free, sizeof(free));
	return count;
}

static ssize_t free_show(struct class *cla,
			 struct class_attribute *attr, char *buf)
{
	int i, sz = 0;

	for (i = 0; i < PARA_COUNT; i++)
		sz += sprintf(buf + sz, "%d ", cs->free[i]);
	sz += sprintf(buf + sz, "\n");
	return sz;
}

static ssize_t enable_store(struct class *cla,
			  struct class_attribute *attr,
			  const char *buf, size_t count)
{
	int ret;
	unsigned int enable;

	ret = kstrtouint(buf, 10, &enable);
	if (ret) {
		pr_err("invalid input:%s\n", buf);
		return count;
	}

	cs->enable = enable;
	return count;
}

static ssize_t enable_show(struct class *cla,
			 struct class_attribute *attr, char *buf)
{
	int sz = 0;

	sz = sprintf(buf, "enable flag: %d\n", cs->enable);
	return sz;
}

static CLASS_ATTR_RW(adj);
static CLASS_ATTR_RW(free);
static CLASS_ATTR_RW(enable);

static struct attribute *cma_shrinker_attrs[] = {
	&class_attr_adj.attr,
	&class_attr_free.attr,
	&class_attr_enable.attr,
	NULL
};

ATTRIBUTE_GROUPS(cma_shrinker);

static struct class cma_shrinker_class = {
		.name = "cma_shrinker",
		.class_groups = cma_shrinker_groups,
};

static int cma_shrinker_probe(struct platform_device *pdev)
{
	struct cma_shrinker *p;
	struct device_node *np;
	int ret, i;

	p = kzalloc(sizeof(*p), GFP_KERNEL);
	if (!p)
		return -ENOMEM;

	cs = p;
	cs->enable = 1;
	np = pdev->dev.of_node;
	ret = of_property_read_u32_array(np, "adj", cs->adj, PARA_COUNT);
	if (ret < 0)
		goto err;

	ret = of_property_read_u32_array(np, "free", cs->free, PARA_COUNT);
	if (ret < 0)
		goto err;

	ret = class_register(&cma_shrinker_class);
	if (ret)
		goto err;

	if (register_shrinker(&cma_shrinkers))
		goto err;

	for (i = 0; i < PARA_COUNT; i++) {
		pr_debug("cma shrinker, adj:%3d, free:%d\n",
			cs->adj[i], cs->free[i]);
	}
	return 0;

err:
	kfree(p);
	cs = NULL;
	return -EINVAL;
}

static int cma_shrinker_remove(struct platform_device *pdev)
{
	if (cs) {
		class_unregister(&cma_shrinker_class);
		kfree(cs);
		cs = NULL;
	}
	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id cma_shrinker_dt_match[] = {
	{
		.compatible = "amlogic, cma-shrinker",
	},
	{}
};
#endif

static struct platform_driver cma_shrinker_driver = {
	.driver = {
		.name  = "cma_shrinker",
		.owner = THIS_MODULE,
	#ifdef CONFIG_OF
		.of_match_table = cma_shrinker_dt_match,
	#endif
	},
	.probe = cma_shrinker_probe,
	.remove = cma_shrinker_remove,
};

static int filter_adj_show(struct seq_file *m, void *arg)
{
	seq_printf(m, "filter_adj=%d\n", filter_adj);

	return 0;
}

static ssize_t filter_adj_write(struct file *file, const char __user *buffer,
			       size_t count, loff_t *ppos)
{
	int arg = 0;
	int ok = 0;
	char *buf;

	buf = kmalloc(count, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	if (copy_from_user(buf, buffer, count))
		goto exit;

	if (kstrtoint(buf, 10, &arg))
		goto exit;

	ok = 1;
	filter_adj = arg;
	pr_info("filter_adj: %d\n", filter_adj);
exit:
	kfree(buf);
	if (ok)
		return count;
	else
		return -EINVAL;
}

static int filter_adj_open(struct inode *inode, struct file *file)
{
	return single_open(file, filter_adj_show, NULL);
}

static const struct proc_ops filter_adj_file_ops = {
	.proc_open	= filter_adj_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= filter_adj_write,
	.proc_release	= single_release,
};

static int filter_name_show(struct seq_file *m, void *arg)
{
	seq_printf(m, "filter_name=%s\n", filter_name);

	return 0;
}

static ssize_t filter_name_write(struct file *file, const char __user *buffer,
			       size_t count, loff_t *ppos)
{
	int ok = 0;

	memset(filter_name, 0, 64);
	if (copy_from_user(filter_name, buffer, count))
		goto exit;

	if (!strcmp(filter_name, "clear"))
		memset(filter_name, 0, 64);

	ok = 1;
	pr_info("filter_name: %s\n", filter_name);
exit:
	if (ok)
		return count;
	else
		return -EINVAL;
}

static int filter_name_open(struct inode *inode, struct file *file)
{
	return single_open(file, filter_name_show, NULL);
}

static const struct proc_ops filter_name_file_ops = {
	.proc_open	= filter_name_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= filter_name_write,
	.proc_release	= single_release,
};

static int __init cma_shrinker_init(void)
{
	dentry_adj = proc_create("filter_adj", 0644, NULL, &filter_adj_file_ops);
	if (IS_ERR_OR_NULL(dentry_adj)) {
		pr_err("%s, create sysfs failed\n", __func__);
		return -1;
	}

	dentry_name = proc_create("filter_name", 0644, NULL, &filter_name_file_ops);
	if (IS_ERR_OR_NULL(dentry_name)) {
		pr_err("%s, create sysfs failed\n", __func__);
		return -1;
	}

	return platform_driver_register(&cma_shrinker_driver);
}

static void __exit cma_shrinker_uninit(void)
{
	platform_driver_unregister(&cma_shrinker_driver);
}

device_initcall(cma_shrinker_init);
module_exit(cma_shrinker_uninit);
