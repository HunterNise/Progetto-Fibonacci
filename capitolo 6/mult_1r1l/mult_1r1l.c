#include "../../mylib.h"


struct MIXNUM { // a mixed number
	int pint; // integer part
	struct FRACTION pfrac; // fractional part (one simple fraction)
};

struct MIXNUM init_MIXNUM (char s[]) {
	// WARNING: this function does not sanitize input
	// it expects a string of the following pattern:
	//   number + "/" + number + "|" + number
	// whitespaces are ignored

	s = trim (s);
	int L = length_str (s);

	int slash = -1, pipe = L;

	for (int k = 0; s[k] != '\0'; k++) {
		switch (s[k]) {
			case '/': {
				slash = k;
			} break;
			case '|': {
				pipe = k;
			} break;
		}
	}


	struct MIXNUM mix;

	mix.pfrac.num = str_to_int (substr (s, 0, slash-1));
	mix.pfrac.den = str_to_int (substr (s, slash+1, pipe-1));
	mix.pint      = str_to_int (substr (s, pipe+1, L-1));


	return mix;
}


int numTOT (struct MIXNUM mix) {
	int N = mix.pint;

	N *= mix.pfrac.den;
	N += mix.pfrac.num;

	return N;
}

struct mixedNUMBER mult (struct MIXNUM mix1, struct MIXNUM mix2) {
	// calculate numerator
	int N1 = numTOT (mix1),	    N2 = numTOT (mix2);
	int N = N1 * N2;
		printf ("N1 = %d \t N2 = %d\n", N1, N2);
		printf ("N = %d\n\n", N);


	// calculate denominators
	int* denvec1 = factorize (mix1.pfrac.den),	   * denvec2 = factorize (mix2.pfrac.den);

	int l1 = length_vec (denvec1),	    l2 = length_vec (denvec2);
	int L = l1 + l2;

	// merge denominators in one array
	int* denvec_all = calloc (L+1, sizeof(int));
	for (int k = 0; k < l1; k++) {
		denvec_all[k] = denvec1[k];
	}
	for (int k = 0; k < l2; k++) {
		denvec_all[k+l1] = denvec2[k];
	}
	denvec_all[L] = -1;
		print_vec (denvec_all, L);   printf ("\n");

	// order denominators
	int* denvec_new = simple_den (denvec_all);
	L = length_vec (denvec_new);
		print_vec_rev (denvec_new, L);   printf ("\n");


	// calculate result
	struct mixedNUMBER result;
	result.pfrac.circle = -1;
	result.pfrac.length = L;
	result.pfrac.num = calloc (L, sizeof(int));
	result.pfrac.den = denvec_new;

	int m = N;
	for (int k = L-1; k >= 0; k--) {
		result.pfrac.num[k] = m % result.pfrac.den[k];
		m /= result.pfrac.den[k];
	}
	result.pint = m;


	// free memory
	free (denvec1);
	free (denvec2);
	free (denvec_all);

	return result;
}


int main (void) {
	setbuf (stdout, NULL); // for buggy output


	// initialize variables
	char s1 [MAX_STR], s2 [MAX_STR];
	struct MIXNUM mix1, mix2;

	fgets (s1, MAX_STR, stdin);   s1 [strcspn (s1, "\n")] = ' ';  // for buggy input
	fgets (s2, MAX_STR, stdin);   s2 [strcspn (s2, "\n")] = ' ';  // for buggy input
	mix1 = init_MIXNUM (s1);
	mix2 = init_MIXNUM (s2);
		printf ("\n----------\n\n");

	// call functions and output
	struct mixedNUMBER result;
	result = mult (mix1, mix2);
		printf ("\n");   print_mixedNUMBER (result);


	// free memory
	free (result.pfrac.num);
	free (result.pfrac.den);

	fflush (stdout); // for buggy output

	return 0;
}



// multiplicate two mixed numbers

// INPUT: 2 strings that represents mixed numbers with one simple fraction
// OUTPUT: the steps of the operation [incomplete, missing calculations]
//         1 mixed number, result of the operation



// examples

// 1/2|11 x 1/3|22 = 5/6|256

// 1/2|12 x 3/5|23 = 295

// 2/3|13 x 5/7|24 = 1,5/3,7|337

// 1/4|16 x 2/5|27 = 0,1/5,4|445
//                 = 1/4|445

// 3/8|18 x 4/9|24 = 4,1/8,9|449
//                 = 1/6|449