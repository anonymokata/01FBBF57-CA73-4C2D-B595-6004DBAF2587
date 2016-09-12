/*
roman_numeral_calc.c

Andrew Howard - 2016 

This file defines library functions that allow the user to add and subtract Roman numeral numbers.  Other functions facilitate the conversion between Roman numerals and decimal numbers.  The largest Roman numeral supported is 3999 (MMMCMXCIX) and the smallest is 1 (I).  

*/

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "roman_numeral_calc.h"

//Roman numeral symbols and associated decimal values.  These are used 
//in the conversion between decimal numbers and Roman numerals.  
static const char roman_symbol[] = {'M','D','C','L','X','V','I'};
static const int decimal_symbol[] = {1000, 500, 100, 50, 10, 5, 1};
static const int num_symbol = 7;

/* Convert decimal numbers (1-3999) to Roman numerals.  The function writes to a C string provided by the caller.  The array must be large enough to store the characters of the numerals and the null-terminating character.  A '0' value is returned if the conversion was successful.  A '1' value is returned if the conversion fails due to invalid input. */
int decimal_to_roman(const int decimal, char * numeral) {

	//Flag set to non-zero value if input is incorrect or conversion 
	//fails.  
	int flag = 0;

	//First check if number is within the accepted range. 
	if(decimal < MIN_DECIMAL || decimal > MAX_DECIMAL) {
		flag = 1;
		return flag;
	}

	//Check for a null pointer.  
	if(numeral == NULL) {
		flag = 1;
		return flag;
	}

	//Ensure the string is clean before writing over it.  
	memset(numeral, 0, strlen(numeral));

	//Generate buffer string used when generating Roman numerals.  
	char * buffer = (char*)malloc(sizeof(char) + (strlen(MAX_ROMAN)+1));
	memset(buffer, 0, (strlen(MAX_ROMAN)+1));

	//Temporary integer to store value of remaining decimal number 
	//as Roman numerals are added to the string.  
	int dtemp = decimal;

	/* When converting from decimal to Roman numerals, there are four 
	sections within each decimal place (powers of 10) that are 
	represented different by numerals.  
	
	i) Immediately less the higher power of 10 (i.e. 9 -> IX)
	ii) Less than the highest number but not less than the middle 
		value. (i.e. 8 -> VIII)
	iii) Immediately less than the middle value.  (i.e. 4 -> IV)
	iv) Lowest values (i.e. 2 -> II)
	
	The rules for converting decimal numbers to Roman numerals can be 
	applied systematically to each decimal place, so long as these 
	four regions are processed accordingly for the respective Roman 
	numeral values.  
	*/

	//As the largest decimal number allowed is 3999, only the lowest 
	//region of the 1000's place is checked.  This is a special case.
	if(dtemp >= 1000) {

		//Count the 1000's in the decimal number.  Rounds down. 
		int n = floor(((float)dtemp)/1000.0);

		//Add 1000's by appending "M" several times.  Maximum of 3. 
		for(int i=0; i<n; i++) {

			//Append "M" to end of numeral buffer string.
			strcat(buffer,"M");

			//Subtract 1000 from decimal value.  
			dtemp -= 1000;
		}
	}
	
	//For each decimal place from the 100's to the 1's, convert to 
	//Roman numerals.  "symbol_iter" starts at 2 because that is the 
	//index for the 100s decimal value and Roman numeral symbol C.  
	for(int symbol_iter=2; symbol_iter < num_symbol; symbol_iter+=2) {
	
		//decimal value that represents the decimal place
		int dplace = decimal_symbol[symbol_iter];
	
		//Decimal number values representing the four cases of values 
		//within the current decimal place.  
		int val9 = 9 * dplace;
		int val5 = 5 * dplace;
		int val4 = 4 * dplace;
		int val1 = dplace;
		
		if(dtemp >= val9) {

			//Append the Roman numeral for the value immediately 
			//before the next highest decimal place.  That is, "CM", 
			//"XC", or "IX".  
			buffer[strlen(buffer)] = roman_symbol[symbol_iter];
			buffer[strlen(buffer)] = roman_symbol[symbol_iter-2];

			//Subtract the value from dtemp
			dtemp -= val9;
		}
	
		//Check if dtemp contains the middle value for the current 
		//decimal place.   
		if(dtemp >= val5) {
	
			//Append Roman numeral for middle value to buffer.
			buffer[strlen(buffer)] = roman_symbol[symbol_iter-1];

			//Subtract middle value from dtemp.  
			dtemp -= val5;
		}
	
		//Check if dtemp is immediately less than the middle value. 
		if(dtemp >= val4) {
		
			//Append Roman numerals for the value immediately less 
			//than the middle value.  (i.e. IV)
			buffer[strlen(buffer)] = roman_symbol[symbol_iter];
			buffer[strlen(buffer)] = roman_symbol[symbol_iter-1];

			//Subract from dtemp.
			dtemp -= val4;
		}
	
		//Check dtemp for value in the lowest portion of the current 
		//decimal place.  
		if(dtemp >= val1) {
	
			//Count the multiples of val1 in dtemp. Round down. 
			int n = floor(((float)dtemp)/((float)val1));

			//Add numeral for current decimal place.  Maximum of 3. 
			for(int i=0; i<n; i++) {

				//Append numeral for current decimal place to end of 
				//numeral buffer string.
				buffer[strlen(buffer)] = roman_symbol[symbol_iter];

				//Subtract val1 from remaining decimal value.  
				dtemp -= val1;
			}
		}
	}

	//Copy contents of buffer to "numeral". 
	strncpy(numeral, buffer, strlen(buffer)+1);

	//dtemp should equal zero now, with all value extracted and 	
	//converted to Roman numerals.  If not, something went wrong.  
	if(dtemp != 0) {
		flag = 1;
		return flag;
	}

	//Free buffer memory
	free(buffer);

	return flag;
}

