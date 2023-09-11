#!/bin/bash

TOTAL=0

is_float () {
    if [[ $1 =~ ^[+-]?[0-9]+$ ]]
    then
        return 0 #int
    elif [[ $1 =~ ^[+-]?[0-9]+\.$ ]]
    then
        return 1 #string
    elif [[ $1 =~ ^[+-]?[0-9]+\.?[0-9]*$ ]]
    then
        return 0 #float
    else
        return 1 #string
    fi
}

#Base case
is_float $1
if [ $? = 1 ]
then
    echo "Invalid input"
    exit
fi
TOTAL=$1
shift

# Handle expression
case $1 in
    +)
        shift
        is_float $1
        if [ $? = 1 ]
        then
            echo "Invalid input"
            exit
        fi
        TOTAL=$(bc <<< "$TOTAL+$1")
        ;;
    -)
        shift
        is_float $1
        if [ $? = 1 ]
        then
            echo "Invalid input"
            exit
        fi
        TOTAL=$(bc <<< "$TOTAL-$1")
        ;;
    x)
        shift
        is_float $1
        if [ $? = 1 ]
        then
            echo "Invalid input"
            exit
        fi
        TOTAL=$(bc <<< "scale=2;$TOTAL*$1")
        ;;
    /)
        shift
        is_float $1
        if [ $? = 1 ]
        then
            echo "Invalid input"
            exit
        fi
        TOTAL=$(bc <<< "scale=2;$TOTAL/$1")
        ;;
    *)
        echo "Invalid input"
        exit
        ;;
esac

# Check if too much input
shift
if [ "$#" -gt "0" ]
then
    echo "Invalid input"
    exit
fi

echo $TOTAL