#include "../../mylib.h"


struct DIVresult {
	struct NUMBER quotient;
	struct NUMBER rem_num;
	struct NUMBER rem_den;
};


void shift (struct NUMBER num, int a) { // shift the digits and add a new one in the units place
	int l = num.length;
	for (int k = l-1; k > 0; k--) {
		num.digits[k] = num.digits[k-1];
	}
	num.digits[0] = a;
}

void detract (struct NUMBER* ptx_num1, struct NUMBER num2) { // detract the second number from the first
	// initialize variables
	struct NUMBER num1 = *ptx_num1;
	int l1 = num1.length, l2 = num2.length;
	int lmax = l1; // maximum length of the result
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

	// update digits
	free (ptx_num1->digits);
	ptx_num1->digits = d;
}


struct DIVresult division (struct NUMBER num1, struct NUMBER num2) {
	// initialize variables
	struct DIVresult result;
	int l1 = num1.length, l2 = num2.length;
	int lmax = l1; // maximum length of the result

	// allocate memory
	int* d = calloc (lmax, sizeof(int)); // quotient digits

	int* r = calloc (lmax, sizeof(int)); // remainder digits
	for (int k = 0; k < lmax; k++) {
		r[k] = num1.digits[k];
	}

	struct NUMBER small;
	small.length = l2 + 1;
	small.digits = calloc (small.length, sizeof(int));


	// calculate quotient and remainder digits
	for (int k = lmax-1; k >= 0; k--) {

		shift (small, r[k]);

		// this is the black box
		int q = 0;
		while (geq (small, num2)) {
			detract (&small, num2);
			q++;
		}

		d[k] = q;
		for (int j = 0; j < small.length; j++) {
			r[k+j] = small.digits[j];
		}
	}
	result.quotient.digits = d;
	result.rem_num.digits = r;
	result.rem_den.digits = num2.digits;


	// calculate quotient and remainder length
	int k = lmax-1;
	while (d[k] == 0 && k > 0) { // ignore trailing 0s (0s at the beginning of the number)
		k--;
	}
	result.quotient.length = k+1;

	k = lmax-1;
	while (r[k] == 0 && k >= 0) { // ignore trailing 0s (0s at the beginning of the number)
		k--;
	}
	result.rem_num.length = k+1;

	result.rem_den.length = l2;

	return result;
}


int main (void) {
	setbuf (stdout, NULL); // for buggy output


	int n1, n2;
	struct NUMBER num1, num2;

	scanf ("%d%d", &n1, &n2);
	num1 = init_NUMBER (n1);
	num2 = init_NUMBER (n2);

	struct DIVresult result = division (num1, num2);
	print_NUMBER (result.rem_num);
	printf ("/");
	print_NUMBER (result.rem_den);
	printf (" ");
	print_NUMBER (result.quotient);


	free (num1.digits);
	free (num2.digits);
	free (result.quotient.digits);
	free (result.rem_num.digits);
	free (result.rem_den.digits);
	return 0;
}



// take two numbers, divide them and output their rest (in fraction form) and the quotient