#!/bin/bash

readonly BIN=./calc

make clean && make

[ -x $BIN ] || exit 1

# Test calc
#
# @param 1 - string to calculate
# @param 2 - desired result
test_calc()
{
	local R=`$BIN "$1" 2>&1`

	[ "$R" != "$2" ] && {
		echo "FAILED! $1 != $R (should be $2)"
		exit 1
	}

	echo "$1 == $R"
}

test_calc 0 '0.000000'
test_calc 1 '1.000000'
test_calc '(3+' 'syntax error'
test_calc 'abc' 'syntax error'
test_calc '1+2' '3.000000'
test_calc '(1+2+27)/10' '3.000000'
test_calc '1+2+27/10' '5.700000'
test_calc '(30)/10' '3.000000'
test_calc '((30))/10' '3.000000'
test_calc '(((30))/10)' '3.000000'
test_calc '1<<4' '16.000000'
test_calc '16>>4' '1.000000'
test_calc '10 % 4' '2.000000'
test_calc '(10%4)*2.5' '5.000000'
test_calc '(10%4)<<2' '8.000000'
test_calc '4*4+21/7-3--3' '19.000000'
test_calc '4*-4*4/8*4*-1' '32.000000'
test_calc '3.1415*3.1415*2/1.34+3-7*8+(23*1.3+(87/3.2)*(12*3))' '970.379884'
test_calc ' 3.1415*  3.1415 *  2/ 1.34  +3 -7*  8+ ( 23 *1.3 +( 87/3.2)* ( 12*3) ) ' '970.379884'
