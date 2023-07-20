#include "../../mylib.h"


void mask (struct FRACTION frac1, struct FRACTION frac2, char s) {
	printf ("\n");
	print_FRACTION (frac1);
	printf (" %c ", s);
	print_FRACTION (frac2);
	printf (" = ");
}

void op_v1 (struct FRACTION frac1, struct FRACTION frac2) {
	int a1 = frac1.num, b1 = frac1.den;
	int a2 = frac2.num, b2 = frac2.den;
	
	int m = mcm (b1, b2);
	int N1 = (m / b1) * a1, 	    N2 = (m / b2) * a2;
	
	struct FRACTION result;
	
	// addition
	result.num = N1 + N2;
	result.den = m;
	simplify (&result);
	mask (frac1, frac2, '+');
	print_FRACTION (result);
	
	// subtraction
	if (N1 >= N2) { // negative numbers are not contemplated
		result.num = N1 - N2;
		result.den = m;
		simplify (&result);
		mask (frac1, frac2, '-');
		print_FRACTION (result);
	}
	else {
		result.num = N2 - N1;
		result.den = m;
		simplify (&result);
		mask (frac2, frac1, '-');
		print_FRACTION (result);
	}
	
	// division
	result.num = N1;
	result.den = N2;
	simplify (&result);
	mask (frac1, frac2, '/');
	print_FRACTION (result);
	
	result.num = N2;
	result.den = N1;
	simplify (&result);
	mask (frac2, frac1, '/');
	print_FRACTION (result);
}

void op_v2 (struct FRACTION frac1, struct FRACTION frac2) {
	int a1 = frac1.num, b1 = frac1.den;
	int a2 = frac2.num, b2 = frac2.den;
	
	int d = MCD (b1, b2), m = mcm (b1, b2);
	int N1 = a1 * (b2 / d), 	    N2 = a2 * (b1 / d);
		
	
	struct mixedNUMBER result;
	
	// addition
	result = mix_div (N1 + N2, m);
	mask (frac1, frac2, '+');
	print_mixedNUMBER (result);
	
	// subtraction
	if (N1 >= N2) { // negative numbers are not contemplated
		result = mix_div (N1 - N2, m);
		mask (frac1, frac2, '-');
		print_mixedNUMBER (result);
	}
	else {
		result = mix_div (N2 - N1, m);
		mask (frac2, frac1, '-');
		print_mixedNUMBER (result);
	}
	
	// division
	result = mix_div (N1, N2);
	mask (frac1, frac2, '/');
	print_mixedNUMBER (result);
	
	result = mix_div (N2, N1);
	mask (frac2, frac1, '/');
	print_mixedNUMBER (result);
}


int main (void) {
	int a1, b1, a2, b2;
	struct FRACTION frac1, frac2;
	
	scanf ("%d %d", &a1, &b1);
	scanf ("%d %d", &a2, &b2);
	frac1 = init_FRACTION (a1, b1);
	frac2 = init_FRACTION (a2, b2);
	
	op_v1 (frac1, frac2);
	printf ("\n");
	op_v2 (frac1, frac2);
	
	return 0;
}



// add, subtract and divide two simple fractions
// use two different algorithms

// INPUT: 4 numbers, 2 for the first fraction and 2 for the second one
// OUTPUT: the results of the operations grouped per algorithm



// examples


// 1/3 + 1/4 = 7/12

// 1/3 - 1/4 = 1/12
//           = 1,0/3,4

// 1/3 / 1/4 = 1/3|1
//           = 4/3

// 1/4 / 1/3 = 3/4


// 2/3 + 4/5 = 7/15|1
//           = 1,2/3,5|1

// 4/5 - 2/3 = 2,0/3,5
//           = 2/15

// 4/5 / 2/3 = 1/15|1

// 2/3 / 4/5 = 5/6


// 5/6 + 7/10 = 16/30|1
//            = 8/15|1
//            = 1,5/3,10|1

// 5/6 - 7/10 = 1,1/3,10

// 5/6 / 7/10 = 1,1/3,7|1

// 7/10 / 5/6 = 1,4/5,5


// 1/6 + 5/9 = 1,6/2,9

// 5/9 - 1/6 = 1,3/2,9

// 5/9 / 1/6 = 1/3|3

// 1/6 / 5/9 = 3/10