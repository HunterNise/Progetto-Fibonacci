#include "../../mylib.h"


struct MIXNUM { // a mixed number
	int pint; // integer part
	struct multFRACTION* pfrac; // fractional part (an arbitrary number of multiple fractions)
	int mFN; // number of multiple fractions
};

struct MIXNUM init_MIXNUM (char s[]) {
	// WARNING: this function does not sanitize input
	// it expects a string of the following pattern:
	//   mixed_number + ";" ... + ";" + mixed_number "|" + number
	//   where mixed_number is a string with one slash "/" and a balanced number of commas around it alternated with numbers (no circle 'o')
	// whitespaces are ignored

	// prepare the string and find separators
	s = trim (s);
	int L = length_str (s);

	int mFN = 1; // number of multFRACTION

	for (int k = 0; s[k] != '\0'; k++) { // count the number of multiple fractions
		if (s[k] == ';') {
			mFN++;
		}
	}

	int* commas 	= calloc (mFN, sizeof(int)),	   * slash 		= calloc (mFN, sizeof(int)),	   * semicolon 	= calloc (mFN, sizeof(int));
	int pipe = L;

	for (int j = 0; j < mFN; j++) {
		slash[j] = -1;
		semicolon[j] = -1;
	}

	for (int k = 0, j = 0; s[k] != '\0'; k++) {
		switch (s[k]) {
			case ',': {
				commas[j]++;
				break;
			}
			case '/': {
				slash[j] = k;
				break;
			}
			case ';': {
				semicolon[j+1] = k;
				j++;
				break;
			}
			case '|': {
				pipe = k;
				break;
			}

		}
	}
	for (int j = 0; j < mFN; j++) {
		commas[j] /= 2;
	}


	// intialize result
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

	// extract numbers from the string
	for (int j = 0; j < mFN; j++) {
		int* num = calloc (l[j], sizeof(int)),		   * den = calloc (l[j], sizeof(int));

		int start = semicolon[j],		    end = (j == mFN-1) ? pipe : semicolon[j+1]; // check if it's the last multFRACTION

		int i = start, c = 0; // i = counter for substring beginning, c = counter for array position
		for (int k = start+1; k <= slash[j]; k++) {
			if ((s[k] == ',') || (k == slash[j])) { // k = counter for substring end
				num [(l[j]-1)-c] = str_to_int (substr (s, i+1, k-1));
				i = k;
				c++;
			}
		}
		mix.pfrac[(mFN-1)-j].num = num;

		c = 0;
		for (int k = slash[j]+1; k <= end; k++) {
			if ((s[k] == ',') || (k == end)) {
				den [(l[j]-1)-c] = str_to_int (substr (s, i+1, k-1));
				i = k;
				c++;
			}
		}
		mix.pfrac[(mFN-1)-j].den = den;
	}

	mix.pint = str_to_int (substr (s, pipe+1, L-1));


	// free memory
	free (commas);
	free (slash);
	free (semicolon);
	free (l);

	return mix;
}


int numTOT (struct MIXNUM mix) {
	int N = mix.pint;

	for (int k = 0; k < mix.pfrac[0].length; k++) {
		N *= mix.pfrac[0].den[k];
		N += mix.pfrac[0].num[k];
	}
	for (int j = 1; j < mix.mFN; j++) {
		for (int k = 0; k < mix.pfrac[j].length; k++) {
			N *= mix.pfrac[j].den[k];
		}
	}

	for (int j = 1; j < mix.mFN; j++) {
		int s = mix.pfrac[j].num[0];
		for (int k = 1; k < mix.pfrac[j].length; k++) {
			s *= mix.pfrac[j].den[k];
			s += mix.pfrac[j].num[k];
		}
		for (int i = 0; i < mix.mFN; i++) {
			if (i != j) {
				for (int k = 0; k < mix.pfrac[i].length; k++) {
					s *= mix.pfrac[i].den[k];
				}
			}
		}
		N += s;
	}

	return N;
}

