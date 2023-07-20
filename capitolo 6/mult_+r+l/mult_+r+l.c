#include "../../mylib.h"


struct MIXNUM { // a mixed number
	int pint; // integer part
	struct multFRACTION* pfrac; // fractional part
	int mFN;
};

struct MIXNUM init_MIXNUM (char s[]) {
	// this function does not sanitize input:
	// it expects a string of the following pattern:
	//   mixed_number + ";" ... + ";" + mixed_number "|" + number
	//   where mixed_number is a string with one slash and a balanced number of commas alternated with numbers
	// whitespaces are ignored

	s = trim (s);
	
	int mFN = 1; // number of multFRACTION
	
	for (int k = 0; s[k] != '\0'; k++) { // count the number of multiple fractions
		switch (s[k]) {
			case ';': {
				mFN++;
			} break;
		}
	}
	/* printf ("mFN = %d\n", mFN); */

	int* commas = calloc (mFN, sizeof(int));
	int* slash = calloc (mFN, sizeof(int));
	int* semicolon = calloc (mFN, sizeof(int));
	int pipe = -1;
	
	for (int k = 0; k < mFN; k++) {
		slash[k] = -1;
	}
	for (int k = 0; k < mFN; k++) {
		semicolon[k] = -1;
	}

	for (int k = 0, j = 0; s[k] != '\0'; k++) {
		switch (s[k]) {
			case ',': {
				commas[j]++;
			} break;
			case '/': {
				slash[j] = k;
			} break;
			case ';': {
				semicolon[j+1] = k;
				j++;
			} break;
			case '|': {
				pipe = k;
			} break;
		}
	}
	for (int k = 0; k < mFN; k++) {
		commas[k] /= 2;
	}
	/* printf ("commas = "); print_vec (commas, mFN); printf ("\n");
	printf ("slash = "); print_vec (slash, mFN); printf ("\n");
	printf ("semicolon = "); print_vec (semicolon, mFN); printf ("\n");
	printf ("pipe = %d\n", pipe); */


	struct MIXNUM mix;
	mix.mFN = mFN;
	mix.pfrac = calloc (mFN, sizeof(struct multFRACTION));
	
	for (int j = 0; j < mFN; j++) {
		mix.pfrac[j].circle = -1;
	}
	
	int* l = calloc (mFN, sizeof(int));
	for (int j = 0; j < mFN; j++) {
		l[j] = commas[j] + 1;
		mix.pfrac[(mFN-1)-j].length = l[j];
	}
	

	for (int j = 0; j < mFN; j++) {
		int* num = calloc (l[j], sizeof(int));
		int* den = calloc (l[j], sizeof(int));
		
		int start = semicolon[j];
		int end = (j == mFN-1) ? pipe : semicolon[j+1]; // check if it's the last multFRACTION
		/* printf ("j = %d, start = %d, end = %d\n", j, start, end); */
		
		int i = start, c = 0;
		for (int k = start; k <= slash[j]; k++) {
			if ((s[k] == ',') || (k == slash[j])) {
				num[(l[j]-1)-c] = str_to_int (substr (s, i+1, k-1));
				i = k;
				c++;
			}
		}
		mix.pfrac[(mFN-1)-j].num = num;

		c = 0;
		for (int k = slash[j]+1; k <= end; k++) {
			if ((s[k] == ',') || (k == end)) {
				den[(l[j]-1)-c] = str_to_int (substr (s, i+1, k-1));
				i = k;
				c++;
			}
		}
		mix.pfrac[(mFN-1)-j].den = den;
	}
	
	int end = length_str (s);
	mix.pint = str_to_int (substr (s, pipe+1, end-1));


	return mix;
}


int numTOT (struct MIXNUM mix) {
	int N = mix.pint;
	/* printf ("%d, ", N); */
	
	for (int k = 0; k < mix.pfrac[0].length; k++) {
		N *= mix.pfrac[0].den[k];
		N += mix.pfrac[0].num[k];
		/* printf ("%d %d, ", mix.pfrac[0].den[k], mix.pfrac[0].num[k]); */
	}
	/* printf (" // %d->", N); */
	for (int j = 1; j < mix.mFN; j++) {
		for (int k = 0; k < mix.pfrac[j].length; k++) {
			N *= mix.pfrac[j].den[k];
		}
	}
	/* printf ("%d ", N); */
	
	for (int j = 1; j < mix.mFN; j++) {
		int s = mix.pfrac[j].num[0];
		for (int k = 1; k < mix.pfrac[j].length; k++) {
			s *= mix.pfrac[j].den[k];
			s += mix.pfrac[j].num[k];
		}
		/* printf ("%d->", s); */
		for (int i = 0; i < mix.mFN; i++) {
			if (i != j) {
				for (int k = 0; k < mix.pfrac[i].length; k++) {
					s *= mix.pfrac[i].den[k];
				}	
			}
		}
		/* printf ("%d ", s); */
		N += s;
	}
	/* printf ("\n"); */
	
	return N;
}

