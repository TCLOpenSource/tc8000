#ifndef __TRACK_DRV_H
#define __TRACK_DRV_H

struct track_driver{
	int flag;
	int (*snd_pcm_set_status)(
				struct file *file,
				struct snd_pcm *pcm,
				int stream,
				int status);
};

#endif