struct mixedNUMBER mult (struct MIXNUM mix1, struct MIXNUM mix2) {
	// calculate numerator
	int N1 = numTOT (mix1),	    N2 = numTOT (mix2);
	llu N = (llu) N1 * (llu) N2; // for overflow
		printf ("N1 = %d \t N2 = %d\n", N1, N2);
		printf ("N = %llu\n\n", N);


	// calculate denominator
	int*** denvec1 = (int***) calloc (mix1.mFN, sizeof(int**)),	   *** denvec2 = (int***) calloc (mix2.mFN, sizeof(int**));
	for (int j = 0; j < mix1.mFN; j++) {
		denvec1[j] = (int**) calloc (mix1.pfrac[j].length, sizeof(int*));
		for (int i = 0; i < mix1.pfrac[j].length; i++) {
			denvec1[j][i] = factorize (mix1.pfrac[j].den[i]);
		}
	}
	for (int j = 0; j < mix2.mFN; j++) {
		denvec2[j] = (int**) calloc (mix2.pfrac[j].length, sizeof(int*));
		for (int i = 0; i < mix2.pfrac[j].length; i++) {
			denvec2[j][i] = factorize (mix2.pfrac[j].den[i]);
		}
	}

	int** l1 = (int**) calloc (mix1.mFN, sizeof(int*)),	   ** l2 = (int**) calloc (mix2.mFN, sizeof(int*));
	for (int j = 0; j < mix1.mFN; j++) {
		l1[j] = (int*) calloc (mix1.pfrac[j].length, sizeof(int));
		for (int i = 0; i < mix1.pfrac[j].length; i++) {
			l1[j][i] = length_vec (denvec1[j][i]);
		}
	}
	for (int j = 0; j < mix2.mFN; j++) {
		l2[j] = (int*) calloc (mix2.pfrac[j].length, sizeof(int));
		for (int i = 0; i < mix2.pfrac[j].length; i++) {
			l2[j][i] = length_vec (denvec2[j][i]);
		}
	}

	int L = 0;
	for (int j = 0; j < mix1.mFN; j++) {
		for (int i = 0; i < mix1.pfrac[j].length; i++) {
			L += l1[j][i];
		}
	}
	for (int j = 0; j < mix2.mFN; j++) {
		for (int i = 0; i < mix2.pfrac[j].length; i++) {
			L += l2[j][i];
		}
	}

