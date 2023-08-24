#ifndef __GLOBAL_CONTROL__
#define __GLOBAL_CONTROL__

#include <asm/atomic.h>
#include <sound/asound.h>
#include <sound/pcm.h>
#include <sound/core.h>
#include <linux/fs.h>
#include <linux/mm.h>

#define TLKG_DEBUG 1
#if TLKG_DEBUG
#define ___TIME___ ""
#define LOGD(fmt,args...)\
	printk(KERN_ERR"[%s] [tlkg-sound] <<<  fun:%s line: %d >>> " fmt "\n",___TIME___,__FUNCTION__,__LINE__,## args)
#define LOGI(fmt,args...)\
	printk(KERN_ALERT"[%s] [tlkg-sound] <<<  fun:%s line:%d >>> " fmt "\n",___TIME___,__FUNCTION__,__LINE__,## args)
#define LOGE(fmt,args...)\
	printk(KERN_CRIT"[%s] [tlkg-sound] <<<  fun:%s line:%d >>> " fmt "\n",___TIME___,__FUNCTION__,__LINE__,## args)
#else
#define LOGD(fmt,args...)
#define LOGI(fmt,args...)
#define LOGE(fmt,args...)
#endif

//extern GMicOps* captureOps;

#define TLKG_GLOBAL_MACRO 1
#if TLKG_GLOBAL_MACRO

typedef struct usb_event_ops {
	int (*in)(void*);
	int (*out)(void*);
	int (*preopen)(struct snd_pcm *pcm,int stream);
	//int (*open)(struct snd_pcm_substream *substream);
	int (*open)(struct snd_pcm *pcm,int stream,struct snd_pcm_file *file);
	int (*setHWparameters)(struct snd_pcm_substream *substream);
	int (*trigger)(int);
	int (*suspend)(void);
	int (*resume)(void);
	int  (*read)(struct snd_pcm_substream *substream,char*,int);
	int  (*write)(struct snd_pcm_substream *substream,char* hwbuf,char __user *buffer,int size);
	int (*preclose)(struct snd_pcm_substream *substream);
	//int (*close)(struct snd_pcm_substream *substream);
	int (*close)(int card,int device,int stream);
	int  (*status)(struct snd_pcm_substream *substream);
	int  (*period_elapsed)(struct snd_pcm_substream *substream);
}GMicOps;

typedef struct usb_event_box{
	atomic_t ret;
	GMicOps *gmicops;
}GMicOpsBox;

extern GMicOpsBox gmicOpsBox_core;
extern GMicOpsBox gmicOpsBox_usb;

GMicOpsBox* GMIC_ops_usb_get(void);

#define GMIC_ops(ops,_gmicOpsBox,args...)({\
	int __ret = -1;\
	if(atomic_inc_not_zero(&_gmicOpsBox.ret)){\
		if(_gmicOpsBox.gmicops && _gmicOpsBox.gmicops->ops){ \
			__ret = _gmicOpsBox.gmicops->ops(args);\
		}\
		if(atomic_dec_and_test(&_gmicOpsBox.ret)){\
			_gmicOpsBox.gmicops = NULL;\
		};\
	}\
	__ret;\
})
#define GMIC_ops_core(ops,args...)\
	GMIC_ops(ops,gmicOpsBox_core,args)

#define GMIC_ops_usb(ops,args...)({\
	int __ret = -1;\
	GMicOpsBox * box = GMIC_ops_usb_get();\
	if(atomic_inc_not_zero(&box->ret)){\
		if(box->gmicops && box->gmicops->ops){ \
			__ret = box->gmicops->ops(args);\
		}\
		if(atomic_dec_and_test(&box->ret)){\
			box->gmicops = NULL;\
		};\
	}\
	__ret;\
})

#else

#define GMIC_ops_core(ops,args...)({\
	-1;\
})
#define GMIC_ops_usb(ops,args...)({\
	-1;\
})
#endif


#endif
