#!/bin/bash
#program to print first N fibonacci numbers and star pattern.
#reading a number.
read -p "Enter a number: " n
if ! [[ $n =~ ^[0-9]+$ ]]
then
	echo "Enter a valid positive number"
	exit 1
fi
echo "Fibonacci series"
a=0
b=1
#loop for finding next fibonacci number.
for ((i=1;i<=n;i++))
do 
	echo -n "$a "
	temp=$((a+b))
	a=$b
	b=$temp
done
echo
#printing star pattern.
for ((i=1;i<=n;i++))
do
	for ((j=1;j<=i;j++))
	do
		echo -n "* "
	done
	echo
done

