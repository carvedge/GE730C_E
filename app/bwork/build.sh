#!/bin/bash
make
arm-buildroot-linux-uclibcgnueabi-strip 0.mod
cp 0.mod /home/ge730c/nfs/ge730c/app
