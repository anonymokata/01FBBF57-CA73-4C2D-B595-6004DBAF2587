/*
roman_numeral_calc.c

Andrew Howard - 2016 

This file defines library functions that allow the user to add and subtract Roman numeral numbers.  Other functions facilitate the conversion between Roman numerals and decimal numbers.  The largest Roman numeral supported is 3999 (MMMCMXCIX) and the smallest is 1 (I).  

*/

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include "roman_numeral_calc.h"

/* Convert decimal numbers (1-3999) to Roman numerals.  The function writes to a C string character array provided by the caller.  The array must be large enough to store the characters of the numerals and the null-terminating character.   */ 
void decimal_to_roman(const int i, char * numeral) {

	//Ensure the string is clean before writing over it.  
	memset(numeral, 0, strlen(numeral));

	switch(i) {
		case 1:
			strcpy(numeral,"I");
			break;
		case 4:
			strcpy(numeral,"IV");
			break;
	}
}


