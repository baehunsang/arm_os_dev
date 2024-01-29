ARCH = armv7-a
MCPU = cortex-a8

TARGET = rvpb

CC = arm-linux-gnueabihf-gcc
AS = arm-linux-gnueabihf-as
LD = arm-linux-gnueabihf-ld
OC = arm-linux-gnueabihf-objcopy

LINKER_SCRIPT = ./navilos.ld
MAP_FILE = build/navilos.map

ASM_SRCS = $(wildcard boot/*.S)
ASM_OBJS = $(patsubst boot/%.S, build/%.os, $(ASM_SRCS))

VPATH = boot \
		hal/$(TARGET) \
		lib

C_SRCS = $(notdir $(wildcard boot/*.c))
C_SRCS += $(notdir $(wildcard hal/$(TARGET)/*.c))
C_SRCS += $(notdir $(wildcard lib/*.c))
C_OBJS = $(patsubst %.c, build/%.o, $(C_SRCS))

INC_DIRS = -Iinclude \
			-Ihal \
			-Ihal/$(TARGET) \
			-Ilib

CFLAGS = -c -g -std=c11 -mfloat-abi=soft -fno-stack-protector

navilos = build/navilos.axf
navilos_bin = build/navilos.bin

.PHONY: all clean run debug gdb

all: $(navilos)

clean:
	@rm -rf build

run: $(navilos)
	qemu-system-arm -M realview-pb-a8 -kernel $(navilos) -nographic

debug: $(navilos)
	qemu-system-arm -M realview-pb-a8 -kernel $(navilos) -gdb tcp::1234,ipv4 -nographic -S 

gdb: 
	gdb-multiarch build/navilos.axf

$(navilos): $(ASM_OBJS) $(C_OBJS) $(LINKER_SCRIPT)
	$(LD) -n -T $(LINKER_SCRIPT) -o $(navilos) $(ASM_OBJS) $(C_OBJS) -Map=$(MAP_FILE)
	$(OC) -O binary $(navilos) $(navilos_bin)

build/%.o: %.c
	mkdir -p $(shell dirname $@)
	$(CC) -march=$(ARCH) -mcpu=$(MCPU) $(INC_DIRS) $(CFLAGS) -o $@ $<

build/%.os: %.S
	mkdir -p $(shell dirname $@)
	$(CC) -march=$(ARCH) -mcpu=$(MCPU) $(INC_DIRS) $(CFLAGS) -o $@ $<