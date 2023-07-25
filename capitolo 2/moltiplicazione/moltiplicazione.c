#include "../../mylib.h"


struct NUMBER multiplication (struct NUMBER num1, struct NUMBER num2) {
	// initialize variables
	struct NUMBER result;
	int l1 = num1.length,	    l2 = num2.length;
	int lmax = l1 + l2; // maximum length of the result

		for (int k = 0; k < lmax-l1; k++) {
			printf ("0");
		}
		print_NUMBER (num1);	printf ("\n");
		for (int k = 0; k < lmax-l2; k++) {
			printf ("0");
		}
		print_NUMBER (num2);	printf ("\n");
		printf ("\n");

	// allocate memory
	int* d = calloc (lmax, sizeof(int)); // result digits

	// calculate result digits
	int carry = 0, c1 = 0, c2 = 0, s = 0; // c1 = current digit of the first number (idem c2 for the second one), s = partial sum of products of current digits plus the carry
	for (int k = 0; k < lmax; k++) {
		s = 0;
		for (int i = 0; i <= k; i++) {
			int j = k-i;
			c1 = (i < l1) ? num1.digits[i] : 0 ; // check if it's within the boundaries of the array
			c2 = (j < l2) ? num2.digits[j] : 0 ;

			s += c1 * c2;
				printf ("%dx%d + ", c1, c2, s);
		}
		s += carry;
			printf ("%d = %d\n", carry, s);

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
	// calculate remainders
	int m1 = mod9 (num1),	    m2 = mod9 (num2),	    mr = mod9 (result);
	int M = (m1 * m2) % 9;

	// output
		print_NUMBER (num1);	printf (" %% 9 = %d\n", m1);
		print_NUMBER (num2);	printf (" %% 9 = %d\n", m2);
		print_NUMBER (result);	printf (" %% 9 = %d\n", mr);

		printf ("\n");
		printf ( (M == mr) ? "correct" : "wrong");
}


int main (void) {
	// initialize variables
	int n1, n2;
	struct NUMBER num1, num2;

	scanf ("%d%d", &n1, &n2);
	num1 = init_NUMBER (n1);
	num2 = init_NUMBER (n2);
		printf ("\n----------\n\n");

	// call functions and output
	struct NUMBER result;
	result = multiplication (num1, num2);
		printf ("\n");   print_NUMBER (result);
		printf ("\n\n----------\n\n");

	cast_out_9 (num1, num2, result);


	// free memory
	free (num1.digits);
	free (num2.digits);
	free (result.digits);

	return 0;
}



// multiply two numbers
// also check the result by casting out nines

// INPUT: 2 numbers
// OUTPUT: the steps of the operation
//         1 number, result of the operation
//         the remainders by 9
//         the result of casting out nines ("correct" or "wrong")



// examples


// 12 x 12 = 144

// 37 x 37 = 1369

// 98 x 98 = 9604

// 70 x 70 = 4900

// 37 x 49 = 1813


// 8 x 49 = 392

// 7 x 308 = 2156

// 70 x 81 = 5670


// 345 x 345 = 119025

// 607 x 607 = 368449

// 780 x 780 = 608400

// 900 x 900 = 810000

// 123 x 456 = 56088

// 370 x 451 = 166870

// 320 x 570 = 182400


// 1234 x 1234 = 1522756

// 2345 x 6789 = 15920205

// 5000 x 7000 = 35000000

// 5100 x 7430 = 37893000

// 2500 x 3701 = 9252500

// 12345 x 12345 = 152399025


// 12345678 x 87654321 = 1082152022374638

// 345 x 698541 = 240996645