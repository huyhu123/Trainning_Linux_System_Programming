#!/bin/bash

# Read the number of integers
read n
# Check if input is int
if ! [[ $n =~ ^[0-9]+$ ]]; then
   echo "Invalid Input"
   exit
fi

# Read the array
read -a arr
# Check length of input
if ! [ ${#arr[@]} -eq $n ]
then
    echo "Invalid Input"
    exit
fi
# Check if input is int
for ((i=0; i<n; i++))
do
    if ! [[ ${arr[$i]} =~ ^[0-9]+$ ]]
    then
        echo "Invalid Input"
        exit
    fi
done

# Loop through each element in the array
for ((i=0; i<n; i++))
do
    # Counter variable
    count=0

    # Loop through the array
    for ((j=0; j<n; j++))
    do
        if [ $i -ne $j ] && [ ${arr[i]} = ${arr[j]} ]
        then
            count=$((count+1))
        fi
    done

    # Check if current element occurs only once
    if [ $count -eq 0 ]
    then
        echo -n ${arr[i]}
        echo -n " "
    fi
done

echo ""