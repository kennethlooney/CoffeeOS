
OSNAME = CoffeeOS

GNUEFI = ../gnu-efi
OVMFDIR = ../OVMFbin
LDS = kernel.ld
CC = gcc
LD = ld

CFLAGS = -ffreestanding -fshort-wchar -O0 -g
LDFLAGS = -T $(LDS) -static -Bsymbolic -nostdlib

SRCDIR := src
OBJDIR := lib
BUILDDIR = bin
BOOTEFI := $(GNUEFI)/x86_64/bootloader/main.efi

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard,$(SRCDIR),*.c)          
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
DIRS = $(wildcard $(SRCDIR)/*)

setup:
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(SRCDIR)
	@mkdir -p $(OBJDIR)

buildimg:
	dd if=/dev/zero of=$(BUILDDIR)/$(OSNAME).hdd bs=512 count=93750
	mformat -i $(BUILDDIR)/$(OSNAME).hdd -f 1440 ::
	mmd -i $(BUILDDIR)/$(OSNAME).hdd ::/EFI
	mmd -i $(BUILDDIR)/$(OSNAME).hdd ::/EFI/BOOT
	mcopy -i $(BUILDDIR)/$(OSNAME).hdd $(BOOTEFI) ::/EFI/BOOT
	mcopy -i $(BUILDDIR)/$(OSNAME).hdd startup.nsh ::
	mcopy -i $(BUILDDIR)/$(OSNAME).hdd $(BUILDDIR)/kernel.elf ::

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo !=== COMPILING $^
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@

link: $(OBJS)
	@echo !=== LINKING
	$(LD) $(LDFLAGS) -o $(BUILDDIR)/kernel.elf $(OBJS)

kernel: link

.PHONY: build
build:
	make setup
	make kernel
	make buildimg

.PHONY: clean
clean:
	@echo !=== CLEANING KERNEL 
	rm -rd $(BUILDDIR) $(OBJDIR)



run:
	qemu-system-x86_64 -drive file=$(BUILDDIR)/$(OSNAME).hdd -m 256M -cpu qemu64 -drive if=pflash,format=raw,unit=0,file="$(OVMFDIR)/OVMF_CODE-pure-efi.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="$(OVMFDIR)/OVMF_VARS-pure-efi.fd" -net none
