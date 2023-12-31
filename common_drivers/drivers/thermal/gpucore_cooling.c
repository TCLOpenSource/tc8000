// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <linux/module.h>
#include <linux/thermal.h>
#include <linux/cpufreq.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/cpu.h>
#include <linux/amlogic/gpucore_cooling.h>
#include <linux/device.h>

/**
 * struct gpucore_cooling_device - data for cooling device with gpucore
 * @id: unique integer value corresponding to each gpucore_cooling_device
 *	registered.
 * @cool_dev: thermal_cooling_device pointer to keep track of the
 *	registered cooling device.
 * @gpucore_state: integer value representing the current state of gpucore
 *	cooling	devices.
 * @gpucore_val: integer value representing the absolute value of the clipped
 *	frequency.
 * @allowed_cpus: all the cpus involved for this gpucore_cooling_device.
 *
 * This structure is required for keeping information of each
 * gpucore_cooling_device registered. In order to prevent corruption of this a
 * mutex lock cooling_gpucore_lock is used.
 */

static int gpucorecd_id;
static DEFINE_MUTEX(cooling_gpucore_lock);

/* notify_table passes value to the gpucore_ADJUST callback function. */
#define NOTIFY_INVALID NULL

static struct device_node *np;
static int save_flag = -1;

void save_gpucore_thermal_para(struct device_node *n)
{
	if (!n)
		return;

	if (save_flag == -1) {
		save_flag = 1;
		np = n;
	}
}

static void gpucore_coolingdevice_id_get(int *id)
{
	mutex_lock(&cooling_gpucore_lock);
	*id = gpucorecd_id++;
	mutex_unlock(&cooling_gpucore_lock);
}

static void gpucore_coolingdevice_id_put(void)
{
	mutex_lock(&cooling_gpucore_lock);
	gpucorecd_id--;
	mutex_unlock(&cooling_gpucore_lock);
}

/* gpucore cooling device callback functions are defined below */

/**
 * gpucore_get_max_state - callback function to get the max cooling state.
 * @cdev: thermal cooling device pointer.
 * @state: fill this variable with the max cooling state.
 *
 * Callback for the thermal cooling device to return the gpucore
 * max cooling state.
 *
 * Return: 0 on success, an error code otherwise.
 */
static int gpucore_get_max_state(struct thermal_cooling_device *cdev,
				 unsigned long *state)
{
	struct gpucore_cooling_device *gpucore_device = cdev->devdata;
	*state = gpucore_device->max_gpu_core_num;
	pr_debug("max Gpu core=%ld\n", *state);
	return 0;
}

/**
 * gpucore_get_cur_state - callback function to get the current cooling state.
 * @cdev: thermal cooling device pointer.
 * @state: fill this variable with the current cooling state.
 *
 * Callback for the thermal cooling device to return the gpucore
 * current cooling state.
 *
 * Return: 0 on success, an error code otherwise.
 */
static int gpucore_get_cur_state(struct thermal_cooling_device *cdev,
				 unsigned long *state)
{
	struct gpucore_cooling_device *gpucore_device = cdev->devdata;

	*state = gpucore_device->gpucore_state;
	pr_debug("current state=%ld\n", *state);
	return 0;
}

/**
 * gpucore_set_cur_state - callback function to set the current cooling state.
 * @cdev: thermal cooling device pointer.
 * @state: set this variable to the current cooling state.
 *
 * Callback for the thermal cooling device to change the gpucore
 * current cooling state.
 *
 * Return: 0 on success, an error code otherwise.
 */
static int gpucore_set_cur_state(struct thermal_cooling_device *cdev,
				 unsigned long state)
{
	struct gpucore_cooling_device *gpucore_device = cdev->devdata;
	int set_max_num;

	if (WARN_ON(state >= gpucore_device->max_gpu_core_num))
		return -EINVAL;

	mutex_lock(&cooling_gpucore_lock);
	if (gpucore_device->stop_flag) {
		mutex_unlock(&cooling_gpucore_lock);
		return 0;
	}
	if ((state & GPU_STOP) == GPU_STOP) {
		gpucore_device->stop_flag = 1;
		state = state & (~GPU_STOP);
	}
	mutex_unlock(&cooling_gpucore_lock);
	set_max_num = gpucore_device->max_gpu_core_num - state;
	/* pp should not be 0 */
	if (!set_max_num)
		return -EINVAL;

	gpucore_device->gpucore_state = state;
	gpucore_device->set_max_pp_num((unsigned int)set_max_num);
	pr_debug("need set max gpu num=%d,state=%ld\n", set_max_num, state);
	return 0;
}

