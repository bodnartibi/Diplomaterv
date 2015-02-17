#!/bin/bash
sudo umount ./temp
rm -rf temp
gzip noheader.uramdisk.image
mkimage -A arm -O linux -T ramdisk -C gzip -d noheader.uramdisk.image.gz uramdisk.image.gz
rm noheader.uramdisk.image.gz
