ARCH = armv7-a
MCPU = cortex-a8

CC = arm-linux-gnueabihf-gcc
AS = arm-linux-gnueabihf-as
LD = arm-linux-gnueabihf-ld
OC = arm-linux-gnueabihf-objcopy

LINKER_SCRIPT = ./navilos.ld

ASM_SRCS = $(wildcard boot/*.S)
ASM_OBJS = $(patsubst boot/%.S, build/%.o, $(ASM_SRCS))

navilos = build/navilos.axf
navilos_bin = build/navilos.bin

.PHONY: all clean run debug gdb

all: $(navilos)

clean:
	@rm -rf build

run: $(navilos)
	qemu-system-arm -M realview-pb-a8 -kernel navilos.axf -nographic

debug: $(navilos)
	qemu-system-arm -M realview-pb-a8 -kernel build/navilos.axf -gdb tcp::1234,ipv4 -nographic -S 

gdb: 
	gdb-multiarch build/navilos.axf

$(navilos): $(ASM_OBJS) $(LINKER_SCRIPT)
	$(LD) -n -T $(LINKER_SCRIPT) -o $(navilos) $(ASM_OBJS)
	$(OC) -O binary $(navilos) $(navilos_bin)

build/%.o: boot/%.S
	mkdir -p $(shell dirname $@)
	$(AS) -march=$(ARCH) -mcpu=$(MCPU) -g -o $@ $<
