#include "../../mylib.h"


struct DIVresult {
	struct multFRACTION pfrac; // fractional part
	int pint; // integer part
};


void copy (list head, int* v, int k) { // copy a list into a vector
	if (!head) {
		return;
	}
	v[k] = head->info;
	copy (head->next, v, k+1);
}


int factorize (int n, list* ptx_head) { // calculate the prime factors and store them in a vector
	int p = 2;
	while (p*p <= n) { // it suffices to reach the square root
		if (n % p == 0) {
			add_front (ptx_head, p);
			return factorize (n/p, ptx_head) + 1;
		}
		p++;
	}
	add_front (ptx_head, n);
	return 1;
}


struct DIVresult division (int n1, int n2) {
	// initialize variables
	list fact = NULL; // list of prime factors (stored in descending order)
	int l = factorize (n2, &fact);

	struct DIVresult result;
	result.pfrac.circle = -1;
	result.pfrac.length = l;
	result.pfrac.num = calloc (l, sizeof(int));
	result.pfrac.den = calloc (l, sizeof(int));
	result.pint = 0;
	copy (fact, result.pfrac.den, 0);

	// calculate result digits
	int m = n1;
	for (int k = l-1; k >= 0; k--) {
		result.pfrac.num[k] = m % result.pfrac.den[k];
		m /= result.pfrac.den[k];
	}
	result.pint = m;

	return result;
}

void cast_out_X (int n1, struct DIVresult result) {
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

	struct DIVresult result = division (n1, n2);
	print_multFRACTION (result.pfrac);
	printf (" %d", result.pint);

	cast_out_X (n1, result);


	free (result.pfrac.num);
	free (result.pfrac.den);
	return 0;
}



// divide by a composite number by finding his prime factors