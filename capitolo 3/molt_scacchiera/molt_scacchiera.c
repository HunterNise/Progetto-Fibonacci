#include "../../mylib.h"


struct NUMBER matrix_mult (struct NUMBER num1, struct NUMBER num2) {
	// initialize variables
	struct NUMBER result;
	int l1 = num1.length,	    l2 = num2.length;
	int lmax = l1 + l2; // maximum length of the result
	int lrow = l1 + 1; // length of the matrix's rows

		for (int k = 0; k < lmax-l1-1; k++) {
			printf ("0");
		}
		print_NUMBER (num1);	printf ("\n");
		for (int k = 0; k < lmax-l2-1; k++) {
			printf ("0");
		}
		print_NUMBER (num2);	printf ("\n");
		printf ("\n");

	// allocate memory
	int* d = calloc (lmax, sizeof(int)); // result digits
	int** M = (int**) calloc (l2, sizeof(int*)); // matrix of intermediate products
	for (int k = 0; k < l2; k++) {
		M[k] = (int*) calloc (lrow, sizeof(int));
	}


	// intermediate products
	int carry = 0, c1 = 0, c2 = 0, s = 0; // c1 = current digit of the first number (idem c2 for the second one), s = partial sum of products of current digits plus the carry
	for (int j = 0; j < l2; j++) {
		carry = 0;
		for (int i = 0; i < lrow; i++) {
			c1 = (i < l1) ? num1.digits[i] : 0 ; // check if it's within the boundary of the array
			c2 = (j < l2) ? num2.digits[j] : 0 ;

			s = c1 * c2 + carry;

			M[j][i] = get_last_digit (s);
			carry = (int) (s / 10);
		}
	}

		for (int j = 0; j < l2; j++) {
			for (int i = lrow-1; i >= 0; i--) {
				printf ("%d", M[j][i]);
			}
			printf ("\n");
		}
		printf ("\n");


	// diagonal sum
	int c_j = 0; // current digit
	carry = 0;
	for (int k = 0; k < lmax; k++) {
		s = 0;
			printf ("%d  ", carry);
		for (int j = 0; j < l2; j++) {
			c_j = ((k-j >= 0) && (k-j < l1+1)) ? M[j][k-j] : 0 ; // check if it's within the boundaries of the array
				printf ("%d ", c_j);
			s += c_j;
		}
		s += carry;
			printf (" = %d\n", s);

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

	// free allocated memory
	for (int k = 0; k < l2; k++) {
		free (M[k]);
	}
	free (M);

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
	result = matrix_mult (num1, num2);
		printf ("\n");   print_NUMBER (result);
		printf ("\n\n----------\n\n");

	cast_out_9 (num1, num2, result);


	// free memory
	free (num1.digits);
	free (num2.digits);
	free (result.digits);

	return 0;
}



// multiply two numbers in matrix or checkerboard style
// also check the result by casting out nines

// INPUT: 2 numbers
// OUTPUT: the steps of the operation *
//         1 number, result of the operation
//         the remainders by 9
//         the result of casting out nines ("correct" or "wrong")

// * in the first section there are the intermediate products
//   in the second section the first number is the eventual carry,
//     followed by the digits of the intermediate products
//     and their sum at the end


// examples


// 4321 x 567 = 2450007