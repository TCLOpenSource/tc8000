#ifndef __STRS_INTF_H
#define __STRS_INTF_H
extern int snd_pcm_set_status(
				struct file *file,
				struct snd_pcm *pcm,
				int stream,
				int status);
#endif
