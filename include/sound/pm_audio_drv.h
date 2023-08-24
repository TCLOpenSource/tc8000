#ifndef __PM_AUDIO_DRV_H__
#define __PM_AUDIO_DRV_H__
#ifndef CONFIG_SND_USB_CAPTURE_DATA
#define CONFIG_SND_USB_CAPTURE_DATA
#endif
extern int pm_sink_init(struct snd_pcm_substream *substream);
extern int pm_sink_deinit(struct snd_pcm_substream *substream);
extern int pm_sink_copy(struct snd_pcm_substream *substream, int channel, snd_pcm_uframes_t pos, snd_pcm_uframes_t frames);
extern int pm_usb_set_capture_status(struct snd_pcm_substream *substream, bool isrunning);
extern int pm_usb_audio_capture_init(struct snd_pcm_substream *substream);
extern int pm_usb_audio_capture_deinit(struct snd_pcm_substream *substream);
extern int pm_retire_capture_usb(struct snd_pcm_substream *substream,
		unsigned char *cp, unsigned int bytes,
		unsigned int oldptr, unsigned int stride);
#endif
