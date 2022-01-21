clear
export PATH=$PATH:/usr/local/i386elfgcc/bin

i386-elf-gcc -ffreestanding -m32 -g -c "Kernel.c" -o "Kernel.o"
i386-elf-gcc -ffreestanding -m32 -g -c "Utils.c" -o "Utils.o"
i386-elf-gcc -ffreestanding -m32 -g -c "IDT.c" -o "IDT.o"
i386-elf-gcc -ffreestanding -m32 -g -c "ISRs.c" -o "ISRs_c.o"
i386-elf-gcc -ffreestanding -m32 -g -c "IRQs.c" -o "IRQs_c.o"

nasm "Kernel_entry.asm" -f elf -o "Kernel_entry.o"
nasm "ISRs.asm" -f elf -o "ISRs_asm.o"
nasm "IRQs.asm" -f elf -o "IRQs_asm.o"
nasm "Cursor.asm" -f elf -o "Cursor.o"

i386-elf-ld -o "full_kernel.bin" -Ttext 0x1000 "Kernel_entry.o" "Kernel.o" "Utils.o" "IDT.o" "ISRs_c.o" "ISRs_asm.o" "IRQs_c.o" "IRQs_asm.o" "Cursor.o" --oformat binary 

nasm "Bootloader.asm" -f bin -o "Bootloader.bin"
nasm "Ceros.asm" -f bin -o "Ceros.bin"

cat "Bootloader.bin" "full_kernel.bin" > "everything.bin"
cat "everything.bin" "Ceros.asm" > "OS.bin"

qemu-system-x86_64 -drive format=raw,file="OS.bin",index=0,if=floppy, -m 128M