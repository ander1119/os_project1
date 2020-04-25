#!/bin/sh
sudo dmesg -c
mkdir output

inputFileName=$(echo $1 | cut -d '/' -f2 | cut -d '.' -f1)
outputFilePath="output/${inputFileName}_stdout.txt"
dmesgFilePath="output/${inputFileName}_dmesg.txt"

sudo ./main < $1 2>/dev/null > outputFilePath
sudo dmesg -c | grep Project1 > dmesgFilePath

