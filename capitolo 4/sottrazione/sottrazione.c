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
		c1 = (k < l1) ? num1.digits[k] : 0 ; // check if it's within the boundary of the array
		c2 = (k < l2) ? num2.digits[k] : 0 ;

		borrow = (c1 < c2) ? 1 : 0; // check if borrow is needed
		s = (c1 + borrow*10) - (c2 + carry);

		d[k] = get_last_digit (s);
		carry = borrow;
	}
	result.digits = d;

	// calculate result length
	int k = lmax-1;
	while (d[k] == 0 && k >= 0) { // ignore trailing 0s (0s at the beginning of the number)
		k--;
	}
	result.length = (k >= 0) ? k+1 : 1 ; // exception: if result = 0 then k = -1, but length should be 1

	return result;
}


int main (void) {
	int n1, n2;
	struct NUMBER num1, num2;

	scanf ("%d%d", &n1, &n2);
	num1 = init (n1);
	num2 = init (n2);

	if (n1 < n2) { // negative numbers are not contemplated
		printf ("Impossibile eseguire la sottrazione.");
		return 0;
	}

	struct NUMBER result = sub (num1, num2);
	print (result);


	free (num1.digits);
	free (num2.digits);
	free (result.digits);
	return 0;
}