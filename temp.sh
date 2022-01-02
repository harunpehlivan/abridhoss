problem=0
printf "checking if qemu exists... "
if [[ $(which qemu-system-x86_64) ]]; then
    echo "yes"
else
    echo "no"
	problem=1
fi
printf "checking if gcc exists... "
if [[ $(which gcc) ]]; then
    echo "yes"
else
    echo "no"
	problem=1
fi
printf "checking if nasm exists... "
if [[ $(which nasm) ]]; then
    echo "yes"
else
    echo "no"
	problem=1
fi

printf "checking if ld exists... "
if [[ $(which ld) ]]; then
    echo "yes"
else
    echo "no"
	problem=1
fi

if [ "$problem" == "1" ]; then
	echo Please install the correct programs to run Abrid. This is likely QEMU, so we will install it.
	install-pkg qemu
fi


sources=$(find ./kernel/* -type f -name "*.c")
objects=$(echo ${sources//\.c/.o})
objb=''
char=' '
end=$(awk -F"${char}" '{print NF-1}' <<< "${objects}")
end=$((end+1))
# echo $List | awk '{print $2}'
nasm -f elf ./boot/boot.asm -o ./bin/boot.o

for i in $(seq 1 $end); do 
	ta=$(echo ./bin/$(basename $(echo $objects | cut -d" " -f$i )))
	tb=$(echo $sources | cut -d" " -f$i)
	objb="${objb} ${ta}"
	gcc -m32 -elf_i386 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./kernel/include -fno-stack-protector -c -o $ta $tb
done
objb="${objb:1}"
ld -T link.ld -m elf_i386 -o os.bin $objb ./bin/boot.o
cd qemu
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:. qemu-system-i386 -kernel os.bin -m 128M