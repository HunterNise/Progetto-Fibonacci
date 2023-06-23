#include "../../mylib.h"


struct NUMBER sub (struct NUMBER num1, struct NUMBER num2) {
	// initialize variables
	struct NUMBER result;
	int l1 = num1.length, l2 = num2.length;
	int lmax = l1; // maximum length of the result

	// allocate memory
	int* d = calloc (lmax, sizeof(int)); // result digits

	// calculate result digits
	int borrow = 0, carry = 0, c1 = 0, c2 = 0, s = 0;
	for (int k = 0; k < lmax; k++) {
		c1 = (k < l1) ? num1.digits[k] : 0 ; // check if it's within the boundaries of the array
		c2 = (k < l2) ? num2.digits[k] : 0 ;

		borrow = (c1 < c2) ? 1 : 0; // check if borrow is needed
		s = (c1 + borrow*10) - (c2 + carry);

		d[k] = get_last_digit (s);
		carry = borrow;
	}
	result.digits = d;

	// calculate result length
	int k = lmax-1;
	while (d[k] == 0 && k > 0) { // ignore trailing 0s (0s at the beginning of the number)
		k--;
	}
	result.length = k+1;

	return result;
}

void cast_out_9 (struct NUMBER num1, struct NUMBER num2, struct NUMBER result) {
	int m1 = mod9 (num1), m2 = mod9 (num2), mr = mod9 (result);
	
	int M;
	if (m1 >= m2) {
		M = mod9 (init_NUMBER (m1 - m2));
	}
	else {
		M = mod9 (init_NUMBER ((m1 + 9) - m2));
	}
	
	if (M == mr) {
		printf ("\n\ncorrect");
	}
	else {
		printf ("\n\nwrong");
	}
}


int main (void) {
	int n1, n2;
	struct NUMBER num1, num2;

	scanf ("%d%d", &n1, &n2);
	num1 = init_NUMBER (n1);
	num2 = init_NUMBER (n2);

	int verif = geq (num1, num2);
	if (!verif) {
		printf ("Impossibile eseguire la sottrazione."); // negative numbers are not contemplated
		return 0;
	}

	struct NUMBER result = sub (num1, num2);
	print_NUMBER (result);
	
	cast_out_9 (num1, num2, result);


	free (num1.digits);
	free (num2.digits);
	free (result.digits);
	return 0;
}



// take two numbers, subtract them and output their difference
// if the first number is smaller than the second one, it throws an exception