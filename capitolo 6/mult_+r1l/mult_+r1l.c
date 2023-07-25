#include "../../mylib.h"


struct MIXNUM { // a mixed number
	int pint; // integer part
	struct multFRACTION pfrac; // fractional part (one multiple fraction)
};

struct MIXNUM init_MIXNUM (char s[]) {
	// WARNING: this function does not sanitize input
	// it expects a string of the following pattern:
	//   mixed_number + "|" + number
	//   where mixed_number is a string with one slash "/" and a balanced number of commas around it alternated with numbers (no circle 'o')
	// whitespaces are ignored

	// prepare the string and find separators
	s = trim (s);
	int L = length_str (s);

	int commas = 0, slash = -1, pipe = L;

	for (int k = 0; s[k] != '\0'; k++) {
		switch (s[k]) {
			case ',': {
				commas++;
			} break;
			case '/': {
				slash = k;
			} break;
			case '|': {
				pipe = k;
			} break;
		}
	}
	commas /= 2;


	// intialize result
	struct MIXNUM mix;

	mix.pfrac.circle = -1;

	int l = commas + 1;
	mix.pfrac.length = l;

	int* num = calloc (l, sizeof(int));
	int* den = calloc (l, sizeof(int));


	// extract numbers from the string
	int start = -1, end = length_str (s);

	int i = start, c = 0; // i = counter for substring beginning, c = counter for array position
	for (int k = start+1; k <= slash; k++) {
		if ((s[k] == ',') || (k == slash)) { // k = counter for substring end
			num [(l-1)-c] = str_to_int (substr (s, i+1, k-1));
			i = k;
			c++;
		}
	}
	mix.pfrac.num = num;

	c = 0;
	for (int k = slash+1; k <= pipe; k++) {
		if ((s[k] == ',') || (k == pipe)) {
			den [(l-1)-c] = str_to_int (substr (s, i+1, k-1));
			i = k;
			c++;
		}
	}
	mix.pfrac.den = den;

	mix.pint = str_to_int (substr (s, pipe+1, L-1));


	return mix;
}


int numTOT (struct MIXNUM mix) {
	int l = mix.pfrac.length;
	int N = mix.pint;

	for (int k = 0; k < l; k++) {
		N *= mix.pfrac.den[k];
		N += mix.pfrac.num[k];
	}

	return N;
}

struct mixedNUMBER mult (struct MIXNUM mix1, struct MIXNUM mix2) {
	// calculate numerator
	int N1 = numTOT (mix1),	    N2 = numTOT (mix2);
	int N = N1 * N2;
		printf ("N1 = %d \t N2 = %d\n", N1, N2);
		printf ("N = %d\n\n", N);


	// calculate denominator
	int** denvec1 = (int**) calloc (mix1.pfrac.length, sizeof(int*)),
	   ** denvec2 = (int**) calloc (mix2.pfrac.length, sizeof(int*));
	for (int i = 0; i < mix1.pfrac.length; i++) {
		denvec1[i] = factorize (mix1.pfrac.den[i]);
	}
	for (int i = 0; i < mix2.pfrac.length; i++) {
		denvec2[i] = factorize (mix2.pfrac.den[i]);
	}

	int* l1 = (int*) calloc (mix1.pfrac.length, sizeof(int)),
	   * l2 = (int*) calloc (mix2.pfrac.length, sizeof(int));
	for (int i = 0; i < mix1.pfrac.length; i++) {
		l1[i] = length_vec (denvec1[i]);
	}
	for (int i = 0; i < mix2.pfrac.length; i++) {
		l2[i] = length_vec (denvec2[i]);
	}

	int L = 0;
	for (int i = 0; i < mix1.pfrac.length; i++) {
		L += l1[i];
	}
	for (int i = 0; i < mix2.pfrac.length; i++) {
		L += l2[i];
	}

	// merge denominators in one array
	int* denvec_all = calloc (L+1, sizeof(int));
	int c = 0; // counter for denvec_all
	for (int i = 0; i < mix1.pfrac.length; i++) {
		for (int k = 0; k < l1[i]; k++) {
			denvec_all[c++] = denvec1[i][k];
		}
	}
	for (int i = 0; i < mix2.pfrac.length; i++) {
		for (int k = 0; k < l2[i]; k++) {
			denvec_all[c++] = denvec2[i][k];
		}
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
	for (int i = 0; i < mix1.pfrac.length; i++) {
		free (denvec1[i]);
	}
	free (denvec1);
	for (int i = 0; i < mix2.pfrac.length; i++) {
		free (denvec2[i]);
	}
	free (denvec2);
	free (denvec_all);

	return result;
}


int main (void) {
	setbuf (stdout, NULL); // for buggy output


	// initialize variables
	char s1 [MAX_STR], s2 [MAX_STR];
	struct MIXNUM mix1, mix2;

	fgets (s1, MAX_STR, stdin);   s1 [strcspn(s1, "\n")] = ' ';  // for buggy input
	fgets (s2, MAX_STR, stdin);   s2 [strcspn(s2, "\n")] = ' ';  // for buggy input
	mix1 = init_MIXNUM (s1);
	mix2 = init_MIXNUM (s2);
		printf ("\n----------\n\n");

	// call functions and output
	struct mixedNUMBER result;
	result = mult (mix1, mix2);
		printf ("\n");	print_mixedNUMBER (result);


	// free memory
	free (mix1.pfrac.num);
	free (mix1.pfrac.den);
	free (mix2.pfrac.num);
	free (mix2.pfrac.den);
	free (result.pfrac.num);
	free (result.pfrac.den);

	fflush (stdout); // for buggy output

	return 0;
}



// multiplicate two mixed numbers

// INPUT: 2 strings that represent mixed numbers with a multiple fraction
// OUTPUT: the steps of the operation [incomplete, missing calculations]
//         1 mixed number, result of the operation



// examples


// 1,3/2,8|13 x 3,2/4,9|24 = 5,3,5/8,8,9|326

// 1,3,3/2,8,11|14 x 1,2,4/3,9,13|25 = 2,5,4,6/3,8,9,13|362
//                                  (= 2,6,6,5,6/6,8,9,11,13|362)