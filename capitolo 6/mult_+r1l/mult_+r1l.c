#include "../../../mylib.h"


struct MIXNUM { // a mixed number
	int pint; // integer part
	struct multFRACTION pfrac; // fractional part
};

struct MIXNUM init_MIXNUM (char s[]) {
	// this function does not sanitize input:
	// it expects a string of the following pattern:
	//   mixed_number + "|" + number
	//   where mixed_number is a string with one slash and a balanced number of commas alternated with numbers
	// whitespaces are ignored

	s = trim (s);

	int commas = 0, slash = -1, pipe = -1;

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


	struct MIXNUM mix;
	
	mix.pfrac.circle = -1;
	
	int l = commas + 1;
	mix.pfrac.length = l;
	
	int* num = calloc (l, sizeof(int));
	int* den = calloc (l, sizeof(int));
	

	int start = -1, end = length_str (s);
	
	int i = start, c = 0; // i = counter for substring beginning, c = counter for array position
	for (int k = start+1; k <= slash; k++) {
		if ((s[k] == ',') || (k == slash)) {
			num[(l-1)-c] = str_to_int (substr (s, i+1, k-1));
			i = k;
			c++;
		}
	}
	mix.pfrac.num = num;

	c = 0;
	for (int k = slash+1; k <= pipe; k++) {
		if ((s[k] == ',') || (k == pipe)) {
			den[(l-1)-c] = str_to_int (substr (s, i+1, k-1));
			i = k;
			c++;
		}
	}
	mix.pfrac.den = den;

	mix.pint = str_to_int (substr (s, pipe+1, end-1));


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
	// initialize variables
	int l1 = mix1.pfrac.length, l2 = mix2.pfrac.length;
	
	int N1 = numTOT (mix1), N2 = numTOT (mix2);
	int N = N1 * N2;
	
	int M = 1;
	for (int k = 0; k < l1; k++) {
		M *= mix1.pfrac.den[k];
	}
	for (int k = 0; k < l2; k++) {
		M *= mix2.pfrac.den[k];
	}
	
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



// examples

// 1,3/2,8|13 x 3,2/4,9|24 = 5,3,5/8,8,9|326

// 1,3,3/2,8,11|14 x 1,2,4/3,9,13|25 = 2,5,4,6/3,8,9,13|362
//                                  (= 2,6,6,5,6/6,8,9,11,13|362)
