# Makefile for the Linux sound card driver
#
# 18 Apr 1998, Michael Elizabeth Chastain, <mailto:mec@shout.net>
# Rewritten to use lists instead of if-statements.

# Module dependencies for the MXC driver

#
# Define the header file locations for mc13783 PMIC drivers.
#
ifdef CONFIG_MXC_MC13783_LEGACY
CFLAGS_mxc_audio_mc13783.o  = -I$(TOPDIR)/drivers/mxc
endif

#
# Define the header file locations for sc55112 PMIC drivers.
#
ifdef CONFIG_MXC_PMIC_SC55112
CFLAGS_mxc-oss.o          = -I$(TOPDIR)/drivers/mxc/sc55112/module \
			    -I$(TOPDIR)/drivers/mxc/sc55112/core   \
                            -I$(TOPDIR)/drivers/mxc/ssi               \
			    -I$(TOPDIR)/drivers/mxc/dam
CFLAGS_mxc-oss-hw.o       = -I$(TOPDIR)/drivers/mxc/sc55112/module \
                            -I$(TOPDIR)/drivers/mxc/ssi               \
			    -I$(TOPDIR)/drivers/mxc/dam
endif

#
# If we want to build an mxc-sound.ko loadable module, then we must explicitly
# define the CONFIG_SOUND_MXC macro when compiling the audio.c, dmabuf.c, and
# soundcard.c in order to select the correct code sections.
#
# Note that if we are not building a kernel module (i.e., CONFIG_SOUND_MXC=y),
# then the kernel build process will automatically create  the following
# statement:
#
#     #define CONFIG_SOUND_MXC 1
#
# In this case we don't need to explicitly redefine the CONFIG_SOUND_MXC macro.
#
ifeq ($(CONFIG_SOUND_MXC),m)
CFLAGS_audio.o     += -DCONFIG_SOUND_MXC
CFLAGS_dmabuf.o    += -DCONFIG_SOUND_MXC
CFLAGS_soundcard.o += -DCONFIG_SOUND_MXC
endif

#
# Define a macro to enable Virtio-specific code.
#
ifdef CONFIG_VIRTIO_SUPPORT
ifdef CONFIG_MXC_MC13783_LEGACY
CFLAGS_mxc-audio.o       += -DDRV_FOR_VIRTIO_MODEL
CFLAGS_mxc-hwdep.o       += -DDRV_FOR_VIRTIO_MODEL
endif
ifdef CONFIG_MXC_PMIC_SC55112
CFLAGS_mxc-oss.o         += -DDRV_FOR_VIRTIO_MODEL
CFLAGS_mxc-oss-hw.o      += -DDRV_FOR_VIRTIO_MODEL
endif
endif

# Each configuration option enables a list of files.

obj-$(CONFIG_SOUND_OSS)		+= sound.o
obj-$(CONFIG_SOUND_CS4232)	+= cs4232.o ad1848.o 

obj-$(CONFIG_SOUND_OMAP)        += omap-audio-dma-intfc.o omap-audio.o
obj-$(CONFIG_SOUND_OMAP_TSC2101)+= omap-audio-tsc2101.o

# Please leave it as is, cause the link order is significant !

obj-$(CONFIG_SOUND_MXC)		+= mxc-sound.o
obj-$(CONFIG_SOUND_SH_DAC_AUDIO)	+= sh_dac_audio.o
obj-$(CONFIG_SOUND_HAL2)	+= hal2.o
obj-$(CONFIG_SOUND_AEDSP16)	+= aedsp16.o
obj-$(CONFIG_SOUND_PSS)		+= pss.o ad1848.o mpu401.o
obj-$(CONFIG_SOUND_TRIX)	+= trix.o ad1848.o sb_lib.o uart401.o
obj-$(CONFIG_SOUND_OPL3SA1)	+= opl3sa.o ad1848.o uart401.o
obj-$(CONFIG_SOUND_SSCAPE)	+= sscape.o ad1848.o mpu401.o
obj-$(CONFIG_SOUND_MAD16)	+= mad16.o ad1848.o sb_lib.o uart401.o
obj-$(CONFIG_SOUND_CS4232)	+= cs4232.o uart401.o
obj-$(CONFIG_SOUND_MSS)		+= ad1848.o
obj-$(CONFIG_SOUND_OPL3SA2)	+= opl3sa2.o ad1848.o mpu401.o
obj-$(CONFIG_SOUND_PAS)		+= pas2.o sb.o sb_lib.o uart401.o
obj-$(CONFIG_SOUND_SB)		+= sb.o sb_lib.o uart401.o
obj-$(CONFIG_SOUND_KAHLUA)	+= kahlua.o
obj-$(CONFIG_SOUND_WAVEFRONT)	+= wavefront.o
obj-$(CONFIG_SOUND_MAUI)	+= maui.o mpu401.o
obj-$(CONFIG_SOUND_MPU401)	+= mpu401.o
obj-$(CONFIG_SOUND_UART6850)	+= uart6850.o
obj-$(CONFIG_SOUND_GUS)		+= gus.o ad1848.o
obj-$(CONFIG_SOUND_ADLIB)	+= adlib_card.o opl3.o
obj-$(CONFIG_SOUND_YM3812)	+= opl3.o
obj-$(CONFIG_SOUND_VMIDI)	+= v_midi.o
obj-$(CONFIG_SOUND_VIDC)	+= vidc_mod.o
obj-$(CONFIG_SOUND_WAVEARTIST)	+= waveartist.o
obj-$(CONFIG_SOUND_SGALAXY)	+= sgalaxy.o ad1848.o
obj-$(CONFIG_SOUND_AD1816)	+= ad1816.o
obj-$(CONFIG_SOUND_AD1889)	+= ad1889.o ac97_codec.o
obj-$(CONFIG_SOUND_ACI_MIXER)	+= aci.o
obj-$(CONFIG_SOUND_AWE32_SYNTH)	+= awe_wave.o