	// merge denominators in one array
	int* denvec_all = calloc (L+1, sizeof(int));
	int c = 0; // counter for denvec_all
	for (int j = 0; j < mix1.mFN; j++) {
		for (int i = 0; i < mix1.pfrac[j].length; i++) {
			for (int k = 0; k < l1[j][i]; k++) {
				denvec_all[c++] = denvec1[j][i][k];
			}
		}
	}
	for (int j = 0; j < mix2.mFN; j++) {
		for (int i = 0; i < mix2.pfrac[j].length; i++) {
			for (int k = 0; k < l2[j][i]; k++) {
				denvec_all[c++] = denvec2[j][i][k];
			}
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

	llu m = N; // for overflow
	for (int k = L-1; k >= 0; k--) {
		result.pfrac.num[k] = (int) (m % (llu) result.pfrac.den[k]);
		m /= (llu) result.pfrac.den[k];
	}
	result.pint = (int) m;


	// free memory
	for (int j = 0; j < mix1.mFN; j++) {
		for (int i = 0; i < mix1.pfrac[j].length; i++) {
			free (denvec1[j][i]);
		}
		free (denvec1[j]);
		free (l1[j]);
	}
	free (denvec1);
	free (l1);
	for (int j = 0; j < mix2.mFN; j++) {
		for (int i = 0; i < mix2.pfrac[j].length; i++) {
			free (denvec2[j][i]);
		}
		free (denvec2[j]);
		free (l2[j]);
	}
	free (denvec2);
	free (l2);
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
	for (int j = 0; j < mix1.mFN; j++) {
		free (mix1.pfrac[j].num);
		free (mix1.pfrac[j].den);
	}
	free (mix1.pfrac);
	for (int j = 0; j < mix2.mFN; j++) {
		free (mix2.pfrac[j].num);
		free (mix2.pfrac[j].den);
	}
	free (mix2.pfrac);
	free (result.pfrac.num);
	free (result.pfrac.den);

	fflush (stdout); // for buggy output

	return 0;
}



// multiplicate two mixed numbers

// INPUT: 2 strings that represent mixed numbers with an arbitrary number of multiple fractions
// OUTPUT: the steps of the operation [incomplete, missing calculations]
//         1 mixed number, result of the operation

// WARNING: due to the algorithm implementation it is easy to run in overflow if the fractions have too many denominators (and/or too big numbers)



// examples


// 1/4;1/3|15 x 1/6;1/5|26 = 1,7,8/4,9,10|410

// 2/9;3/5|16 x 2/11;5/8|27 = 3,8,4,8/4,9,10,11|467

// 1/9;1/8|17 x 1/17;1/3|28 = 1,3/3,9|489
//                         (= 1,5,2,6/3,8,9,17|489)


// 1,2/5,9;1,5/2,8|17 x 2,1/5,5;3,4/8,11|28 = 1,6,4,1,2,7,2/2,8,9,10,10,10,11|514

// 1,5,1/2,6,8;2,2,3/7,9,10|19 x 1,2,1/3,5,7;5,3,3/6,8,9|23 = 1,5,6,3,4,2,1,5,6/2,6,7,7,8,8,9,9,10|461
//                                                         (= 0,3,6,1,3,7,1,4,6,5,6/2,6,7,7,8,8,9,9,9,10,10|461)


// 1/5;1/4;1/3|21 x 1/8;2/9;3/7|32 = 5,3,7,5,9/6,7,8,9,10|713

// 2,1/5,5;1,2/8,9;2,2/3,7|23 x 1,5/2,17;2,3/5,10;1,5/4,13|32 = 1,0,2,3,1,1,1,8,3/2,7,9,10,10,10,10,13,17|790
//                                                           (= 0,4,0,2,3,1,1,1,8,3/3,7,8,9,10,10,10,10,13,17|790)

// 1,2,1/3,5,5;1,2,3/2,9,10;1,1,6/2,7,17|11 x 2,5,1/3,6,7;1,2,2/5,7,9;1,3,3/2,8,10|22 = 1,2,1,0,1,3,9,5,0,4/2,7,7,8,9,9,10,10,10,17|274
//                                                                                   (= 0,0,3,3,6,3,3,5,4,3,9,5,0,4/6,7,7,7,8,9,9,9,10,10,10,10,10,17|274)


// 1/3 x 1/4 = 1,0/2,6
//           = 1/12

// 2/3 x 3/4 = 1/2

// 3/7 x 4/9 = 4/21
//          (= 5,1/7,9)

// 1,4/2,7 x 2,3/3,5 = 5,4/7,10

// 1,5,3/2,8,11 x 1,4,7/3,9,13 = 2,2,5,5,2/6,8,9,11,13

// 1/4;2/3 x 1/6;3/5 = 1,0,7/4,9,10

// 1,3/4,8;1,4/2,7 x 1,2/6,11;1,5/3,9 = 2,0,0,1,9/6,8,8,9,11
//                                   (= 0,0,3,0,0,1,9/2,7,8,8,9,9,11)

// 1/3;1/4;1/5 x 2/5;1/6;1/7 = 1,1,5,5,5/2,7,9,10,10

// 1,6/2,11;2,3/3,10;3,2/4,9 x 1,7/2,13;1,3/3,7;4,1/5,8 = 2,1,4,2,6,8,10,7/3,7,8,9,10,10,11,13|1
//                                                     (= 1,1,6,4,2,6,8,10,7/7,8,8,9,9,10,10,11,13|1)