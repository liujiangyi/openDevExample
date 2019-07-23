#!/bin/bash
outFile="build_log.c"
inFile="$1"
if [ -f $inFile ];then
    if [ -f $outFile ];then
	> $outFile    
    else
	touch $outFile
    fi
    grep  "): error" $inFile > $outFile 
    grep  "Error:"  $inFile >> $outFile
    echo -e '\n' >> $outFile
    grep  "): warning" $inFile >> $outFile
    dos2unix $outFile
fi
