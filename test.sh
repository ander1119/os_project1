#!/bin/sh

# this file is to automatically generate output file

DIR="~/OS_PJ1_Test/"
for FILE in "${DIR}RR"*
do
	sudo dmesg -c

	inputFileName=$(echo $FILE | cut -d '/' -f2 | cut -d '.' -f1)
	outputFilePath="output/${inputFileName}_stdout.txt"
	dmesgFilePath="output/${inputFileName}_dmesg.txt"

	sudo ./main < "${FILE}" 2>/dev/null > "${outputFilePath}"
	sudo dmesg -c | grep Project1 > "${dmesgFilePath}"
done




