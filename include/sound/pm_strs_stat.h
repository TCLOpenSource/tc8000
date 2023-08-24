#ifndef __PM_STRS_STAT_H
#define __PM_STRS_STAT_H

enum {
	SNDRV_PCM_STREAMS = 2,
};

enum {
	SNDRV_PCM_STREAM_BLOCK = 0,
	SNDRV_PCM_STREAM_OPEN,
	SNDRV_PCM_STREAM_RELEASE,
};

struct snd_file {
	struct list_head list;
	struct file *file;
	int stream;
	int status;
};

struct file_list{
	struct list_head file_list_head[SNDRV_PCM_STREAMS]; /* PLAYBACK and CAPTURE */
	struct mutex list_lock;
};

#endif