/*
 * Simple mathematics model for gpu core power:
 * just for ipa hook
 */
static int gpucore_get_requested_power(struct thermal_cooling_device *cdev,
				       u32 *power)
{
	*power = 0;

	return 0;
}

static int gpucore_state2power(struct thermal_cooling_device *cdev,
			       unsigned long state, u32 *power)
{
	*power  = 0;

	return 0;
}

static int gpucore_power2state(struct thermal_cooling_device *cdev,
			       u32 power, unsigned long *state)
{
	cdev->ops->get_cur_state(cdev, state);
	return 0;
}

/* Bind gpucore callbacks to thermal cooling device ops */
static struct thermal_cooling_device_ops const gpucore_cooling_ops = {
	.get_max_state = gpucore_get_max_state,
	.get_cur_state = gpucore_get_cur_state,
	.set_cur_state = gpucore_set_cur_state,
	.state2power   = gpucore_state2power,
	.power2state   = gpucore_power2state,
	.get_requested_power = gpucore_get_requested_power,
};

/**
 * gpucore_cooling_register - function to create gpucore cooling device.
 * @clip_cpus: cpumask of cpus where the frequency constraints will happen.
 *
 * This interface function registers the gpucore cooling device with the name
 * "thermal-gpucore-%x". This api can support multiple instances of gpucore
 * cooling devices.
 *
 * Return: a valid struct thermal_cooling_device pointer on success,
 * on failure, it returns a corresponding ERR_PTR().
 */
struct gpucore_cooling_device *gpucore_cooling_alloc(void)
{
	struct gpucore_cooling_device *gcdev;

	gcdev = kzalloc(sizeof(*gcdev), GFP_KERNEL);
	if (!gcdev)
		return ERR_PTR(-ENOMEM);
	memset(gcdev, 0, sizeof(*gcdev));
	if (save_flag == 1)
		gcdev->np = np;
	return gcdev;
}
EXPORT_SYMBOL_GPL(gpucore_cooling_alloc);

int gpucore_cooling_register(struct gpucore_cooling_device *gpucore_dev)
{
	struct thermal_cooling_device *cool_dev;
	char dev_name[THERMAL_NAME_LENGTH];

	gpucore_coolingdevice_id_get(&gpucore_dev->id);
	snprintf(dev_name, sizeof(dev_name), "thermal-gpucore-%d",
		 gpucore_dev->id);

	gpucore_dev->gpucore_state = 0;
	cool_dev = thermal_of_cooling_device_register(gpucore_dev->np,
						      dev_name,
						      gpucore_dev,
						      &gpucore_cooling_ops);
	if (!cool_dev) {
		gpucore_coolingdevice_id_put();
		kfree(gpucore_dev);
		return -EINVAL;
	}
	gpucore_dev->cool_dev = cool_dev;
	return 0;
}
EXPORT_SYMBOL_GPL(gpucore_cooling_register);

/**
 * gpucore_cooling_unregister - function to remove gpucore cooling device.
 * @cdev: thermal cooling device pointer.
 *
 * This interface function unregisters the "thermal-gpucore-%x" cooling device.
 */
void gpucore_cooling_unregister(struct thermal_cooling_device *cdev)
{
	struct gpucore_cooling_device *gpucore_dev;

	if (!cdev)
		return;

	gpucore_dev = cdev->devdata;

	thermal_cooling_device_unregister(gpucore_dev->cool_dev);
	gpucore_coolingdevice_id_put();
	kfree(gpucore_dev);
}
EXPORT_SYMBOL_GPL(gpucore_cooling_unregister);
