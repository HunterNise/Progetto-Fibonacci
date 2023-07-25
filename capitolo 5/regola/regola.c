#include "../../mylib.h"


void cast_out_X (int n, int* denvec, int l) {
	// initialize variables
	int p = find_prime (denvec, l); // find a prime number different from the denominators
		printf ("mod %d\n\n", p);

	// calculate remainders
	int m = n % p;					printf ("%d %% %d = %d\n", n, p, m);
	int M = denvec[l-1] % p;		printf ("%d -> %d\n", denvec[l-1], M);
	for (int k = l-2; k >= 0; k--) {
									printf ("%d x %d -> %d x %d", M, denvec[k], M, denvec[k] % p);
		M *= denvec[k] % p;			printf (" = %d", M);
		M = M % p;					printf (" -> %d\n", M);
	}

	// output
		printf ("\n");
		printf ( (M == m) ? "correct" : "wrong");
}


int main (void) {
	setbuf (stdout, NULL); // for buggy output


	// initialize variables
	int n;

	scanf ("%d", &n);

	// call functions and output
	int* denvec = simple_den (factorize (n)); // factorize the number
	int l = length_vec (denvec);

	if (l == 1) { // only one factor = prime
		printf ("%d is prime", n);
	}
	else {
		printf ("%d = ", n);
		for (int k = l-1; k >= 0; k--) {
			printf ("%d", denvec[k]);
			printf ( (k != 0) ? "x" : ""); // don't print after the last number
		}
		printf ("\n\n----------\n\n");

		cast_out_X (n, denvec, l);
	}


	// free memory
	free (denvec);

	fflush (stdout); // for buggy output

	return 0;
}



// find the "rule" of a number
//   this is equivalent to finding his prime factors (and beautify the factors)
// also check the result by casting out a prime number that is not already present in the factors

// INPUT: 1 number
// OUTPUT*: the sequence of factors
//          the modulo chosen and the remainders
//          the result of casting out ("correct" or "wrong")

// * if the number is prime the only ouptut is a message saying that



// examples


// 805 = 5×7×23

// 957 = 3×11×29

// 951 = 3×317

// 873 = 9x97

// 1469 = 13×113

// 2543 primo

// 624481 = 11×11×13×397

// 126 = 2×7×9

// 2112 = 4×6×8×11 =
//      = 3×8×8×11

// 4664 = 8×11×53

// 13652 = 4×3413

// 15560 = 4×10×389

// 32600 = 2×10×10×163

// 7546000 = 2×7×7×7×10×10×10×11