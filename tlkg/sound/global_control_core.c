#include "global_control.h"

//GMicOps* playbackOps = NULL;
//extern GMicOpsBox playbackOpsBox = {

#if TLKG_GLOBAL_MACRO

GMicOpsBox gmicOpsBox_core = {
	.ret = 0,
};

void register_gmic_ops_core(void* arg){
    if(arg){
		if(atomic_read(&gmicOpsBox_core.ret)!=0){
			LOGD("register_capture_ops set error ret is not 0\n");
			return;
		}
    	gmicOpsBox_core.gmicops = (GMicOps*)arg;
		atomic_inc(&gmicOpsBox_core.ret);
    }else{
		if(atomic_dec_and_test(&gmicOpsBox_core.ret)){
			gmicOpsBox_core.gmicops = NULL;
		};
	}
	/*
    if(arg){
    	playbackOps = (GMicOps*)arg;
    }
	*/
}

#else

void register_gmic_ops_core(void* arg){
	LOGD("not used TLKG_GLOBAL_MACRO \n");
}

#endif


EXPORT_SYMBOL(register_gmic_ops_core);






