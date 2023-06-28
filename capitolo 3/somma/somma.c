#include "../../mylib.h"


struct NUMBER sum (struct NUMBER num1, struct NUMBER num2) {
	// initialize variables
	struct NUMBER result;
	int l1 = num1.length, l2 = num2.length;
	int lmax = max (l1, l2) + 1; // maximum length of the result

	// allocate memory
	int* d = calloc (lmax, sizeof(int)); // result digits

	// calculate result digits
	int carry = 0, c1 = 0, c2 = 0, s = 0;
	for (int k = 0; k < lmax; k++) {
		c1 = (k < l1) ? num1.digits[k] : 0 ; // check if it's within the boundaries of the array
		c2 = (k < l2) ? num2.digits[k] : 0 ;

		s = c1 + c2 + carry;

		d[k] = get_last_digit (s);
		carry = (int) (s / 10);
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
	int M = mod9 (init_NUMBER (m1 + m2));

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

	struct NUMBER result = sum (num1, num2);
	print_NUMBER (result);

	cast_out_9 (num1, num2, result);


	free (num1.digits);
	free (num2.digits);
	free (result.digits);
	return 0;
}



// take two numbers, add them and output their sum