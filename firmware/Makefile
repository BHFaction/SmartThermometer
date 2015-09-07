#Put your personal build config in config.mk and DO NOT COMMIT IT!
-include config.mk

######### JTAG and environment configuration ##########

#ST-Link configuration
STFLASH 			?=st-flash
CROSS_COMPILE 		?= arm-none-eabi-
PYTHON2 			?= python

CLOAD 				?= 0
DEBUG 				?= 0
PLATFORM			?= TF1

ifeq ($(PLATFORM), TF1)
STFLASH_ADDRESS		?= 0x8000000
USE_FPU 			?= 0
endif

## Flag that can be added to config.mk
ifeq ($(PLATFORM), TF1)
REV               ?= F
endif

############### Location configuration ################
# FREERTOS = lib/FreeRTOS
# ifeq ($(USE_FPU), 1)
# PORT = $(FREERTOS)/portable/GCC/ARM_CM4F
# else
# PORT = $(FREERTOS)/portable/GCC/ARM_CM3
# endif

ifeq ($(PLATFORM), TF1)
LINKER_DIR = scripts/F103/linker
ST_OBJ_DIR  = scripts/F103
endif

STLIB = lib

################ Build configuration ##################
# St Lib
VPATH_TF1 += $(STLIB)/CMSIS/Core/CM3
VPATH_TF1 += $(STLIB)/CMSIS/Core/CM3/startup/gcc
VPATH_TF1 += $(STLIB)/STM32_CPAL_Driver/src
VPATH_TF1 += $(STLIB)/STM32_CPAL_Driver/devices/stm32f10x
CRT0_TF1 = startup_stm32f10x_md.o system_stm32f10x.o

# Should maybe be in separate file?
-include $(ST_OBJ_DIR)/st_obj.mk

ST_OBJ_TF1 =  #cpal_hal.o cpal_i2c.o cpal_usercallback_template.o cpal_i2c_hal_stm32f10x.o

VPATH += init hal/src modules/src utils/src drivers/src
VPATH_TF1 += platform/tf1

ifeq ($(PLATFORM), TF1)
VPATH +=$(VPATH_TF1)
endif


############### Source files configuration ################

# Init
PROJ_OBJ += main.o
PROJ_OBJ_TF1 += platform_tf1.o

# Drivers
PROJ_OBJ += exti.o nvic.o spi.o max6675.o

# Hal
PROJ_OBJ += uart.o temp.o

# Modules
PROJ_OBJ += system.o

# Utilities
# PROJ_OBJ += debug.o

OBJ =  $(PORT_OBJ) $(ST_OBJ) $(PROJ_OBJ)
ifeq ($(PLATFORM), TF1)
OBJ += $(CRT0_TF1) $(ST_OBJ_TF1) $(PROJ_OBJ_TF1)
endif

ifdef P
  C_PROFILE = -D P_$(P)
endif

############### Compilation configuration ################
AS = $(CROSS_COMPILE)as
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)gcc
SIZE = $(CROSS_COMPILE)size
OBJCOPY = $(CROSS_COMPILE)objcopy

# INCLUDES  = -I$(FREERTOS)/include -I$(PORT) -I.
INCLUDES += -Iconfig -Ihal/interface -Imodules/interface
INCLUDES += -Iutils/interface -Idrivers/interface -Iplatform
INCLUDES += -I$(STLIB)/CMSIS/Include

INCLUDES_TF1 += -I$(STLIB)/STM32F10x_StdPeriph_Driver/inc
INCLUDES_TF1 += -I$(STLIB)/CMSIS/Core/CM3
INCLUDES_TF1 += -I$(STLIB)/STM32_CPAL_Driver/inc
INCLUDES_TF1 += -I$(STLIB)/STM32_CPAL_Driver/devices/stm32f10x

ifeq ($(USE_FPU), 1)
	PROCESSOR = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
	CFLAGS += -fno-math-errno
else
	ifeq ($(PLATFORM), TF1)
		PROCESSOR = -mcpu=cortex-m3 -mthumb -mfloat-abi=soft
	endif
endif

#Flags required by the ST library
STFLAGS_TF1 = -DSTM32F10X_MD -DHSE_VALUE=16000000 -include stm32f10x_conf.h -DPLATFORM_TF1

ifeq ($(DEBUG), 1)
  CFLAGS += -O0 -g3 -DDEBUG
else
  CFLAGS += -Os -g3
endif

ifeq ($(LTO), 1)
  CFLAGS += -flto
endif

ifeq ($(USE_ESKYLINK), 1)
  CFLAGS += -DUSE_ESKYLINK
endif

CFLAGS += -DBOARD_REV_$(REV)

CFLAGS += $(PROCESSOR) $(INCLUDES) $(STFLAGS)
ifeq ($(PLATFORM), TF1)
CFLAGS += $(INCLUDES_TF1) $(STFLAGS_TF1)
endif


CFLAGS += -Wall -fno-strict-aliasing $(C_PROFILE)
# Compiler flags to generate dependency files:
CFLAGS += -MD -MP -MF $(BIN)/dep/$(@).d -MQ $(@)
#Permits to remove un-used functions and global variables from output file
CFLAGS += -ffunction-sections -fdata-sections

ASFLAGS = $(PROCESSOR) $(INCLUDES)
LDFLAGS = --specs=nano.specs $(PROCESSOR) -Wl,-Map=$(PROG).map,--cref,--gc-sections

#Flags required by the ST library
ifeq ($(CLOAD), 1)
  LDFLAGS += -T $(LINKER_DIR)/FLASH_CLOAD.ld
  LOAD_ADDRESS = 0x8004000
else
  LDFLAGS += -T $(LINKER_DIR)/FLASH.ld
  LOAD_ADDRESS = 0x8000000
endif

ifeq ($(LTO), 1)
  LDFLAGS += -Os -flto -fuse-linker-plugin
endif

#Program name
ifeq ($(PLATFORM), TF1)
PROG = tf1
endif
#Where to compile the .o
BIN = bin
VPATH += $(BIN)

#Dependency files to include
DEPS := $(foreach o,$(OBJ),$(BIN)/dep/$(o).d)

##################### Misc. ################################
ifeq ($(SHELL),/bin/sh)
  COL_RED=\033[1;31m
  COL_GREEN=\033[1;32m
  COL_RESET=\033[m
endif

#################### Targets ###############################


all: build
build: clean_version compile print_version size
compile: clean_version $(PROG).hex $(PROG).bin $(PROG).dfu

clean_version:
ifeq ($(SHELL),/bin/sh)
	@echo "  CLEAN_VERSION"
	@rm -f version.c
endif

print_version: compile
ifeq ($(SHELL),/bin/sh)
	@./scripts/print_revision.sh
endif
ifeq ($(PLATFORM), TF1)
	@echo "# SmartThermometer build!"
endif
ifeq ($(CLOAD), 1)
	@echo "# loader build!"
endif


size: compile
	@$(SIZE) -B $(PROG).elf

#Flash the stm.
flash:
	$(STFLASH) write $(PROG).bin $(STFLASH_ADDRESS)
	
#$(OPENOCD) -d2 -f $(OPENOCD_INTERFACE) -f $(OPENOCD_TARGET) -c init -c targets -c "reset halt" \
#                -c "flash write_image erase $(PROG).elf" -c "verify_image $(PROG).elf" -c "reset run" -c shutdown

erase:
	$(STFLASH) erase

#Print preprocessor #defines
prep:
	@$(CC) -dD

include scripts/targets.mk

#include dependencies
-include $(DEPS)
