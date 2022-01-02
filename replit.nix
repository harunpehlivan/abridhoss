{ pkgs }: {
	deps = [
		pkgs.qemu
		pkgs.gcc
		pkgs.binutils
		pkgs.grub2
		pkgs.xorriso
	];
}