obj-$(CONFIG_SOUND_VIA82CXXX)	+= via82cxxx_audio.o ac97_codec.o
ifeq ($(CONFIG_MIDI_VIA82CXXX),y)
  obj-$(CONFIG_SOUND_VIA82CXXX) += sound.o uart401.o
endif
obj-$(CONFIG_SOUND_YMFPCI)	+= ymfpci.o ac97_codec.o
ifeq ($(CONFIG_SOUND_YMFPCI_LEGACY),y)
  obj-$(CONFIG_SOUND_YMFPCI)    += opl3.o uart401.o
endif
obj-$(CONFIG_SOUND_MSNDCLAS)	+= msnd.o msnd_classic.o
obj-$(CONFIG_SOUND_MSNDPIN)	+= msnd.o msnd_pinnacle.o
obj-$(CONFIG_SOUND_VWSND)	+= vwsnd.o
obj-$(CONFIG_SOUND_NM256)	+= nm256_audio.o ac97.o
obj-$(CONFIG_SOUND_ICH)		+= i810_audio.o ac97_codec.o
obj-$(CONFIG_SOUND_SONICVIBES)	+= sonicvibes.o
obj-$(CONFIG_SOUND_CMPCI)	+= cmpci.o
ifeq ($(CONFIG_SOUND_CMPCI_FM),y)
  obj-$(CONFIG_SOUND_CMPCI)     += sound.o opl3.o
endif
ifeq ($(CONFIG_SOUND_CMPCI_MIDI),y)
  obj-$(CONFIG_SOUND_CMPCI)     += sound.o mpu401.o
endif
obj-$(CONFIG_SOUND_ES1370)	+= es1370.o
obj-$(CONFIG_SOUND_ES1371)	+= es1371.o ac97_codec.o
obj-$(CONFIG_SOUND_VRC5477)	+= nec_vrc5477.o ac97_codec.o
obj-$(CONFIG_SOUND_AU1000)	+= au1000.o ac97_codec.o  
obj-$(CONFIG_SOUND_AU1550_AC97)	+= au1550_ac97.o ac97_codec.o  
obj-$(CONFIG_SOUND_ESSSOLO1)	+= esssolo1.o
obj-$(CONFIG_SOUND_FUSION)	+= cs46xx.o ac97_codec.o
obj-$(CONFIG_SOUND_MAESTRO)	+= maestro.o
obj-$(CONFIG_SOUND_MAESTRO3)	+= maestro3.o ac97_codec.o
obj-$(CONFIG_SOUND_TRIDENT)	+= trident.o ac97_codec.o
obj-$(CONFIG_SOUND_HARMONY)	+= harmony.o
obj-$(CONFIG_SOUND_EMU10K1)	+= ac97_codec.o
obj-$(CONFIG_SOUND_BCM_CS4297A)	+= swarm_cs4297a.o
obj-$(CONFIG_SOUND_RME96XX)     += rme96xx.o
obj-$(CONFIG_SOUND_BT878)	+= btaudio.o
obj-$(CONFIG_SOUND_ALI5455)	+= ali5455.o ac97_codec.o
obj-$(CONFIG_SOUND_IT8172)	+= ite8172.o ac97_codec.o
obj-$(CONFIG_SOUND_FORTE)	+= forte.o ac97_codec.o

obj-$(CONFIG_SOUND_AD1980)	+= ac97_plugin_ad1980.o
obj-$(CONFIG_SOUND_WM97XX)	+= ac97_plugin_wm97xx.o
obj-$(CONFIG_SOUND_PNX4008)	+= pnx4008-dsp.o

ifeq ($(CONFIG_MIDI_EMU10K1),y)
  obj-$(CONFIG_SOUND_EMU10K1)	+= sound.o
