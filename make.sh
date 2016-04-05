#!/bin/sh

make CROSS_COMPILE=/opt/buildroot/imx6-dev/host/usr/bin/arm-buildroot-linux-gnueabihf- $@
cp SPL u-boot.img ./BIN_BSJ
