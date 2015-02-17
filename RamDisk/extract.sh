#!/bin/bash

dd if=uramdisk.image.gz of=noheader.uramdisk.image.gz bs=64 skip=1
gunzip noheader.uramdisk.image.gz
mkdir temp
sudo mount -o loop noheader.uramdisk.image ./temp/
