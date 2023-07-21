#include "../../mylib.h"


struct NUMBER addition (struct NUMBER num1, struct NUMBER num2) {
	// initialize variables
	struct NUMBER result;
	int l1 = num1.length, l2 = num2.length;
	int lmax = max (l1, l2) + 1; // maximum length of the result

	for (int k = 0; k < lmax-l1; k++) {
		printf ("0");
	}
	print_NUMBER (num1);
	printf ("\n");
	for (int k = 0; k < lmax-l2; k++) {
		printf ("0");
	}
	print_NUMBER (num2);
	printf ("\n\n");

	// allocate memory
	int* d = calloc (lmax, sizeof(int)); // result digits

	// calculate result digits
	int carry = 0, c1 = 0, c2 = 0, s = 0; // c1 = current digit of the first number (idem c2 for the second one), s = partial sum of current digits plus the carry
	for (int k = 0; k < lmax; k++) {
		c1 = (k < l1) ? num1.digits[k] : 0 ; // check if it's within the boundaries of the array
		c2 = (k < l2) ? num2.digits[k] : 0 ;

		s = c1 + c2 + carry;
		printf ("%d + %d + %d = %d\n", c1, c2, carry, s);

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

void cast_out_9 (struct NUMBER num1, struct NUMBER num2, struct NUMBER result) { // check the correctness of the operation by casting out nines
	int m1 = mod9 (num1),
	    m2 = mod9 (num2),
		mr = mod9 (result);
	int M = (m1 + m2) % 9;

	int n1 = NUMBER_to_int (num1),
	    n2 = NUMBER_to_int (num2),
		nr = NUMBER_to_int (result);
	printf ("%d %% 9 = %d\n", n1, m1);
	printf ("%d %% 9 = %d\n", n2, m2);
	printf ("%d %% 9 = %d\n", nr, mr);

	printf ("\n");
	if (M == mr) {
		printf ("correct");
	}
	else {
		printf ("wrong");
	}
}


int main (void) {
	// initialize variables
	int n1, n2;
	struct NUMBER num1, num2;

	scanf ("%d%d", &n1, &n2);
	num1 = init_NUMBER (n1);
	num2 = init_NUMBER (n2);
	printf ("\n----------\n\n");

	// call functions
	struct NUMBER result;
	result = addition (num1, num2);
	printf ("\n");
	print_NUMBER (result);
	printf ("\n\n----------\n\n");

	cast_out_9 (num1, num2, result);


	// free memory
	free (num1.digits);
	free (num2.digits);
	free (result.digits);

	return 0;
}



// add two numbers
// also check the result by casting out nines

// INPUT: 2 numbers
// OUTPUT: the steps of the addition
//         1 number, result of the operation
//         the remainders by 9
//         the result of casting out nines ("correct" or "wrong")



// examples


// 25 + 49 = 74

// 123 + 4567 = 4690

// 4321 + 506789 = 511110