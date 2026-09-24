CC = x86_64-elf-gcc
LD = x86_64-elf-ld
ASM = nasm

CFLAGS = -ffreestanding -m64 -O2 -Wall -Wextra -mcmodel=kernel -mno-red-zone -Ikernel

BUILD = build
ISO_DIR = iso

KERNEL = $(BUILD)/kernel.elf
ISO = Trikernel.iso

LIMINE_DIR = /usr/share/limine

C_SRC := $(shell find kernel -type f -name '*.c')
ASM_SRC := $(shell find kernel -type f -name '*.asm')

C_OBJ := $(patsubst %.c,$(BUILD)/%.o,$(C_SRC))
ASM_OBJ := $(patsubst %.asm,$(BUILD)/%.o,$(ASM_SRC))

OBJ := $(C_OBJ) $(ASM_OBJ)


all: $(ISO)


$(BUILD):
	mkdir -p $(BUILD)


$(BUILD)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


$(BUILD)/%.o: %.asm
	mkdir -p $(dir $@)
	$(ASM) -f elf64 $< -o $@


$(KERNEL): $(OBJ) linker.ld
	$(LD) -T linker.ld -nostdlib -o $@ $(OBJ)


$(ISO): $(KERNEL)
	rm -rf $(ISO_DIR)

	mkdir -p $(ISO_DIR)/boot
	mkdir -p $(ISO_DIR)/EFI/BOOT

	cp $(KERNEL) $(ISO_DIR)/boot/kernel.elf
	cp limine.conf $(ISO_DIR)/limine.conf

	cp $(LIMINE_DIR)/BOOTX64.EFI \
		$(ISO_DIR)/EFI/BOOT/BOOTX64.EFI

	cp $(LIMINE_DIR)/limine-bios-cd.bin \
		$(ISO_DIR)/limine-bios-cd.bin

	cp $(LIMINE_DIR)/limine-uefi-cd.bin \
		$(ISO_DIR)/limine-uefi-cd.bin

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

	limine bios-install $(ISO)

run: $(ISO)
	rm -f OVMF_VARS.4m.fd
	cp /usr/share/edk2/x64/OVMF_VARS.4m.fd .
	python3 tool/smbios.py
	qemu-system-x86_64 -d in_asm -D qemu.log \
		-machine pc \
		-m 128m \
		-drive if=pflash,format=raw,readonly=on,file=/usr/share/edk2/x64/OVMF_CODE.4m.fd \
		-drive if=pflash,format=raw,file=OVMF_VARS.4m.fd \
		-cdrom Trikernel.iso \
		-display gtk
clean:
	rm -rf $(BUILD) $(ISO_DIR) *.iso OVMF_VARS.4m.fd