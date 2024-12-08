#!/bin/bash

BOARD=smarc_pmic_1
PLAT=g2l
CROSS_COMPILE=aarch64-linux-gnu-

# 清理临时文件
rm build bl2.srec fip.srec -rf

# 编译
make CROSS_COMPILE=$CROSS_COMPILE PLAT=$PLAT BOARD=$BOARD all

# 编译工具
cd tools/pack/ && make && cd -
cd tools/fiptool/ && make && cd -

# 制作输出文件
tools/pack/pack build/$PLAT/release/bl2.bin bl2.bin
tools/fiptool/fiptool create --align 16 --soc-fw build/$PLAT/release/bl31.bin --nt-fw /root/code/renesas-u-boot-cip/u-boot.bin fip.bin

# 文件转换
aarch64-linux-gnu-objcopy -I binary -O srec --adjust-vma=0x00000000 --srec-forceS3 fip.bin fip.srec
aarch64-linux-gnu-objcopy -I binary -O srec --adjust-vma=0x00011E00 --srec-forceS3 bl2.bin bl2.srec

# 删除临时文件
rm fip.bin bl2.bin