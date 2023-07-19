#include "../../mylib.h"


int main (void) {
	setbuf (stdout, NULL); // for buggy output
	
	
	char s[100];
	struct multFRACTION frac;

	gets (s);
	frac = init_multFRACTION (s);

	struct FRACTION result = mult_to_simple (frac);
	print_FRACTION (result);


	free (frac.num);
	free (frac.den);
	fflush (stdout); // for buggy output
	return 0;
}



// convert a multiple fraction to a simple fraction

// INPUT: 1 string that encode a multiple fraction
// OUTPUT: the simple fraction



// examples

// 1,4/2,7 = 9/14

// 1,0/2,7 = 1/14

// 1,5,7/2,6,10 = 95/120
//              = 19/24

// 2,4,6,8/3,5,7,9o = 2520/945
//                  = 8/3

// o8,6,4,2/9,7,5,3 = 384/945
//                  = 128/315