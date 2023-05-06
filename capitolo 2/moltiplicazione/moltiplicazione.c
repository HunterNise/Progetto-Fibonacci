#include "../../mylib.h"


struct NUMBER mult (struct NUMBER num1, struct NUMBER num2) {
	// initialize variables
	struct NUMBER result;
	int l1 = num1.length, l2 = num2.length;
	int lmax = num1.length + num2.length; // maximum length of the result

	// allocate memory
	int* d = calloc (lmax, sizeof(int)); // result digits

	// calculate result digits
	int carry = 0, c1 = 0, c2 = 0, s = 0;
	for (int k = 0; k < lmax; k++) {

		s = 0;
		for (int i = 0; i <= k; i++) {
			int j = k-i;
			c1 = (i < l1) ? num1.digits[i] : 0 ; // check if it's within the boundary of the array
			c2 = (j < l2) ? num2.digits[j] : 0 ;

			s += c1 * c2;
		}
		s += carry;

		d[k] = get_last_digit (s);
		carry = (int) (s / 10);
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
	num1 = init_NUMBER (n1);
	num2 = init_NUMBER (n2);

	struct NUMBER result = mult (num1, num2);
	print (result);


	free (num1.digits);
	free (num2.digits);
	free (result.digits);
	return 0;
}