struct mixedNUMBER mult (struct MIXNUM mix1, struct MIXNUM mix2) {
	// initialize variables
	int N1 = numTOT (mix1), N2 = numTOT (mix2);
	int N = N1 * N2;
	/* printf ("%d %d\n", N1, N2); */
	
	int M = 1;
	/* printf ("\nmFN = %d", mix1.mFN); */
	for (int j = 0; j < mix1.mFN; j++) {
		/* printf ("\nl = %d, ", mix1.pfrac[j].length); */
		for (int k = 0; k < mix1.pfrac[j].length; k++) {
			M *= mix1.pfrac[j].den[k];
			/* printf ("%d ", mix1.pfrac[j].den[k]); */
		}
	}
	/* printf ("\nmFN = %d", mix2.mFN); */
	for (int j = 0; j < mix2.mFN; j++) {
		/* printf ("\nl = %d, ", mix2.pfrac[j].length); */
		for (int k = 0; k < mix2.pfrac[j].length; k++) {
			M *= mix2.pfrac[j].den[k];
			/* printf ("%d ", mix2.pfrac[j].den[k]); */
		}
	}
	/* printf ("\nM = %d\n", M); */
	
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
	
	/* printf ("\n");
	for (int j = mix1.mFN-1; j >= 0; j--) {
		print_multFRACTION (mix1.pfrac[j]);
		printf (" ");
	}
	printf ("%d\n", mix1.pint);
	for (int j = mix2.mFN-1; j >= 0; j--) {
		print_multFRACTION (mix2.pfrac[j]);
		printf (" ");
	}
	printf ("%d\n", mix2.pint);
	printf ("\n"); */
	
	struct mixedNUMBER result = mult (mix1, mix2);
	print_mixedNUMBER (result);


	for (int j = 0; j < mix1.mFN; j++) {
		free (mix1.pfrac[j].num);
		free (mix1.pfrac[j].den);
	}
	for (int j = 0; j < mix1.mFN; j++) {
		free (mix1.pfrac[j].num);
		free (mix1.pfrac[j].den);
	}
	free (result.pfrac.num);
	free (result.pfrac.den);
	fflush (stdout); // for buggy output
	return 0;
}



// multiplicate two mixed numbers

// INPUT: 2 strings that represent mixed numbers with an arbitrary number of multiple fractions
// OUTPUT: 1 mixed number, result of the multiplication

// WARNING: due to the algorithm implementation it is easy to run in overflow if the fractions have too many denominators (and/or too big)



// examples


// 1/4;1/3|15 x 1/6;1/5|26 = 1,7,8/4,9,10|410

// 2/9;3/5|16 x 2/11;5/8|27 = 3,8,4,8/4,9,10,11|467

// 1/9;1/8|17 x 1/17;1/3|28 = 1,3/3,9|489


// 1,2/5,9;1,5/2,8|17 x 2,1/5,5;3,4/8,11|28 = 1,6,4,1,2,7,2/2,8,9,10,10,10,11|514

// 1,5,1/2,6,8;2,2,3/7,9,10|19 x 1,2,1/3,5,7;5,3,3/6,8,9|23 = 1,5,6,3,4,2,1,5,6/2,6,7,7,8,8,9,9,10|461


// 1/5;1/4;1/3|21 x 1/8;2/9;3/7|32 = 5,3,7,5,9/6,7,8,9,10|713

// 2,1/5,5;1,2/8,9;2,2/3,7|23 x 1,5/2,17;2,3/5,10;1,5/4,13|32 = 1,0,2,3,1,1,1,8,3/2,7,9,10,10,10,10,13,17|790

// 1,2,1/3,5,5;1,2,3/2,9,10;1,1,6/2,7,17|11 x 2,5,1/3,6,7;1,2,2/5,7,9;1,3,3/2,8,10|22 = 1,2,1,0,1,3,9,5,0,4/2,7,7,8,9,9,10,10,10,17|274


// 1/3|0 x 1/4|0 = 1,0/2,6|0
//               = 1/12|0

// 2/3|0 x 3/4|0 = 1/2|0

// 3/7|0 x 4/9|0 = 4/21|0

// 1,4/2,7|0 x 2,3/3,5|0 = 5,4/7,10|0

// 1,5,3/2,8,11|0 x 1,4,7/3,9,13|0 = 2,2,5,5,2/6,8,9,11,13|0

// 1/4;2/3|0 x 1/6;3/5|0 = 1,0,7/4,9,10|0

// 1,3/4,8;1,4/2,7|0 x 1,2/6,11;1,5/3,9|0 = 2,0,0,1,9/6,8,8,9,11|0

// 1/3;1/4;1/5|0 x 2/5;1/6;1/7|0 = 1,1,5,5,5/2,7,9,10,10|0

// 1,6/2,11;2,3/3,10;3,2/4,9|0 x 1,7/2,13;1,3/3,7;4,1/5,8|0 = 2,1,4,2,6,8,10,7/3,7,8,9,10,10,11,13|1