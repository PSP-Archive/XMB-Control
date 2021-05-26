TARGET = xmbctrl
OBJS = main.o utils.o stub.o exports.o conf.o nidresolver.o

CFLAGS = -O2 -Os -G0 -Wall -std=c99 -fshort-wchar -fno-pic -mno-check-zero-division
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

BUILD_PRX = 1
PRX_EXPORTS = exports.exp

PSP_FW_VERSION = 271

ifeq ($(ITEM), 1)
CFLAGS += -DITEM=1
endif

INCDIR = include/

USE_PSPSDK_LIBS = 1
USE_PSPSDK_LIBC = 1

LIBS = -lpspsystemctrl_user -lpspkubridge -lpspvshctrl -lpspsystemctrl_kernel -lpspsystemctrl_user -lpspkubridge -lpsppower

PSPSDK = $(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build_prx.mak