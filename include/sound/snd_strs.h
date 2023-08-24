#ifndef __SND_STRS_H
#define __SND_STRS_H

struct snd_pcm_str {
	int stream;				/* stream (direction) */
	struct snd_pcm *pcm;
	/* -- substreams -- */
	unsigned int substream_count;
	unsigned int substream_opened;
	struct snd_pcm_substream *substream;
#if defined(CONFIG_SND_PCM_OSS) || defined(CONFIG_SND_PCM_OSS_MODULE)
	/* -- OSS things -- */
	struct snd_pcm_oss_stream oss;
#endif
#ifdef CONFIG_SND_VERBOSE_PROCFS
	struct snd_info_entry *proc_root;
	struct snd_info_entry *proc_info_entry;
#ifdef CONFIG_SND_PCM_XRUN_DEBUG
	unsigned int xrun_debug;	/* 0 = disabled, 1 = verbose, 2 = stacktrace */
	struct snd_info_entry *proc_xrun_debug_entry;
#endif
#endif
	struct snd_kcontrol *chmap_kctl; /* channel-mapping controls */
};

struct snd_pcm {
	struct snd_card *card;
	struct list_head list;
	int device; /* device number */
	unsigned int info_flags;
	unsigned short dev_class;
	unsigned short dev_subclass;
	char id[64];
	char name[80];
	struct snd_pcm_str streams[2];
	struct mutex open_mutex;
	wait_queue_head_t open_wait;
	void *private_data;
	void (*private_free) (struct snd_pcm *pcm);
	struct device *dev; /* actual hw device this belongs to */
	bool internal; /* pcm is for internal use only */
#if defined(CONFIG_SND_PCM_OSS) || defined(CONFIG_SND_PCM_OSS_MODULE)
	struct snd_pcm_oss oss;
#endif
};

#endif