endif

obj-$(CONFIG_SOUND_EMU10K1)	+= emu10k1/
obj-$(CONFIG_SOUND_CS4281)	+= cs4281/
obj-$(CONFIG_DMASOUND)		+= dmasound/

# Declare multi-part drivers.

sound-objs	:= 							\
    dev_table.o soundcard.o sound_syms.o		\
    audio.o audio_syms.o dmabuf.o					\
    midi_syms.o midi_synth.o midibuf.o					\
    sequencer.o sequencer_syms.o sound_timer.o sys_timer.o

gus-objs	:= gus_card.o gus_midi.o gus_vol.o gus_wave.o ics2101.o
pas2-objs	:= pas2_card.o pas2_midi.o pas2_mixer.o pas2_pcm.o
sb-objs		:= sb_card.o
sb_lib-objs	:= sb_common.o sb_audio.o sb_midi.o sb_mixer.o sb_ess.o
vidc_mod-objs	:= vidc.o vidc_fill.o
wavefront-objs  := wavfront.o wf_midi.o yss225.o

ifdef CONFIG_MXC_MC13783_LEGACY
mxc-sound-objs  := mxc_audio_mc13783.o
endif
ifdef CONFIG_MXC_PMIC_SC55112
mxc-sound-objs  := mxc-oss.o mxc-oss-hw.o
endif

hostprogs-y	:= bin2hex hex2hex

# Files generated that shall be removed upon make clean
clean-files := maui_boot.h msndperm.c msndinit.c pndsperm.c pndspini.c \
               pss_boot.h trix_boot.h

# Firmware files that need translation
#
# The translated files are protected by a file that keeps track
# of what name was used to build them.  If the name changes, they
# will be forced to be remade.
#

# Turtle Beach Maui / Tropez

$(obj)/maui.o: $(obj)/maui_boot.h

ifeq ($(CONFIG_MAUI_HAVE_BOOT),y)
    $(obj)/maui_boot.h: $(patsubst "%", %, $(CONFIG_MAUI_BOOT_FILE)) $(obj)/bin2hex
	$(obj)/bin2hex -i maui_os < $< > $@
else
    $(obj)/maui_boot.h:
	(							\
	    echo 'static unsigned char * maui_os = NULL;';	\
	    echo 'static int maui_osLen = 0;';			\
	) > $@
endif

# Turtle Beach MultiSound

ifeq ($(CONFIG_MSNDCLAS_HAVE_BOOT),y)
    $(obj)/msnd_classic.o: $(obj)/msndperm.c $(obj)/msndinit.c

    $(obj)/msndperm.c: $(patsubst "%", %, $(CONFIG_MSNDCLAS_PERM_FILE)) $(obj)/bin2hex
	$(obj)/bin2hex msndperm < $< > $@

    $(obj)/msndinit.c: $(patsubst "%", %, $(CONFIG_MSNDCLAS_INIT_FILE)) $(obj)/bin2hex
	$(obj)/bin2hex msndinit < $< > $@
endif

ifeq ($(CONFIG_MSNDPIN_HAVE_BOOT),y)
    $(obj)/msnd_pinnacle.o: $(obj)/pndsperm.c $(obj)/pndspini.c

    $(obj)/pndsperm.c: $(patsubst "%", %, $(CONFIG_MSNDPIN_PERM_FILE)) $(obj)/bin2hex
	$(obj)/bin2hex pndsperm < $< > $@

    $(obj)/pndspini.c: $(patsubst "%", %, $(CONFIG_MSNDPIN_INIT_FILE)) $(obj)/bin2hex
	$(obj)/bin2hex pndspini < $< > $@
endif

# PSS (ECHO-ADI2111)

$(obj)/pss.o: $(obj)/pss_boot.h

ifeq ($(CONFIG_PSS_HAVE_BOOT),y)
    $(obj)/pss_boot.h: $(patsubst "%", %, $(CONFIG_PSS_BOOT_FILE)) $(obj)/bin2hex
	$(obj)/bin2hex pss_synth < $< > $@
else
    $(obj)/pss_boot.h:
	(							\
	    echo 'static unsigned char * pss_synth = NULL;';	\
	    echo 'static int pss_synthLen = 0;';		\
	) > $@
endif

# MediaTrix AudioTrix Pro

$(obj)/trix.o: $(obj)/trix_boot.h

ifeq ($(CONFIG_TRIX_HAVE_BOOT),y)
    $(obj)/trix_boot.h: $(patsubst "%", %, $(CONFIG_TRIX_BOOT_FILE)) $(obj)/hex2hex
	$(obj)/hex2hex -i trix_boot < $< > $@
else
    $(obj)/trix_boot.h:
	(							\
	    echo 'static unsigned char * trix_boot = NULL;';	\
	    echo 'static int trix_boot_len = 0;';		\
	) > $@
endif
