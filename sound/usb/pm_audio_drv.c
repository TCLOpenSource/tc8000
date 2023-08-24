#include <linux/init.h>
#include <linux/slab.h>
#include <linux/usb.h>
#include <linux/usb/audio.h>
#include <linux/usb/audio-v2.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
/* puremic begin */
typedef struct tagPmKaraDrv {
	int flag; /*1:usable 0:not usable*/
	int (*sink_init)(struct snd_pcm_substream *substream);
	int (*sink_deinit)(struct snd_pcm_substream *substream);
	int (*sink_copy)(struct snd_pcm_substream *substream, int channel, snd_pcm_uframes_t pos, snd_pcm_uframes_t frames);
	int (*callback)(struct snd_pcm_substream *substream);
	int (*usb_set_cap_status)(struct snd_pcm_substream *substream, bool isrunning);
	int (*usb_audio_cap_init)(struct snd_pcm_substream *substream);
	int (*usb_audio_cap_deinit)(struct snd_pcm_substream *substream);
	int (*retire_capture_usb)(struct snd_pcm_substream *substream,
				unsigned char *cp, unsigned int bytes,
				unsigned int oldptr, unsigned int stride);
} PmKaraDrv;
static PmKaraDrv pmKaraDriver = {
	.flag = 0,
};
int pm_kara_reg_drv(void *drv)
{
	PmKaraDrv *ps = (PmKaraDrv *)drv;
	PmKaraDrv *pd = &pmKaraDriver;
	pd->sink_init			= ps->sink_init;
	pd->sink_deinit			= ps->sink_deinit;
	pd->sink_copy			= ps->sink_copy;
	pd->callback		        = ps->callback;
	pd->usb_set_cap_status		= ps->usb_set_cap_status;
	pd->usb_audio_cap_init		= ps->usb_audio_cap_init;
	pd->usb_audio_cap_deinit	= ps->usb_audio_cap_deinit;
	pd->retire_capture_usb		= ps->retire_capture_usb;
	pd->flag = 1;
	return 0;
}
EXPORT_SYMBOL(pm_kara_reg_drv);
static inline int is_usable(PmKaraDrv *drv)
{
	return drv->flag;
}
static inline PmKaraDrv *get_drv(void)
{
	return &pmKaraDriver;
}
int pm_usb_set_capture_status(struct snd_pcm_substream *substream, bool isrunning)
{
	if (!is_usable(get_drv()))
		return 0;
	return get_drv()->usb_set_cap_status(substream, isrunning);
}
EXPORT_SYMBOL(pm_usb_set_capture_status);

int pm_usb_audio_capture_init(struct snd_pcm_substream *substream)
{
	if (!is_usable(get_drv()))
		return 0;
	return get_drv()->usb_audio_cap_init(substream);
}
EXPORT_SYMBOL(pm_usb_audio_capture_init);

int pm_usb_audio_capture_deinit(struct snd_pcm_substream *substream)
{
	if (!is_usable(get_drv()))
		return 0;
	return get_drv()->usb_audio_cap_deinit(substream);
}
EXPORT_SYMBOL(pm_usb_audio_capture_deinit);

int pm_retire_capture_usb(struct snd_pcm_substream *substream,
	unsigned char *cp, unsigned int bytes,
	unsigned int oldptr, unsigned int stride)
{
	if (!is_usable(get_drv()))
		return -2;
	return get_drv()->retire_capture_usb(substream, cp, bytes, oldptr, stride);
}
EXPORT_SYMBOL(pm_retire_capture_usb);

int pm_sink_init(struct snd_pcm_substream *substream)
{
	if (!is_usable(get_drv()))
		return 0;
	return get_drv()->sink_init(substream);
}
EXPORT_SYMBOL(pm_sink_init);

int pm_sink_deinit(struct snd_pcm_substream *substream)
{
	if (!is_usable(get_drv()))
		return 0;
	return get_drv()->sink_deinit(substream);
}
EXPORT_SYMBOL(pm_sink_deinit);

int pm_sink_copy(struct snd_pcm_substream *substream, int channel, snd_pcm_uframes_t pos, snd_pcm_uframes_t frames)
{
	if (!is_usable(get_drv()))
		return 0;
	return get_drv()->sink_copy(substream, channel, pos, frames);
}
EXPORT_SYMBOL(pm_sink_copy);

int pm_sink_callback(struct snd_pcm_substream *substream)
{
	if (!is_usable(get_drv()))
		return 0;
	return get_drv()->callback(substream);
}
EXPORT_SYMBOL(pm_sink_callback);
