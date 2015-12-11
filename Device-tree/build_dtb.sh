#!/bin/bash
XLNX_DIR="/home/etibbod/dipterv/linux-xlnx"

$XLNX_DIR/scripts/dtc/dtc -I dts -O dtb -o ./devicetree.dtb ./my_device_tree.dts 

