CC=x86_64-elf-gcc
LD=x86_64-elf-ld
CCARG = -ffreestanding -m64 -O2 -Wall -Wextra -mcmodel=kernel -mno-red-zone -Ikernel
ASM=nasm
LIB=kernel/libs/
BUILD=build
ISO_DIR=iso
KERNEL=$(BUILD)/kernel.elf
ISO=Trikernel.iso

LIMINE_DIR=/usr/share/limine


all: $(ISO)


$(BUILD):
	mkdir -p $(BUILD)


$(BUILD)/entry.o: kernel/entry.asm | $(BUILD)
	$(ASM) -f elf64 kernel/entry.asm -o $@


$(BUILD)/kernel.o: kernel/kernel.c | $(BUILD)
	$(CC) \
		$(CCARG) \
		-c kernel/kernel.c \
		-o $@

$(BUILD)/font.o: kernel/screen/font.c | $(BUILD)
	$(CC) \
		$(CCARG) \
		-c kernel/screen/font.c \
		-o $@


$(BUILD)/screen.o: kernel/screen/screen.c | $(BUILD)
	$(CC) \
		$(CCARG) \
		-c kernel/screen/screen.c \
		-o $@
	
$(BUILD)/math.o: $(LIB)/math/math.c | $(BUILD)
	$(CC) \
		$(CCARG) \
		-c $(LIB)/math/math.c \
		-o $@

$(BUILD)/keyboard.o: kernel/keyboard/keyboard.c | $(BUILD)
	$(CC) \
		$(CCARG) \
		-c kernel/keyboard/keyboard.c \
		-o $@


$(KERNEL): $(BUILD)/entry.o $(BUILD)/kernel.o $(BUILD)/screen.o $(BUILD)/font.o $(BUILD)/math.o $(BUILD)/keyboard.o
	$(LD) \
		-T linker.ld \
		-nostdlib \
		-o $@ \
		$^

$(ISO): $(KERNEL)
	rm -rf $(ISO_DIR)

	mkdir -p $(ISO_DIR)/boot
	mkdir -p $(ISO_DIR)/EFI/BOOT

	# Kernel
	cp $(KERNEL) $(ISO_DIR)/boot/kernel.elf
	
	# Limine config
	cp limine.conf $(ISO_DIR)/limine.conf

	# UEFI loader
	cp $(LIMINE_DIR)/BOOTX64.EFI \
		$(ISO_DIR)/EFI/BOOT/BOOTX64.EFI

	# Limine El Torito images
	cp $(LIMINE_DIR)/limine-bios-cd.bin \
		$(ISO_DIR)/limine-bios-cd.bin

	cp $(LIMINE_DIR)/limine-uefi-cd.bin \
		$(ISO_DIR)/limine-uefi-cd.bin


	# Crea ISO ibrida BIOS + UEFI
	xorriso \
		-as mkisofs \
		-o $(ISO) \
		-V LIMINE \
		-R \
		-J \
		-b limine-bios-cd.bin \
		-no-emul-boot \
		-boot-load-size 4 \
		-boot-info-table \
		-eltorito-alt-boot \
		-e limine-uefi-cd.bin \
		-no-emul-boot \
		-append_partition 2 0xef $(ISO_DIR)/limine-uefi-cd.bin \
		-appended_part_as_gpt \
		$(ISO_DIR)


	# Installa bootloader BIOS Limine
	limine bios-install $(ISO)

run: $(ISO)
	qemu-system-x86_64 \
		-machine q35 \
		-m 512M \
		-drive if=pflash,format=raw,readonly=on,file=/usr/share/edk2/x64/OVMF_CODE.4m.fd \
		-drive if=pflash,format=raw,file=OVMF_VARS.4m.fd \
		-cdrom $(ISO) \
		-serial stdio


clean:
	rm -rf $(BUILD) $(ISO_DIR) *.iso