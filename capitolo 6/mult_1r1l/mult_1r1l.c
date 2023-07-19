#include "../../mylib.h"


struct MIXNUM { // a mixed number
	int pint; // integer part
	struct FRACTION pfrac; // fractional part
};

struct MIXNUM init_MIXNUM (char s[]) {
	// this function does not sanitize input:
	// it expects a string of the following pattern:
	//   number + "/" + number + "|" + number
	// whitespaces are ignored

	s = trim (s);

	int slash = -1, pipe = -1;

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

	int end = length_str (s);

	mix.pfrac.num = str_to_int (substr (s, 0, slash-1));
	mix.pfrac.den = str_to_int (substr (s, slash+1, pipe-1));
	mix.pint      = str_to_int (substr (s, pipe+1, end-1));


	return mix;
}


int numTOT (struct MIXNUM mix) {
	int N = mix.pint;
	N *= mix.pfrac.den;
	N += mix.pfrac.num;
	return N;
}


struct mixedNUMBER mult (struct MIXNUM mix1, struct MIXNUM mix2) {
	// initialize variables
	int N1 = numTOT (mix1), N2 = numTOT (mix2);
	int N = N1 * N2;
	
	int M = mix1.pfrac.den * mix2.pfrac.den;
	
	int* denvec = simple_den (M); // array of denominators
	int l = 0;
	while (denvec[l] != -1) {
		l++;
	}

	// calculate result
	struct mixedNUMBER result;
	result.pfrac.circle = -1;
	result.pfrac.length = l;
	result.pfrac.num = calloc (l, sizeof(int));
	result.pfrac.den = denvec;

	for (int k = l-1; k >= 0; k--) {
		result.pfrac.num[k] = N % result.pfrac.den[k];
		N /= result.pfrac.den[k];
	}
	result.pint = N;

	return result;
}


int main (void) {
	setbuf (stdout, NULL); // for buggy output
	
	
	char s1[100], s2[100];
	struct MIXNUM mix1, mix2;

	gets (s1);
	gets (s2);
	mix1 = init_MIXNUM (s1);
	mix2 = init_MIXNUM (s2);

	struct mixedNUMBER result = mult (mix1, mix2);
	print_mixedNUMBER (result);


	free (result.pfrac.num);
	free (result.pfrac.den);
	fflush (stdout); // for buggy output
	return 0;
}



// multiplicate two mixed numbers

// INPUT: 2 strings that represent mixed numbers with a simple fraction
// OUTPUT: 1 mixed number, result of the multiplication



// examples

// 1/2|11 x 1/3|22 = 5/6|256

// 1/2|12 x 3/5|23 = 295

// 2/3|13 x 5/7|24 = 1,5/3,7|337

// 1/4|16 x 2/5|27 = 0,1/5,4|445
//                 = 1/4|445

// 3/8|18 x 4/9|24 = 4,1/8,9|449
//                 = 1/6|449