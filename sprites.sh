#!/bin/sh

#grit nitrofiles/bg/down.bmp -g -gTFF00FF -m -mRtf -mLs -p -ftb

#mv down.img.bin nitrofiles/bg/down.img
#mv down.map.bin nitrofiles/bg/down.map
#mv down.pal.bin nitrofiles/bg/down.pal
#rm down.h

#grit nitrofiles/bg/up.bmp -g -gTFF00FF -m -mRtf -mLs -p -ftb

#mv up.img.bin nitrofiles/bg/up.img
#mv up.map.bin nitrofiles/bg/up.map
#mv up.pal.bin nitrofiles/bg/up.pal
#rm up.h

# ^^^ Fondos, no se usan

grit nitrofiles/sprite/s1.bmp -g -gB8 -p -gT000000 -gt -ftb

mv s1.img.bin nitrofiles/sprite/s1.img
mv s1.pal.bin nitrofiles/sprite/s1.pal
rm s1.h


grit nitrofiles/sprite/f1.bmp -g -gB8 -p -gT000000 -gt -ftb

mv f1.img.bin nitrofiles/sprite/f1.img
mv f1.pal.bin nitrofiles/sprite/f1.pal
rm f1.h
