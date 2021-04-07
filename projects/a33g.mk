VENDOR := abov
XTAL ?= 8000000
IRQ_DEFINES ?= ../devices/$(VENDOR)/$(DEVICE)/irq.def

include $(LIBABOV_ROOT)/arch/arm/cortex-m/m3/m3.mk
include $(LIBABOV_ROOT)/devices/$(VENDOR)/$(DEVICE)/device.mk
