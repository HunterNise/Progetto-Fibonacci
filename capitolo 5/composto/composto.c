#include "../../mylib.h"


void cast_out_X (int n1, struct mixedNUMBER result) {
	// initialize variables
	int l = result.pfrac.length;
	int* primes = calloc (l, sizeof(int)); // store primes (dynamic programming)
	int verif, isprime, j = 0, p = 7; // verif = flag controlling the termination of the cycle, isprime = flag checking whether p is prime, j = primes counter (gives position of last encountered prime), p = current prime (start from 7)

	// find a prime number different from the denominators
	for ( ; 1 ; p++) { // infinite cycle (until broken); at each iteration p gets incremented
		isprime = 1;
		if ((p % 2 == 0) || (p % 3 == 0) || (p % 5 == 0)) { // check divisibility by 2,3,5
			isprime = 0;
			continue;
		}
		for (int k = 0; k < j; k++) { // check divisibility by all other smaller primes
			if (p % primes[k] == 0) {
				isprime = 0;
				break;
			}
		}

		if (isprime == 1) {
			if (j == l) { // checked as many primes as the number of denominators
				break;
			}

			// check p is already in the denominators
			verif = 1;
			for (int k = 0; k < l; k++) {
				if (p == result.pfrac.den[k]) {
					verif = 0;
					primes[j] = p;
					j++;
					break;
				}
			}

			if (verif == 1) {
				break;
			}
		}
	}

	// calculate remainders
	int m1 = n1 % p, mr = result.pint % p;
	for (int k = 0; k < l; k++) {
		mr *= result.pfrac.den[k];
		mr += result.pfrac.num[k];
		mr = mr % p;
	}

	// output
	printf ("\n\nmod %d: ", p);
	if (m1 == mr) {
		printf ("correct");
	}
	else {
		printf ("wrong");
	}

	// free memory
	free (primes);
}


int main (void) {
	setbuf (stdout, NULL); // for buggy output


	int n1, n2;

	scanf ("%d%d", &n1, &n2);

	struct mixedNUMBER result = mix_div (n1, n2);
	print_mixedNUMBER (result);

	cast_out_X (n1, result);


	free (result.pfrac.num);
	free (result.pfrac.den);
	fflush (stdout); // for buggy output
	return 0;
}



// divide by a composite number by finding his prime factors
// also check the result by casting out a prime number that is not already present in the factors of the denominator

// INPUT: 2 numbers
// OUTPUT: 1 mixed number, result of the division
//         the result of casting out and the modulo chosen



// examples

// 749 / 75 = 2,4,4/3,5,5|9
//       75 = 3×5×5

// 67898 / 1760 = 0,5,3,6/2,8,10,11|38
//         1760 = 2×8×10×11

// 81540 / 8190 = 3,12/7,13|9
//         8190 = 7x9x10x13