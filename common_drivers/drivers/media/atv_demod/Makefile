# SPDX-License-Identifier: (GPL-2.0+ OR MIT)

$(MEDIA_MODULE_NAME)-$(CONFIG_AMLOGIC_ATV_DEMOD) += atv_demod/atvdemod_func.o
$(MEDIA_MODULE_NAME)-$(CONFIG_AMLOGIC_ATV_DEMOD) += atv_demod/atvauddemod_func.o
$(MEDIA_MODULE_NAME)-$(CONFIG_AMLOGIC_ATV_DEMOD) += atv_demod/atv_demod_v4l2.o
$(MEDIA_MODULE_NAME)-$(CONFIG_AMLOGIC_ATV_DEMOD) += atv_demod/atv_demod_driver.o
$(MEDIA_MODULE_NAME)-$(CONFIG_AMLOGIC_ATV_DEMOD) += atv_demod/atv_demod_ops.o
$(MEDIA_MODULE_NAME)-$(CONFIG_AMLOGIC_ATV_DEMOD) += atv_demod/atv_demod_afc.o
$(MEDIA_MODULE_NAME)-$(CONFIG_AMLOGIC_ATV_DEMOD) += atv_demod/atv_demod_monitor.o
$(MEDIA_MODULE_NAME)-$(CONFIG_AMLOGIC_ATV_DEMOD) += atv_demod/atv_demod_access.o
$(MEDIA_MODULE_NAME)-$(CONFIG_AMLOGIC_ATV_DEMOD) += atv_demod/atv_demod_debug.o
$(MEDIA_MODULE_NAME)-$(CONFIG_AMLOGIC_ATV_DEMOD) += atv_demod/atv_demod_isr.o
$(MEDIA_MODULE_NAME)-$(CONFIG_AMLOGIC_ATV_DEMOD) += atv_demod/atv_demod_ext.o

ccflags-y += -I.
ccflags-y += -I$(srctree)/drivers/media/dvb-core
ccflags-y += -I$(srctree)/drivers/media/tuners
ccflags-y += -I$(srctree)/include/media
ccflags-y += -I$(srctree)/sound/soc/amlogic/auge
ccflags-y += -I$(srctree)/sound/soc/amlogic/meson
ccflags-y += -I$(srctree)/$(COMMON_DRIVERS_DIR)/drivers/media/vin/tvin/tvafe