/* Convert Roman numerals to decimal numbers in the range 1-3999.  The function is passed a C string containing the Roman numerals to convert and a pointer to the integer variable that will receive the converted decimal value.  A '0' value is returned if the conversion was successful.  A '1' value is returned if the conversion fails due to invalid input. */
int roman_to_decimal(const char * numeral, int * decimal) {

	//temporary variable to store decimal number as it is built  
	int dtemp = 0;
	
	//Copy the original numeral to another c string that can be 
	//manipulated as need.  
	char * ntemp = (char*)malloc(sizeof(char) * (strlen(numeral)+1));
	memcpy(ntemp, numeral, strlen(numeral)+1);
	 
	//If the numeral string is preceeded by M's, these represent 
	//values of 1000.  There should be 3 maximum.  
	int count=0;
	while(strchr(ntemp,'M') == ntemp) {
	
		//Add 1000 to decimal value.  
		dtemp += 1000;
		
		//Shift the characters of the string so the first is 
		//dropped off.  The null terminating character is copied 
		//over as well, so the string's length is correctly 
		//shortened.  
		memmove(ntemp, ntemp+1, strlen(ntemp));
		
		count++;
		
		//Check for more than 3 M's in a row.    
		if(count > 3) {
		
			//Incorrectly formated Roman numeral, return. 
			free(ntemp);
			return 1;
		}
	}
	
	/* Converting from Roman numerals to decimal numbers follows a 
	similar approach as the reverse conversion that is described in 
	"decimal_to_roman()" above.  Within each decimal place, strings 
	representing 9, 5, 4, and 1 are searched for within "ntemp".  As 
	they are identified in leading positions in the numeral string, 
	they are removed from the front of the string and their equivalent 
	value is added to the growing decimal value, stored in "dtemp".  
	*/
	
	//For each decimal place from the 100's to the 1's, search for the 
	//Roman numerals that represent the respective values of 9, 5, 4, 
	//and 1.  "symbol_iter" starts at 2 because that is the 
	//index for the 100s decimal value and Roman numeral symbol C.  
	for(int symbol_iter=2; symbol_iter < num_symbol; symbol_iter+=2) {
	
		//decimal value that represents the decimal place
		int dplace = decimal_symbol[symbol_iter];
	
		//Decimal number values representing the four cases of values 
		//within the current decimal place.  
		int val9 = 9 * dplace;
		int val5 = 5 * dplace;
		int val4 = 4 * dplace;
		int val1 = dplace;
	
		//Roman numeral strings that correspond to the above decimal values.  All are null-terimated c strings.  
		char numstr9[] = {'\0', '\0', '\0'};
		numstr9[0] = roman_symbol[symbol_iter];
		numstr9[1] = roman_symbol[symbol_iter-2];
		
		char numstr5[] = {'\0', '\0'};
		numstr5[0] = roman_symbol[symbol_iter-1];
		
		char numstr4[] = {'\0', '\0', '\0'};
		numstr4[0] = roman_symbol[symbol_iter];
		numstr4[1] = roman_symbol[symbol_iter-1];
		
		char numstr1[] = {'\0', '\0'};
		numstr1[0] = roman_symbol[symbol_iter];
	
		//Check for numeral string that represents the 9 value.   	
		if(strstr(ntemp, numstr9)) {
	
			//Check if the string is at beginning.  
			if(strstr(ntemp, numstr9) == ntemp) {
		
				//Add the corresponding 9 value to the decimal.
				dtemp += val9;
		
				//Remove beginning two characters of string.  
				memmove(ntemp, ntemp+1, strlen(ntemp));
				memmove(ntemp, ntemp+1, strlen(ntemp));
			}
			else {
				//Incorrectly formated Roman numeral, return. 
				free(ntemp);
				return 1;
			}
		}
	
		//Check for numeral for 5, not necessarily at the beginning.  
		if(strstr(ntemp, numstr5)) {
	
			//Check for leading 5 numeral.  
			if(strstr(ntemp, numstr5) == ntemp) {
		
				//Add the corresponding 5 value to the decimal. 
				dtemp += val5;
		
				//Shift the characters of the string so the first is 
				//dropped off.  The null terminating character is copied 
				//over as well, so the string's length is correctly 
				//shortened.  
				memmove(ntemp, ntemp+1, strlen(ntemp));
			}
			else if(strstr(ntemp, numstr4) == ntemp) {
				//Check for leading numeral for 4 value.  
			
				//Add corresponding 4 value to the decimal.   
				dtemp += val4;
		
				//Shift the characters of the string so the first two 
				//are dropped off.  The null terminating character is 
				//copied twice over as well, so the string's length is 
				//correctly shortened.  
				memmove(ntemp, ntemp+1, strlen(ntemp));
				memmove(ntemp, ntemp+1, strlen(ntemp));
			
				//With leading numeral for 4 value, there should be no 
				//following numerals for the 1 value.  
				if(strstr(ntemp,"numstr1")) {
			
					//Incorrectly formated Roman numeral, return. 
					free(ntemp);
					return 1;
				}
			}
			else {
			
				//Incorrectly formated Roman numeral, return. 
				free(ntemp);
				return 1;
			}
		}
	
		//If the string starts with numerals for the 1 value, here 
		//should be 3 maximum.  
		int count=0;
		while(strstr(ntemp, numstr1) == ntemp) {
	
			//Add corresponding 1 value to the decimal. 
			dtemp += val1;
		
			//Shift the characters of the string so the first is 
			//dropped off.  The null terminating character is copied 
			//over as well, so the string's length is correctly 
			//shortened.  
			memmove(ntemp, ntemp+1, strlen(ntemp));
		
			count++;
		
			//Check for more than 3 numerals for 1 values in a row.    
			if(count > 3) {
		
				//Incorrectly formated Roman numeral, return. 
				free(ntemp);
				return 1;
			}
		}
	}
	
	//Free the temporary Roman numeral string.
	free(ntemp);

	//Store the final converted decimal number.  
	*decimal = dtemp;

	return 0;
}


