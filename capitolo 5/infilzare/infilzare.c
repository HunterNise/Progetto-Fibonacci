#include "../../mylib.h"


// for overflow
void swap_llu (llu* ptx_a, llu* ptx_b) { // swap two numbers
	llu t = *ptx_a;
	*ptx_a = *ptx_b;
	*ptx_b = t;
}

llu MCD_llu (llu a, llu b) { // calculate MCD with Euclid's algorithm
	if (b > a) {
		swap_llu (&a, &b);
	}

	llu r = a % b;
	while (r != 0) {
		a = b;
		b = r;
		r = a % b;
	}
	return b; // last non 0 remainder
}


struct multFRACTION init (char s[]) {
	// WARNING: this function does not sanitize input
	// it expects a string with one slash '/' and a balanced number of commas around it alternated with numbers
	// the string may contain one circle 'o' at the beginning or at the end
	// whitespaces are ignored

	// prepare the string and find separators
	s = trim (s);
		printf ("trim: ");   print_str (s);   printf ("\n\n");

	int circle = -1, slash = -1, commas = 0;

	for (int k = 0; s[k] != '\0'; k++) {
		switch (s[k]) {
			case 'o': {
				circle = k;
				break;
			}
			case '/': {
				slash = k;
				break;
			}
			case ',': {
				commas += 1;
				break;
			}
		}
	}
	commas /= 2;
		printf ("circle = %d\n", circle);
		printf ("slash = %d\n", slash);
		printf ("commas = %d\n", commas);
		printf ("\n");


	// intialize result
	struct multFRACTION frac;

	frac.circle = circle;

	int l = commas + 1;
	frac.length = l;
		printf ("l = %d\n", l);

	int* num = calloc (l, sizeof(int));
	int* den = calloc (l, sizeof(int));


	// extract numbers from the string
	int start = -1, end = length_str (s);
	if (circle == 0) {
		start += 1;
	}
	else if (circle > 0) {
		end -= 1;
	}
		printf ("start = %d\n", start);
		printf ("end = %d\n", end);
		printf ("\n");

	int i = start, c = 0; // i = counter for substring beginning, c = counter for array position
	for (int k = start+1; k <= slash; k++) {
		if ((s[k] == ',') || (k == slash)) { // k = counter for substring end
			num [(l-1)-c] = str_to_int (substr (s, i+1, k-1));
				printf ("c = %d, i = %d, k = %d  \t", c, i, k);
				printf ("sub: ");	print_str (substr (s, i+1, k-1));
				printf ("   %d\n", num [(l-1)-c]);
			i = k;
			c++;
		}
	}
	frac.num = num;
		printf ("\n");

	c = 0;
	for (int k = slash+1; k <= end; k++) {
		if ((s[k] == ',') || (k == end)) {
			den [(l-1)-c] = str_to_int (substr (s, i+1, k-1));
				printf ("c = %d, i = %d, k = %d  \t", c, i, k);
				printf ("sub: ");	print_str (substr (s, i+1, k-1));
				printf ("   %d\n", den [(l-1)-c]);
			i = k;
			c++;
		}
	}
	frac.den = den;


	return frac;
}

struct FRACTION infilzare (struct multFRACTION frac) { // convert a multiple fractions to a simple one [mult_to_simple in mylib.h]
	struct FRACTION result;
	int l = frac.length;

	llu a, b; // for overflow

	if (frac.circle < 0) { // no circle
		b = 1;
		for (int k = l-1; k >= 0; k--) { // multiply all denominators
			b *= (llu) frac.den[k];
				printf ("%d", frac.den[k]);
				printf ( (k != 0) ? "x" : ""); // don't print after the last number
		}
			printf (" = %llu\n\n", b);

		a = 0;
		for (int k = 0; k < l; k++) {
			llu n = frac.num[k];
				printf ("%llu", n);
				printf ( (k != l-1) ? "x" : ""); // don't print after the last number

			for (int i = k+1; i < l; i++) { // multiply by all denominators that come after (on the left)
				n *= (llu) frac.den[i];
					printf ("%d", frac.den[i]);
					printf ( (i != l-1) ? "x" : ""); // don't print after the last number
			}
				printf (" = %llu\n", n);

			a += n;
		}
	}

	else if (frac.circle == 0) { // circle on the left
		a = 1;
		b = 1;

		for (int k = l-1; k >= 0; k--) { // multiply all numerators
			a *= (llu) frac.num[k];
				printf ("%d", frac.num[k]);
				printf ( (k != 0) ? "x" : ""); // don't print after the last number
		}
			printf (" = %d\n", a);

		for (int k = l-1; k >= 0; k--) { // multiply all denominators
			b *= (llu) frac.den[k];
				printf ("%d", frac.den[k]);
				printf ( (k != 0) ? "x" : ""); // don't print after the last number
		}
			printf (" = %d\n", b);
	}

	else { // circle on the right
		b = 1;
		for (int k = l-1; k >= 0; k--) { // multiply all denominators
			b *= (llu) frac.den[k];
				printf ("%d", frac.den[k]);
				printf ( (k != 0) ? "x" : ""); // don't print after the last number
		}
			printf (" = %d\n\n", b);

		a = 0;
		for (int k = 0; k < l; k++) {
			llu n = 1;

			for (int i = k; i >= 0; i--) { // multiply by all numerators that come before (on the right)
				n *= (llu) frac.num[i];
					printf ("%d", frac.num[i]);
					printf ( (i != 0) ? "x" : ""); // don't print after the last number
			}
				printf ( (k != l-1) ? "x" : ""); // don't print after the last number

			for (int i = k+1; i < l; i++) { // multiply by all denominators that come after (on the left)
				n *= (llu) frac.den[i];
					printf ("%d", frac.den[i]);
					printf ( (i != l-1) ? "x" : ""); // don't print after the last number
			}
				printf (" = %d\n", n);

			a += n;
		}
	}

		printf ("\n%llu/%llu = ", a, b);

	llu d = MCD_llu (a, b);
	a /= d;
	b /= d;

	result.num = (int) a;
	result.den = (int) b;

	return result;
}


int main (void) {
	setbuf (stdout, NULL); // for buggy output


	// initialize variables
	char s [MAX_STR];
	struct multFRACTION frac;

	fgets (s, MAX_STR, stdin);   s [strcspn(s, "\n")] = ' ';  // for buggy input
		printf ("\n----------\n\n");
	frac = init (s);
		printf ("\n----------\n\n");

	// call functions and output
	struct FRACTION result;
	result = infilzare (frac);
		print_FRACTION (result);


	// free memory
	free (frac.num);
	free (frac.den);

	fflush (stdout); // for buggy output

	return 0;
}



// convert a multiple fraction to a simple fraction

// INPUT: 1 string that encode a multiple fraction
// OUTPUT: the steps of the encoding of the string
//         the steps of the conversion
//         the simple fraction (eventually simplified)



// examples


// 1,4/2,7 = 9/14

// 1,0/2,7 = 1/14

// 1,5,7/2,6,10 = 95/120
//              = 19/24

// 2,4,6,8/3,5,7,9o = 2520/945
//                  = 8/3

// o8,6,4,2/9,7,5,3 = 384/945
//                  = 128/315