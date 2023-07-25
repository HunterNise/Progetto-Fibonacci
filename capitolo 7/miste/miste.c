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

	int* commas 	= calloc (mFN, sizeof(int)),
	   * slash 		= calloc (mFN, sizeof(int)),
	   * semicolon 	= calloc (mFN, sizeof(int));
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
		int* num = calloc (l[j], sizeof(int)),
		   * den = calloc (l[j], sizeof(int));

		int start = semicolon[j],
		    end = (j == mFN-1) ? pipe : semicolon[j+1]; // check if it's the last multFRACTION

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

void print_MIXNUM (struct MIXNUM mix) { // output the mixed number
	for (int j = mix.mFN-1; j >= 0; j--) {
		print_multFRACTION (mix.pfrac[j]);
		printf ( (j != 0) ? ";" : "|"); // don't print ';' after the last number
	}
	printf ("%d", mix.pint);
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


void mask (struct MIXNUM mix1, struct MIXNUM mix2, char s) { // output "mix1 ? mix2 = ", where ? can be '+', '-' or '/'
	printf ("\n");
	print_MIXNUM (mix1);   printf (" %c ", s);   print_MIXNUM (mix2);   printf (" = ");
}

void op (struct MIXNUM mix1, struct MIXNUM mix2) { // calculate the operations
	// calculate relevant quantities
	int m = 1;
	for (int j = mix1.mFN-1; j >= 0; j--) {
		for (int k = 0; k < mix1.pfrac[j].length; k++) {
			m = mcm (m, mix1.pfrac[j].den[k]);
		}
	}
	for (int j = mix2.mFN-1; j >= 0; j--) {
		for (int k = 0; k < mix2.pfrac[j].length; k++) {
			m = mcm (m, mix2.pfrac[j].den[k]);
		}
	}
		printf ("m = %d\n", m);

	int m1 = 1, m2 = 1;
	for (int j = mix1.mFN-1; j >= 0; j--) {
		for (int k = 0; k < mix1.pfrac[j].length; k++) {
			m1 *= mix1.pfrac[j].den[k];
		}
	}
	for (int j = mix2.mFN-1; j >= 0; j--) {
		for (int k = 0; k < mix2.pfrac[j].length; k++) {
			m2 *= mix2.pfrac[j].den[k];
		}
	}
		printf ("m1 = %d \t m2 = %d\n", m1, m2);

	int N1 = numTOT (mix1) * (m / m1),	    N2 = numTOT (mix2) * (m / m2);
			printf ("N1 = %d \t N2 = %d\n", N1, N2);


	struct mixedNUMBER result;

	// addition
	result = mix_div (N1 + N2, m);
		mask (mix1, mix2, '+');			print_mixedNUMBER (result);

	// subtraction
	if (N1 >= N2) { // negative numbers are not contemplated
		result = mix_div (N1 - N2, m);
			mask (mix1, mix2, '-');		print_mixedNUMBER (result);
	}
	else {
		result = mix_div (N2 - N1, m);
			mask (mix2, mix1, '-');		print_mixedNUMBER (result);
	}

	// division
	result = mix_div (N1, N2);
		mask (mix1, mix2, '/');			print_mixedNUMBER (result);

	result = mix_div (N2, N1);
		mask (mix2, mix1, '/');			print_mixedNUMBER (result);
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
	op (mix1, mix2);


	// free memory
	for (int j = 0; j < mix1.mFN; j++) {
		free (mix1.pfrac[j].num);
		free (mix1.pfrac[j].den);
	}
	for (int j = 0; j < mix2.mFN; j++) {
		free (mix2.pfrac[j].num);
		free (mix2.pfrac[j].den);
	}

	fflush (stdout); // for buggy output

	return 0;
}



// add, subtract and divide two mixed numbers

// INPUT: 2 strings that represent mixed numbers with an arbitrary number of multiple fractions
// OUTPUT: the steps of the operation [incomplete, missing calculations]
//         the results of the operations



// examples

// 1/4;1/3 + 1/7;1/5 = 389/420
//                   = 5,1,9/6,7,10

// 1/4;1/3 - 1/7;1/5 = 5,2,2/6,7,10

// 1/4;1/3 / 1/7;1/5 = 1,2,6/2,8,9|1

// 1/7;1/5 / 1/4;1/3 = 4,0,4/5,7,7


// 2/7;3/5 + 2/9;3/8 = 1,3,7,4/4,7,9,10|1

// 2/7;3/5 - 2/9;3/8 = 3,6,7,2/4,7,9,10


// 1/4;1/3 + 1/6;1/5 = 19/20
//                   = 1,9/2,10
//                  (= 3,9/6,10)

// 1/4;1/3 - 1/6;1/5 = 1,2/6,10


// 1/7;2/3 + 1/9;3/5 = 4,4,4/5,7,9|1

// 1/7;2/3 - 1/9;3/5 (= 1,6,0/5,7,9)


// 1/5;3/4 + 1/10;2/9 = 1,6,2/2,9,10|1

// 1/5;3/4 - 1/10;2/9 = 1,2,6/2,9,10

// --------------------------------------------------

// 1/3|12 + 3/4|126 = 1/12|139
//                 (= 1,0/2,6|139)

// 3/4|126 - 1/3|12 = 5/12|114

// 3/4|126 / 1/3|12 =

// 1/3|12 / 3/4|126 = 4,3,1/9,13,13


// 3/4|13 + 2/5|171 = 1,1/2,10|185

// 2/5|171 - 3/4|13 = 1,6/2,10|157
//                  = 1/4;2/5|157

// 2/5|171 / 3/4|13 = 3,0,5/5,5,11|12

// 3/4|13 / 2/5|171 = 3,68/4,857


// 5/6|14 + 2/9|231 = 1,0/2,9|246

// 2/9|231 - 5/6|14 = 1,3/2,9|216

// 2/9|231 / 5/6|14 = 1,52/3,89|15

// 5/6|14 / 2/9|231 = 1,133/2,2081

// --------------------------------------------------

// 1/4;1/3|15 + 1/7;3/5|322 = 5,1,3/6,7,10|338

// 1/7;3/5|322 - 1/4;1/3|15 = 1,4,1/6,7,10|307

// 1/7;3/5|322 / 1/4;1/3|15 = 2,6,0,12/5,7,11,17|20

// 1/4;1/3|15 / 1/7;3/5|322 = 5,2,0,17/6,8,8,353


// 1/5;3/4|16 + 1/9;4/5|442 = 3,7/4,9|459
//                         (= 1,5,8/2,9,10|459)

// 1/9;4/5|442 - 1/5;3/4|16 = 1,5,9/2,9,10|425

// 1/9;4/5|442 / 1/5;3/4|16 = 2,6,14/3,9,113|26

// 1/5;3/4|16 / 1/9;4/5|442 = 3,762/4,19931
//                         (= 3,2,40/4,19,1049)


// 1/6;2/5|17 + 1/10;7/9|523 = 4/9|541
//                          (= 4,4/9,10|541)

// 1/10;7/9|523 - 1/6;2/5|17 = 4,2/5,9|506
//                          (= 1,3/9,10|506)