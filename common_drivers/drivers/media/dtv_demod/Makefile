# SPDX-License-Identifier: (GPL-2.0+ OR MIT)

$(MEDIA_MODULE_NAME)-$(CONFIG_AMLOGIC_DTV_DEMOD) += dtv_demod/demod_func.o
$(MEDIA_MODULE_NAME)-$(CONFIG_AMLOGIC_DTV_DEMOD) += dtv_demod/dvbc_func.o
$(MEDIA_MODULE_NAME)-$(CONFIG_AMLOGIC_DTV_DEMOD) += dtv_demod/i2c_func.o
$(MEDIA_MODULE_NAME)-$(CONFIG_AMLOGIC_DTV_DEMOD) += dtv_demod/tuner_func.o
$(MEDIA_MODULE_NAME)-$(CONFIG_AMLOGIC_DTV_DEMOD) += dtv_demod/atsc_func.o
$(MEDIA_MODULE_NAME)-$(CONFIG_AMLOGIC_DTV_DEMOD) += dtv_demod/dvbc_v2.o
$(MEDIA_MODULE_NAME)-$(CONFIG_AMLOGIC_DTV_DEMOD) += dtv_demod/dvbc_v3.o
$(MEDIA_MODULE_NAME)-$(CONFIG_AMLOGIC_DTV_DEMOD) += dtv_demod/dtmb_func.o
$(MEDIA_MODULE_NAME)-$(CONFIG_AMLOGIC_DTV_DEMOD) += dtv_demod/dvbt_v2.o
$(MEDIA_MODULE_NAME)-$(CONFIG_AMLOGIC_DTV_DEMOD) += dtv_demod/amlfrontend.o
$(MEDIA_MODULE_NAME)-$(CONFIG_AMLOGIC_DTV_DEMOD) += dtv_demod/demod_dbg.o
$(MEDIA_MODULE_NAME)-$(CONFIG_AMLOGIC_DTV_DEMOD) += dtv_demod/aml_demod.o
$(MEDIA_MODULE_NAME)-$(CONFIG_AMLOGIC_DTV_DEMOD) += dtv_demod/dvbs.o
$(MEDIA_MODULE_NAME)-$(CONFIG_AMLOGIC_DTV_DEMOD) += dtv_demod/dvbs_diseqc.o
$(MEDIA_MODULE_NAME)-$(CONFIG_AMLOGIC_DTV_DEMOD) += dtv_demod/dvbs_singlecable.o
$(MEDIA_MODULE_NAME)-$(CONFIG_AMLOGIC_DTV_DEMOD) += dtv_demod/lnb_controller/gpio/gpio_lnbc.o
$(MEDIA_MODULE_NAME)-$(CONFIG_AMLOGIC_DTV_DEMOD) += dtv_demod/lnb_controller/wt20_1811/wt20_1811.o
$(MEDIA_MODULE_NAME)-$(CONFIG_AMLOGIC_DTV_DEMOD) += dtv_demod/isdbt_func.o

ccflags-y += -I.
ccflags-y += -I$(srctree)/drivers/media/dvb-core
#ccflags-y += -I$(srctree)/drivers/iio/adc
ccflags-y += -I$(srctree)/include/media
ccflags-y += -I$(srctree)/$(COMMON_DRIVERS_DIR)/drivers/media/dtv_demod/include
ccflags-y += -I$(srctree)/$(COMMON_DRIVERS_DIR)/drivers/media/dtv_demod/lnb_controller
ccflags-y += -I$(srctree)/$(COMMON_DRIVERS_DIR)/drivers/media/dtv_demod/lnb_controller/wt20_1811
ccflags-y += -I$(srctree)/$(COMMON_DRIVERS_DIR)/drivers/media/dtv_demod/lnb_controller/gpio
ccflags-y += -DDVB_CORE_ORI=1
