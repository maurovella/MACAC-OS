#!/bin/bash

if [[ "$1" == "gdb" ]];
then
     qemu-system-x86_64 -s -S -hda Image/x64BareBonesImage.qcow2 -m 512 -d int -audiodev coreaudio,id=audio0 -machine pcspk-audiodev=audio0
elif [[ "$1" == "-na" ]]; #no audio
then
     qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512
else
     qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -audiodev coreaudio,id=audio0 -machine pcspk-audiodev=audio0
fi
