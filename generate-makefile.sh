#!/bin/bash
LONG_BIT=`getconf LONG_BIT`
if [ $LONG_BIT -eq 64 ]; then
   echo "64-bit operating system detected."
   ./cbp2make/cbp2make.linux-x86_64 -in LibreScribe.cbp -out Makefile
elif [ $LONG_BIT -eq 32 ]; then
   echo "32-bit operating system detected."
   ./cbp2make/cbp2make.linux-x86 -in LibreScribe.cbp -out Makefile
else
   echo "An unsupported operating system was detected. Aborting."
fi
#We want to modify our makefile to display the commands we run using xtrace for debugging purposes.
#While we could just use the --in-place option of sed instead of echo commands,
#older versions of sed do not support that option. This is for backwards compatibility.
#To disable modifications, run "export LS_NOMODIFY=1" before running this script.
if [ "$LS_NOMODIFY" != "1" ]; then 
   echo "Applying modifications to generated Makefile..."
   BINARY_VARS=('CC' 'CXX' 'AR' 'LD') #An array of all of the variables with binary commands
   for ((i=0;i<${#BINARY_VARS[@]};i++)); do #for every element in the binary_vars array
      echo "Processing variable \"${BINARY_VARS[${i}]}\"..."
      #First we need to change the binary values at the top of the Makefile...
      echo "`sh -c \"sed '0,/RE/s/\(^${BINARY_VARS[${i}]}.*= *\)/&sh -c -x \\\"/' < Makefile\"`" > Makefile
   done
   echo "Terminating non-terminated quoted commands..."
   #Now we have to terminate all of the quoted commands, so that we can run the Makefile
   echo "`sh -c \"sed 's/\(\s\+\\\$(.*) *.* -o \\\$(.*).*\)/&\\\"/' < Makefile\"`" > Makefile    
else 
   echo "Makefile modifications have been disabled by the \$LS_NOMODIFY variable."
   echo "To re-enable Makefile modifications, run the command \"unset LS_NOMODIFY\"."
fi
echo "Makefile generation complete."
