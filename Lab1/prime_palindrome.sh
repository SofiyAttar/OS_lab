#!/bin/bash
#program to find if a number is prime and/or palindrome.

#function to check if the string is palindrome.
is_palindrome() {
	str=$1
	rev=$(echo "$str" | rev)
	if [ "$str" = "$rev" ]
	then
		echo "$str is Palindrome."
	else
		echo "$str is NOT Palindrome."
	fi
}
#function to check if a number is prime.
is_prime() {
	num=$1
	if ((num<2))
	then 
		echo "$num is NOT a Prime Number."
		return
	fi
	for ((i=2;i*i<=num;i++))
	do
		if ((num%i==0))
		then
			echo "$num is NOT a Prime Number."
			return 
		fi
	done
	echo "$num is a Prime Number."
}

#main function
read -p "Enter a string or number: " value

#check palindrome.
is_palindrome "$value"

#if numeric, check if it is prime.
if [[ $value =~ ^[0-9]+$ ]]
then
    is_prime "$value"
else
	echo "Not a valid number to find if it is prime."
fi
