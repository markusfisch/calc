#!/usr/bin/env bash

readonly BIN=./calc

[ -x $BIN ] || make || exit 1

# Test run calc
#
# @param 1 - string to calculate
# @param 2 - expected result
test_calc() {
	local RESULT=`$BIN "$1" 2>&1`
	[ "$RESULT" != "$2" ] && {
		echo "FAIL: $1 != $RESULT (should be $2)"
		exit 1
	}
	echo "PASS: $1 == $RESULT"
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
