#!/bin/bash

while [ "$#" -gt "0" ]
do
    EXP+=$1
    shift
done 

#echo $EXP
TOTAL=$(bc <<< "scale=2;$EXP")

echo $TOTAL