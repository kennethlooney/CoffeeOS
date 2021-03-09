all:
	cd gnu-efi && $(MAKE)
	cd gnu-efi && $(MAKE) bootloader
	cd kernel && $(MAKE) build

.PHONY: clean
clean:
	cd gnu-efi && $(MAKE) clean
	cd gnu-efi/bootloader && $(MAKE) clean
	cd kernel && $(MAKE) clean

