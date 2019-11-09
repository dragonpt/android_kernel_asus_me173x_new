#!/bin/bash

export CROSS_COMPILE="./toolchain/arm-unknown-linux-gnueabi-linaro_4.6.4-2013.05/bin/arm-gnueabi-"
export TARGET_PRODUCT=asus
export MTK_ROOT_CUSTOM=./mediatek

echo "**** START CLEANING ****"
make mrproper
make distclean

rm -rf ./mediatek/config/out/
rm -rf ./mediatek/custom/out/
rm -rf ./out/
rm -rf ./build_result
rm -rf kernelFile

find . -name "modules.builtin" -type f -print -delete

echo "**** DONE CLEANING ****"

