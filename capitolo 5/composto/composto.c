#include "../../mylib.h"


struct mixedNUMBER division (int n1, int n2) {
	// initialize variables
	list fact = NULL; // list of prime factors (stored in descending order)
	int l = factorize (n2, &fact);

	struct mixedNUMBER result;
	result.pfrac.circle = -1;
	result.pfrac.length = l;
	result.pfrac.num = calloc (l, sizeof(int));
	result.pfrac.den = calloc (l, sizeof(int));
	
	result.pint = 0;
	copy (fact, result.pfrac.den);

	// calculate numerators
	int m = n1;
	for (int k = l-1; k >= 0; k--) {
		result.pfrac.num[k] = m % result.pfrac.den[k];
		m /= result.pfrac.den[k];
	}
	result.pint = m;

	return result;
}

void cast_out_X (int n1, struct mixedNUMBER result) {
	// initialize variables
	int l = result.pfrac.length;
	int* primes = calloc (l, sizeof(int));
	int verif, isprime, k = 0, p = 7;

	// find a prime number different from the denominators
	while (1) {
		isprime = 1;
		for (int j = 0; j < k; j++) {
			if (p % primes[j] == 0) {
				isprime = 0;
				break;
			}
		}

		if (isprime == 1) {
			if (k == l) {
				break;
			}

			verif = 1;
			for (int j = 0; j < l; j++) {
				if (p == result.pfrac.den[j]) {
					verif = 0;
					primes[k] = p;
					k++;
					break;
				}
			}

			if (verif == 1) {
				break;
			}
		}

		p++;
	}

	// calculate remainders
	int m1 = n1 % p, mr = result.pint % p;
	for (int j = 0; j < l; j++) {
		mr *= result.pfrac.den[j];
		mr += result.pfrac.num[j];
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

	struct mixedNUMBER result = division (n1, n2);
	print_mixedNUMBER (result);

	cast_out_X (n1, result);


	free (result.pfrac.num);
	free (result.pfrac.den);
	return 0;
}



// divide by a composite number by finding his prime factors



// examples

// 749/75 = 2,4,4/3,5,5;9
// 75 = 3×5×5

// 67898/1760 = 0,5,3,6/2,8,10,11;38
// 1760 = 2×8×10×11

// 81540/8190 = 3,12/7,13;9
// 8190 = 7x9x10x13