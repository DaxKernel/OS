#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/DaxOS.kernel isodir/boot/DaxOS.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "DaxOS"{
	multiboot /boot/DaxOS.kernel
}
EOF
grub-mkrescue -o DaxOS.iso isodir
