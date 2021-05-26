TARGET = xmbctrlinst
OBJS = crt0.o main.o 

INCDIR = 
CFLAGS = -O2 -G0 -Wall -std=c99 -fshort-wchar -fno-pic -mno-check-zero-division
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS) -c

ifeq ($(TEST), 1)
CFLAGS += -DTEST=1
endif

LIBDIR = lib/
LDFLAGS = -mno-crt0 -nostdlib -nodefaultlibs
LIBS = -lvlfgui -lvlfgu -lvlfutils -lvlflibc -lpspkubridge

PSP_FW_VERSION = 271

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = XMB Control ver.1.5.5
PSP_EBOOT_ICON = ICON0.PNG
PSP_EBOOT_PIC1 = PIC1.PNG

BUILD_PRX = 1
PSP_LARGE_MEMORY = 1

PSPSDK=$(shell psp-config --pspsdk-path)
include ./build.mak

