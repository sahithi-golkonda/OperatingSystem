#!/bin/bash
###################################################
## FILENAME:    runTraceTest.sh
## AUTHOR:      Jimmy Lin (u5223173) 
## DATE:        2013-09-19
## DESCRIPTION: 
##    bash script to run all trace tests and output
## them in text files.
##
###################################################
## Edited by MacVim
## Documentation autogenerated by snippet
###################################################


dir="."
mkdir $dir
mshOut="./$dir/MSHOUT.txt"
rm -f $mshOut
touch $mshOut

## loop through all trace files
for i in {1..16}
do
    ## trace output for our program ./msh
    echo "###################################################" >> $mshOut
    echo "### For Trace Test $i:" >> $mshOut
    echo "###################################################" >> $mshOut
    echo "" >> $mshOut
    if [ $i -lt 10 ]
    then
        ./sdriver.pl -t trace0$i.txt -s ./msh -a "-p" >> $mshOut
    else 
        ./sdriver.pl -t trace$i.txt -s ./msh -a "-p" >> $mshOut
    fi
    echo "" >> $mshOut
done 

echo "The trace test for my own msh program DONE.."
