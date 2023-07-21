#include "../../mylib.h"


struct NUMBER subtraction (struct NUMBER num1, struct NUMBER num2) {
	// initialize variables
	struct NUMBER result;
	int l1 = num1.length, l2 = num2.length;
	int lmax = l1; // maximum length of the result

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
	int borrow = 0, carry = 0, c1 = 0, c2 = 0, s = 0;
	for (int k = 0; k < lmax; k++) {
		c1 = (k < l1) ? num1.digits[k] : 0 ; // check if it's within the boundaries of the array
		c2 = (k < l2) ? num2.digits[k] : 0 ;

		borrow = (c1 < c2) ? 1 : 0; // check if borrow is needed
		s = (c1 + borrow*10) - (c2 + carry);
		printf ("%d - %d = %d\n", c1 + borrow*10, c2 + carry, s);

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

void cast_out_9 (struct NUMBER num1, struct NUMBER num2, struct NUMBER result) { // check the correctness of the operation by casting out nines
	int m1 = mod9 (num1),
	    m2 = mod9 (num2),
		mr = mod9 (result);

	int M;
	if (m1 >= m2) {
		M = (m1 - m2) % 9;
	}
	else {
		M = ((m1 + 9) - m2) % 9;
	}

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

	// check the input
	int verif = geq (num1, num2);
	if (!verif) {
		printf ("Impossibile eseguire la sottrazione."); // negative numbers are not contemplated
		return 0;
	}

	// call functions
	struct NUMBER result;
	result = subtraction (num1, num2);
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



// subtract two numbers
//   if the first number is smaller than the second one, give an error message
// also check the result by casting out nines

// INPUT: 2 numbers
// OUTPUT: the steps of the subtraction
//         1 number, result of the operation
//         the remainders by 9
//         the result of casting out nines ("correct" or "wrong")



// examples


// 89 - 35 = 54

// 85 - 39 = 46

// 392 - 80 = 312

// 380 - 92 = 288

// 939 - 457 = 482

// 15738 - 841 = 14897

// 81728 - 28391 = 53337

// 8383 - 4562 = 3821