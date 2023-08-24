#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/track_marcro.h>
#include <sound/track_drv.h>
#include <sound/pm_strs_stat.h>
#ifdef CONFIG_SND_PCM_TRACK
/* track driver */
static struct track_driver track_driver = {
	.flag = 0,
};
static inline struct track_driver *get_track_drv(void)
{
        return &track_driver;
}
static inline int is_track_drv_usable(struct track_driver *drv)
{
        return drv->flag;
}
int snd_pcm_set_status(
				struct file *file,
				struct snd_pcm *pcm,
				int stream,
				int status){
	if (!is_track_drv_usable(get_track_drv()))
		return 0;
	return get_track_drv()->snd_pcm_set_status(file, pcm, stream, status);
}
int register_track_driver(void *drv){

	struct track_driver *from
				= (struct track_driver *)drv;
	struct track_driver *to
				= &track_driver;

	to->snd_pcm_set_status = from->snd_pcm_set_status;
	to->flag = 1;
	return 0;
}
EXPORT_SYMBOL(register_track_driver);
int unregister_track_driver(void *drv){
	struct track_driver *to
				= &track_driver;

	to->flag = 0;
	to->snd_pcm_set_status = NULL;
	return 0;
}
EXPORT_SYMBOL(unregister_track_driver);
/* end track driver */
#endif
