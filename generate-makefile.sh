#!/bin/bash
LONG_BIT=`getconf LONG_BIT`
if [ $LONG_BIT -eq 64 ]; then
   echo "64-bit operating system detected."
   ./cbp2make.linux-x86_64 -in LibreScribe.cbp -out Makefile
elif [ $LONG_BIT -eq 32 ]; then
   echo "32-bit operating system detected."
   ./cbp2make.linux-x86 -in LibreScribe.cbp -out Makefile
else
   echo "An unsupported operating system was detected. Aborting."
fi

