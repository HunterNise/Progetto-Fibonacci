#include "../../mylib.h"


struct DIVresult {
	struct NUMBER quotient;
	struct NUMBER rem_num;
	struct NUMBER rem_den;
};


void shift (struct NUMBER small, int a) {
	int l = small.length;
	for (int k = 0; k < l-1; k++) {
		small.digits[k+1] = small.digits[k];
	}
	small.digits[0] = a;
}

int comp (struct NUMBER small, struct NUMBER num2) {
	int l = small.length;
	if (small[l-1] != 0) {
		return 1;
	}
	
	for (int k = l-2; k >= 0; k--) {
		if (small.digits[k] > num2.digits[k]) {
			return 1;
		}
		else if (small.digits[k] < num2.digits[k]) {
			return 0;
		}
	}
	return 1;
}

struct NUMBER sub (struct NUMBER num1, struct NUMBER num2, struct NUMBER result) {
	// initialize variables
	int l1 = num1.length, l2 = num2.length;
	int lmax = l1; // maximum length of the result
	int* d = result.digits; // result digits

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

	return result;
}


struct DIVresult div (struct NUMBER num1, struct NUMBER num2) {
	// initialize variables
	struct DIVresult result;
	int l1 = num1.length, l2 = num2.length;

	// allocate memory
	int* d = calloc (l1, sizeof(int)); // quotient digits
	
	int* rem = calloc (l1, sizeof(int)); // remainder digits
	for (int k = 0; k < l1; k++) {
		rem[k] = num1[k];
	}

	struct NUMBER small;
	small.length = l2+1;
	small.digits = calloc (small.length, sizeof(int));
	
	
	// calculate quotient and remainder digits
	for (int k = l1-1; k >= 0; k--) {
		shift (small, rem.digits[k]);
		
		int q = 0;
		while (comp (small, num2)) {
			small = sub (small, num2);
			q++;
		}
		
		d[k] = q;
		for (int j = 0; j < small.length; j++) {
			rem.digits[k+j] = small.digits[j];
		}
	}
	result.quotient.digits = d;
	result.rem_num.digits = rem;
	result.rem_den.digits = num2;

	// calculate quotient and remainder length
	int k = lmax-1;
	while (d[k] == 0 && k >= 0) { // ignore trailing 0s (0s at the beginning of the number)
		k--;
	}
	result.quotient.length = (k >= 0) ? k+1 : 1 ; // exception: if result = 0 then k = -1, but length should be 1
	
	int k = lmax-1;
	while (rem[k] == 0 && k >= 0) { // ignore trailing 0s (0s at the beginning of the number)
		k--;
	}
	result.rem_num.length = (k >= 0) ? k+1 : 1 ; // exception: if result = 0 then k = -1, but length should be 1
	
	result.rem_den.length = l2;
	
	return result;
}


int main (void) {
	int n1, n2;
	struct NUMBER num1, num2;

	scanf ("%d%d", &n1, &n2);
	num1 = init_NUMBER (n1);
	num2 = init_NUMBER (n2);

	struct DIVresult result = div (num1, num2);
	print (result.rem_num);
	printf ("/");
	print (result.rem_den);
	printf (" ");
	print (result.quotient);


	free (num1.digits);
	free (num2.digits);
	free (result.digits);
	return 0;
}