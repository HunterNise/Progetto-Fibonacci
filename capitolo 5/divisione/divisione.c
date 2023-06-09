#include "../../mylib.h"


struct DIVresult { // integer and fractional part
	struct NUMBER pint;
	struct NUMBER pfrac_num;
	struct NUMBER pfrac_den;
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
	int* d = calloc (lmax, sizeof(int)); // integer part digits

	int* r = calloc (lmax, sizeof(int)); // fractional part digits
	for (int k = 0; k < lmax; k++) {
		r[k] = num1.digits[k];
	}

	struct NUMBER small; // partial dividend
	small.length = l2 + 1;
	small.digits = calloc (small.length, sizeof(int));


	// calculate integer and fractional part
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
	result.pint.digits = d;
	result.pfrac_num.digits = r;
	result.pfrac_den.digits = num2.digits;


	// calculate integer and fractional part lengths
	int k = lmax-1;
	while (d[k] == 0 && k > 0) { // ignore trailing 0s (0s at the beginning of the number)
		k--;
	}
	result.pint.length = k+1;

	k = lmax-1;
	while (r[k] == 0 && k >= 0) { // ignore trailing 0s (0s at the beginning of the number)
		k--;
	}
	result.pfrac_num.length = k+1;

	result.pfrac_den.length = l2;

	return result;
}

void cast_out_9 (struct NUMBER num1, struct NUMBER num2, struct DIVresult result) {
	int m1 = mod9 (num1), m2 = mod9 (num2), mq = mod9 (result.pint), mr = mod9 (result.pfrac_num);
	int M1 = mod9 (init_NUMBER (m2 * mq)), M2 = mod9 (init_NUMBER (M1 + mr));

	if (M2 == m1) {
		printf ("\n\ncorrect");
	}
	else {
		printf ("\n\nwrong");
	}
}


int main (void) {
	setbuf (stdout, NULL); // for buggy output


	int n1, n2;
	struct NUMBER num1, num2;

	scanf ("%d%d", &n1, &n2);
	num1 = init_NUMBER (n1);
	num2 = init_NUMBER (n2);

	struct DIVresult result = division (num1, num2);
	print_NUMBER (result.pfrac_num);
	printf ("/");
	print_NUMBER (result.pfrac_den);
	printf (" ");
	print_NUMBER (result.pint);

	cast_out_9 (num1, num2, result);


	free (num1.digits);
	free (num2.digits);
	free (result.pint.digits);
	free (result.pfrac_num.digits);
	free (result.pfrac_den.digits);
	return 0;
}



// take two numbers, divide them and output their rest (in fraction form) and the pint