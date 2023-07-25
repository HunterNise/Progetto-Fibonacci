#include "../../mylib.h"


struct mixedNUMBER division (int n1, int n2) { // divide two numbers and return a mixed number [mix_div in mylib.h]
	// initialize variables
	int* denvec = simple_den (factorize (n2)); // factorize the denominator
	int l = length_vec (denvec);

		printf ("%d = ", n2);
		for (int k = l-1; k >= 0; k--) {
			printf ("%d", denvec[k]);
			printf ( (k != 0) ? "x" : ""); // don't print after the last number
		}
		printf ("\n\n");

	struct mixedNUMBER result;
	result.pfrac.circle = -1;
	result.pfrac.length = l;
	result.pfrac.num = calloc (l, sizeof(int));
	result.pfrac.den = denvec;

	// calculate numerators
	int m = n1;
	for (int k = l-1; k >= 0; k--) {
			printf ("%d", m);
		result.pfrac.num[k] = m % result.pfrac.den[k];
		m /= result.pfrac.den[k];
			printf (" = %d x %d + %d\n", result.pfrac.den[k], m, result.pfrac.num[k]);
	}
	result.pint = m;

	return result;
}

void cast_out_X (int n1, struct mixedNUMBER result) {
	// initialize variables
	int l = result.pfrac.length;
	int p = find_prime (result.pfrac.den, l); // find a prime number different from the denominators
		printf ("mod %d\n\n", p);

	// calculate remainders
	int m1 = n1 % p;				printf ("%d %% %d = %d\n", n1, p, m1);
	int mr = result.pint % p;		printf ("%d -> %d\n", result.pint, mr);
	for (int k = 0; k < l; k++) {
									printf ("%d", mr);
		mr *= result.pfrac.den[k];	printf (" x %d", result.pfrac.den[k]);
		mr += result.pfrac.num[k];	printf (" + %d", result.pfrac.num[k]);
		mr = mr % p;				printf (" -> %d\n", mr);
	}

	// output
		printf ("\n");
		printf ( (m1 == mr) ? "correct" : "wrong");
}


int main (void) {
	setbuf (stdout, NULL); // for buggy output


	// initialize variables
	int n1, n2;

	scanf ("%d%d", &n1, &n2);
		printf ("\n----------\n\n");

	// call functions and output
	struct mixedNUMBER result;
	result = division (n1, n2);
		printf ("\n");   print_mixedNUMBER (result);
		printf ("\n\n----------\n\n");

	cast_out_X (n1, result);


	// free memory
	free (result.pfrac.num);
	free (result.pfrac.den);

	fflush (stdout); // for buggy output

	return 0;
}



// divide by a composite number by finding his prime factors
// also check the result by casting out a prime number that is not already present in the factors of the denominator

// INPUT: 2 numbers
// OUTPUT: the "rule" of the denominator
//         the steps of the operation
//         1 mixed number, result of the operation
//         the modulo chosen and the remainders
//         the result of casting out ("correct" or "wrong")



// examples


// 749 / 75 = 2,4,4/3,5,5|9
//       75 = 3×5×5

// 67898 / 1760 = 0,5,3,6/2,8,10,11|38
//         1760 = 2×8×10×11

// 81540 / 8190 = 3,12/7,13|9
//             (= 4,2,4,12/7,9,10,13|9)
//         8190 = 7x9x10x13