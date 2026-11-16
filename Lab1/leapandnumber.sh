#!/bin/bash

#program to check leap year and number classification.

#read year.
read -p "Enter a year: " year

#validate year.
if ! [[ $year =~ ^[0-9]+$ ]]
then
    echo "Invalid year."
    exit 1
fi

#leap year check.
if (( (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0) ))
then
    echo "$year is a Leap Year."
else
    echo "$year is NOT a Leap Year."
fi

#read number.
read -p "Enter a number: " num

#validate number.
if ! [[ $num =~ ^-?[0-9]+$ ]]
then
    echo "Invalid number."
    exit 1
fi

#check if the number is even or odd.
if (( num % 2 == 0 ))
then
    echo "$num is Even."
else
    echo "$num is Odd."
fi

#check if the number is positive/negative/zero.
if (( num > 0 ))
then
    echo "$num is Positive."
elif (( num < 0 ))
then
    echo "$num is Negative."
else
    echo "$num is Zero."
fi
