#!/bin/bash
#This script processes all of the stf files, and generates a png image from each one. Written by Dylan Taylor
#The resulting png files will be stored in the same location as the stf file, with ".png" appended to the filename.
#This script depends on stf.py and parsestf.py being in the same directory.
find . -nowarn -name '*.stf' -type f -exec echo "Processing " {} \; -exec python stf.py {} {}.png \; 
echo "STF File Processing Done! :-)"
