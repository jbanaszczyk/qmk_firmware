# Enter lower-power sleep mode when on the ChibiOS idle thread
OPT_DEFS += -DCORTEX_ENABLE_WFI_IDLE=TRUE
#OPT_DEFS += -DNO_USB_STARTUP_CHECK

# Include common Keychron files
include keyboards/keychron/common/keychron_common.mk

VPATH += $(TOP_DIR)/keyboards/keychron

# Custom matrix with 74HC595 shift register
SRC += common/matrix.c
