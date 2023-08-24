#include "global_control.h"

//GMicOps* playbackOps = NULL;
//extern GMicOpsBox playbackOpsBox = {

#if TLKG_GLOBAL_MACRO

GMicOpsBox gmicOpsBox_usb = {
	.ret = 0,
};

void register_gmic_ops_usb(void* arg){
    if(arg){
		if(atomic_read(&gmicOpsBox_usb.ret)!=0){
			LOGD("register_capture_ops set error ret is not 0\n");
			return;
		}
    	gmicOpsBox_usb.gmicops = (GMicOps*)arg;
		atomic_inc(&gmicOpsBox_usb.ret);
    }else{
		if(atomic_dec_and_test(&gmicOpsBox_usb.ret)){
			gmicOpsBox_usb.gmicops = NULL;
		};
	}
	/*
    if(arg){
    	playbackOps = (GMicOps*)arg;
    }
	*/
}

#else

void register_gmic_ops_usb(void* arg){
	LOGD("not used TLKG_GLOBAL_MACRO \n");
}

#endif


EXPORT_SYMBOL(register_gmic_ops_usb);



GMicOpsBox* GMIC_ops_usb_get(void){
	return &gmicOpsBox_usb;
}
EXPORT_SYMBOL(GMIC_ops_usb_get);






