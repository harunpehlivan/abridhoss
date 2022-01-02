if [ ! -d bin ] 
then
    mkdir -p bin
fi
if [ $(dpkg-query -W -f='${Status}' qemu 2>/dev/null | grep -c "ok installed") -eq 0 ];
then
  install-pkg qemu
fi
if [ $(dpkg-query -W -f='${Status}' grub-pc 2>/dev/null | grep -c "ok installed") -eq 0 ];
then
  install-pkg grub-pc
fi


#assembly files
nasm -f elf ./boot/boot.asm -o ./bin/boot.o

#compile c source files
gcc -m32 -elf_i386 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./kernel/include -fno-stack-protector -c -o ./bin/main.o ./kernel/main.c
gcc -m32 -elf_i386 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./kernel/include -fno-stack-protector -c -o ./bin/ports.o ./kernel/ports.c
gcc -m32 -elf_i386 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./kernel/include -fno-stack-protector -c -o ./bin/screen.o ./kernel/screen.c
gcc -m32 -elf_i386 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./kernel/include -fno-stack-protector -c -o ./bin/mem.o ./kernel/lib/mem.c 
gcc -m32 -elf_i386 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./kernel/include -fno-stack-protector -c -o ./bin/fs.o ./kernel/fs.c
gcc -m32 -elf_i386 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./kernel/include -fno-stack-protector -c -o ./bin/gdt.o ./kernel/gdt.c
gcc -m32 -elf_i386 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./kernel/include -fno-stack-protector -c -o ./bin/idt.o ./kernel/idt.c
gcc -m32 -elf_i386 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./kernel/include -fno-stack-protector -c -o ./bin/isr.o ./kernel/isr.c
gcc -m32 -elf_i386 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./kernel/include -fno-stack-protector -c -o ./bin/irq.o ./kernel/irq.c
gcc -m32 -elf_i386 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./kernel/include -fno-stack-protector -c -o ./bin/timer.o ./kernel/timer.c
gcc -m32 -elf_i386 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./kernel/include -fno-stack-protector -c -o ./bin/keyboard.o ./kernel/keyboard.c
gcc -m32 -elf_i386 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./kernel/include -fno-stack-protector -c -o ./bin/string.o ./kernel/lib/string.c
gcc -m32 -elf_i386 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./kernel/include -fno-stack-protector -c -o ./bin/kshell.o ./kernel/kshell.c


#linker scripts
ld -T link.ld -m elf_i386 -o os.bin ./bin/boot.o ./bin/main.o ./bin/ports.o ./bin/screen.o ./bin/mem.o ./bin/gdt.o ./bin/idt.o ./bin/isr.o ./bin/irq.o ./bin/timer.o ./bin/keyboard.o ./bin/string.o ./bin/kshell.o ./bin/fs.o

#make iso file
mkdir -p isodir/boot/grub
cp os.bin isodir/boot/os.bin
cp grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o abrid-hos-x86.iso isodir
qemu-system-i386 -cdrom abrid-hos-x86.iso  
