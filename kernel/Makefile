
OSNAME = CoffeeOS

GNUEFI = ../gnu-efi
OVMFDIR = ../OVMFbin
LDS = kernel.ld
CC = gcc
 
LD = ld

CFLAGS = -Iinclude -ffreestanding -fshort-wchar -nostdlib -fno-builtin -fno-rtti -fno-exceptions -O0 -g

LDFLAGS = -T $(LDS) -static -Bsymbolic -nostdlib

SRCDIR := src
OBJDIR := lib
BUILDDIR = bin
FONTDIR = fonts
BOOTEFI := $(GNUEFI)/x86_64/bootloader/main.efi

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard,$(SRCDIR),*.cpp) 
        
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
 
DIRS = $(wildcard $(SRCDIR)/*)

setup:
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(SRCDIR)
	@mkdir -p $(OBJDIR)

buildimg:
	dd if=/dev/zero of=$(BUILDDIR)/$(OSNAME).hdd bs=512 count=93750
	mformat -i $(BUILDDIR)/$(OSNAME).hdd -f 1440 ::
	mmd -i $(BUILDDIR)/$(OSNAME).hdd ::/EFI
	mmd -i $(BUILDDIR)/$(OSNAME).hdd ::/fonts
	mmd -i $(BUILDDIR)/$(OSNAME).hdd ::/EFI/BOOT
	mcopy -i $(BUILDDIR)/$(OSNAME).hdd $(BOOTEFI) ::/EFI/BOOT
	mcopy -i $(BUILDDIR)/$(OSNAME).hdd startup.nsh ::
	mcopy -i $(BUILDDIR)/$(OSNAME).hdd $(BUILDDIR)/kernel.elf ::
	mcopy -i $(BUILDDIR)/$(OSNAME).hdd $(FONTDIR)/zap-light16.psf ::/fonts
	mcopy -i $(BUILDDIR)/$(OSNAME).hdd $(FONTDIR)/zap-light18.psf ::/fonts
	mcopy -i $(BUILDDIR)/$(OSNAME).hdd $(FONTDIR)/zap-light24.psf ::/fonts


$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo !=== COMPILING $^
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@

 


link: $(OBJS)
	@echo !=== LINKING
	$(LD) $(LDFLAGS) -o $(BUILDDIR)/kernel.elf $(OBJS) $(OBJS2)

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
