#!/bin/bash
#program to calculate simple interest.

#check if exactly 3 arguments are passed.
if [ $# -ne 3 ]
then
    echo "Usage: $0 <Principal> <Rate> <Time>"
    exit 1
fi

#check if all arguments are decimal numbers.
if ! [[ $1 =~ ^[0-9]+([.][0-9]+)?$ && $2 =~ ^[0-9]+([.][0-9]+)?$ && $3 =~ ^[0-9]+([.][0-9]+)?$ ]]
then
    echo "Error: All inputs must be numeric."
    exit 1
fi

P=$1
R=$2
T=$3

#calculate simple interest.
SI=$(echo "scale=2; ($P*$R*$T)/100" | bc)

#calculate total amount.
TOTAL=$(echo "scale=2; $P+$SI" | bc)

echo "Simple Interest = $SI"
echo "Total Amount = $TOTAL"
