#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR 100


typedef unsigned long long int llu;


// int --------------------------------------------------------------------------------------------

void swap (int* ptx_a, int* ptx_b) { // swap two numbers
	int t = *ptx_a;
	*ptx_a = *ptx_b;
	*ptx_b = t;
}

int min (int a, int b) { // return the minimum
	return (a < b) ? a : b ;
}

int max (int a, int b) { // return the maximum
	return (a > b) ? a : b ;
}

int power (int b, int e) { // calculate power b^e
	int p = 1;
	for (int k = 0; k < e; k++) {
		p *= b;
	}
	return p;
}

int MCD (int a, int b) { // calculate the Greatest Common Divisor [GCD] (with Euclid's algorithm)
	if (b > a) {
		swap (&a, &b);
	}

	int r = a % b;
	while (r != 0) {
		a = b;
		b = r;
		r = a % b;
	}
	return b; // last non 0 remainder
}

int mcm (int a, int b) { // calculate the Least Common Multiple [lcd]
	int d = MCD (a, b);
	return (a * b) / d;
}


// int (custom) -----------------------------------------------------------------------------------

int length_int (int n) { // return the number of digits of n
	int k = 0;
	while (n != 0) {
		n = (int) (n / 10);
		k++;
	}
	return (k > 0) ? k : 1 ; // exception: if n = 0 then k = 0, but length should be 1
}

int get_last_digit (int num) { // return the last digit of the number
	return num - (int) (num / 10) * 10;
}


// vector -----------------------------------------------------------------------------------------

void print_vec (int v[], int l) { // output the elements of the vector (with " " separator)
	for (int k = 0; k < l; k++) {
		printf ("%d ", v[k]);
	}
}

void print_vec_rev (int v[], int l) { // output the elements of the vector in reversed order (with " " separator)
	for (int k = l-1; k >= 0; k--) {
		printf ("%d ", v[k]);
	}
}

int length_vec (int v[]) { // calculate the length of a vector that terminates with "-1"
	int l = 0;
	while (v[l] != -1) {
		l++;
	}
	return l;
}


// string -----------------------------------------------------------------------------------------

int length_str (char s[]) { // return the length of the string
	int k = 0;
	while (s[k] != '\0') {
		k++;
	}
	return k;
}

void print_str (char s[]) { // output the string
	for (int k = 0; s[k] != '\0'; k++) {
		printf ("%c", s[k]);
	}
}

char* substr (char s[], int i, int j) { // return the substring between positions i and j
	if (i > j) {
		i = j + 1; // make return an empty string
	}

	char* sub = calloc (j-i+2, sizeof(char));

	for (int k = i; k <= j; k++) {
		sub[k-i] = s[k];
	}
	sub[j-i+1] = '\0';

	return sub;
}

char* trim (char s[]) { // delete whitespaces (return a new string)
	int l = length_str (s);
	char* t = calloc (l, sizeof(char));

	int k = 0, i = 0;
	while (s[k] != '\0') {
		if (s[k] != ' ') {
			t[i] = s[k];
			i++;
		}
		k++;
	}
	t[i] = '\0';

	return t;
}

int str_to_int (char s[]) { // convert a string of digits to a number
	int l = length_str (s);

	int n = 0;
	for (int k = 0; k < l; k++) {
		n += ( (int)(s[k]) - 48 ) * power (10, l-k-1); // convert character to int [see ASCII]
	}
	return n;
}


// ____________________________________________________________________________________________________


// generic struct ---------------------------------------------------------------------------------

struct node { // node of a list (of numbers)
	int info;
	struct node* next;
};
typedef struct node* list;

void add_front (list* ptx_head, int v) { // add an element at the beginning of the list
	list new = malloc (sizeof(struct node));
	new->info = v;
	new->next = *ptx_head;
	*ptx_head = new;
}

void add_back (list* ptx_head, int v) { // add an element at the end of the list
	list head = *ptx_head;
	if (head != NULL) {
		while (head->next != NULL) { // scroll through the list until the end
			head = head->next;
		}

		list new = malloc (sizeof(struct node));
		new->info = v;
		new->next = NULL;
		head->next = new;
	}
	else { // if the list is empty use add_front
		list new = malloc (sizeof(struct node));
		new->info = v;
		new->next = *ptx_head;
		*ptx_head = new;
	}
}

void print_rec (list head) { // output the elements of the list recursively (with " " separator)
	if (head == NULL) {
		return;
	}
	else {
		printf ("%d ", head->info);
		print_rec (head->next);
	}
}

void free_list (list head) { // free memory allocated for the list
	if (head->next == NULL) {
		free (head);
	}
	else {
		free_list (head->next);
	}
}

void copy (list head, int v[]) { // copy a list into a vector
	int k = 0;
	while (head != NULL) {
		v[k] = head->info;
		k++;
		head = head->next;
	}
}

int copy_rev (list head, int v[]) { // copy recursively a list into a vector in reversed order
	if (head->next == NULL) {
		v[0] = head->info;
		return 1;
	}
	else {
		int k = copy_rev (head->next, v);
		v[k] = head->info;
		return k+1;
	}
}


// custom struct ----------------------------------------------------------------------------------

struct NUMBER { // a number broken into digits (the digits are stored in reversed order)
	int* digits;
	int length;
};

struct NUMBER init_NUMBER (int n) { // convert an (int) to a (struct NUMBER)
	// WARNING: this function does not sanitize input
	// it expects a non negative number (otherwise all digits are negative)

	struct NUMBER num;

	int l = length_int (n);
	num.length = l;

	int* d = calloc (l, sizeof(int));
	for (int k = 0; k < l; k++) {
		d[k] = get_last_digit (n);
		n = (int) (n / 10);
	}
	num.digits = d;

	return num;
}

void print_NUMBER (struct NUMBER num) { // output the digits of the number
	int l = num.length;
	int* d = num.digits;

	for (int k = l-1; k >= 0; k--) {
		printf ("%d", d[k]);
	}
}

int NUMBER_to_int (struct NUMBER num) { // convert a (struct NUMBER) to an (int)
	int l = num.length;

	int n = 0;
	for (int k = 0; k < l; k++) {
		n += num.digits[k] * power (10, k);
	}
	return n;
}


struct FRACTION { // a simple fraction
	int num;
	int den;
};

struct FRACTION init_FRACTION (int a, int b) { // convert two (int) to a (struct FRACTION)
	struct FRACTION frac;
	frac.num = a;
	frac.den = b;
	return frac;
}

void print_FRACTION (struct FRACTION frac) { // output the fraction
	int a = frac.num,	    b = frac.den;

	printf("%d/%d", a, b);
}

void simplify (struct FRACTION* frac) { // simplify numerator and denominator of the fraction
	int a = frac->num,	    b = frac->den;
	int d = MCD (a, b);

	frac->num = a / d;
	frac->den = b / d;
}


struct multFRACTION { // a multiple fraction (the numbers are stored in reversed order)
	int* num;
	int* den;
	int length;
	int circle; // <0 = no circle; 0 = circle on the left; >0 = circle on the right
};

struct multFRACTION init_multFRACTION (char s[]) {
	// WARNING: this function does not sanitize input
	// it expects a string with one slash '/' and a balanced number of commas around it alternated with numbers
	// the string may contain one circle 'o' at the beginning or at the end
	// whitespaces are ignored

	// prepare the string and find separators
	s = trim (s);

	int circle = -1, slash = -1, commas = 0; // circle = position of 'o', slash = position of '/', commas = count the number of commas

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


	// intialize result
	struct multFRACTION frac;

	frac.circle = circle;

	int l = commas + 1;
	frac.length = l;

	int* num = calloc (l, sizeof(int)),	   * den = calloc (l, sizeof(int));

	// extract numbers from the string
	int start = -1, end = length_str (s);
	if (circle == 0) { // skip the circle on the left
		start += 1;
	}
	else if (circle > 0) { // skip the circle on the right
		end -= 1;
	}

	int i = start, c = 0; // i = counter for substring beginning, c = counter for array position
	for (int k = start+1; k <= slash; k++) {
		if ((s[k] == ',') || (k == slash)) { // k = counter for substring end
			num [(l-1)-c] = str_to_int (substr (s, i+1, k-1));
			i = k;
			c++;
		}
	}
	frac.num = num;

	c = 0;
	for (int k = slash+1; k <= end; k++) {
		if ((s[k] == ',') || (k == end)) {
			den [(l-1)-c] = str_to_int (substr (s, i+1, k-1));
			i = k;
			c++;
		}
	}
	frac.den = den;


	return frac;
}

void print_multFRACTION (struct multFRACTION frac) { // output the multiple fraction
	int l = frac.length;

	if (frac.circle == 0) { // circle at the beginning
		printf ("o");
	}

	for (int k = l-1; k >= 0; k--) {
		printf ("%d", frac.num[k]);
		printf ( (k != 0) ? "," : ""); // don't print after the last number
	}
	printf ("/");
	for (int k = l-1; k >= 0; k--) {
		printf ("%d", frac.den[k]);
		printf ( (k != 0) ? "," : ""); // don't print after the last number
	}

	if (frac.circle > 0) { // circle at the end
		printf ("o");
	}
}

struct FRACTION mult_to_simple (struct multFRACTION frac) { // convert a multiple fraction to a simple one
	struct FRACTION result;
	int l = frac.length;

	int a, b;
	if (frac.circle < 0) { // no circle
		b = 1;
		for (int k = 0; k < l; k++) { // multiply all denominators
			b *= frac.den[k];
		}

		a = 0;
		for (int k = 0; k < l; k++) {
			int n = frac.num[k];
			for (int i = k+1; i < l; i++) { // multiply by all denominators that come after (on the left)
				n *= frac.den[i];
			}
			a += n;
		}
	}
	else if (frac.circle == 0) { // circle on the left
		a = 1;
		b = 1;
		for (int k = 0; k < l; k++) {
			a *= frac.num[k];
			b *= frac.den[k];
		}
	}
	else { // circle on the right
		b = 1;
		for (int k = 0; k < l; k++) { // multiply all denominators
			b *= frac.den[k];
		}

		a = 0;
		for (int k = 0; k < l; k++) {
			int n = frac.num[k];
			for (int i = 0; i < k; i++) { // multiply by all numerators that come before (on the right)
				n *= frac.num[i];
			}
			for (int i = k+1; i < l; i++) { // multiply by all denominators that come after (on the left)
				n *= frac.den[i];
			}
			a += n;
		}
	}

	result.num = a;
	result.den = b;
	simplify (&result);

	return result;
}


struct mixedNUMBER { // a mixed number
	int pint; // integer part
	struct multFRACTION pfrac; // fractional part
};

void print_mixedNUMBER (struct mixedNUMBER mix) { // output the mixed number
	print_multFRACTION (mix.pfrac);
	if (mix.pint != 0) {
		printf ("|%d", mix.pint);
	}
}


// ____________________________________________________________________________________________________


// custom functions -------------------------------------------------------------------------------

int geq (struct NUMBER num1, struct NUMBER num2) { // check if the first number is greater or equal to the second one
	int l1 = num1.length, l2 = num2.length;

	if (l1 > l2) {
		int k = l1-1;
		while (k > l2-1) {
			if (num1.digits[k] != 0) { // non-zero digit of the first number beyond the length of the second number
				return 1;
			}
			k--;
		}
	}
	else if (l2 > l1) {
		int k = l2-1;
		while (k > l1-1) {
			if (num2.digits[k] != 0) { // non-zero digit of the second number beyond the length of the first number
				return 0;
			}
			k--;
		}
	}

	int l = min (l1, l2);
	for (int k = l-1; k >= 0; k--) {
		if (num1.digits[k] > num2.digits[k]) {
			return 1;
		}
		else if (num1.digits[k] < num2.digits[k]) {
			return 0;
		}
	}
	return 1; // the numbers are equal
}

int mod9 (struct NUMBER num) { // return the remainder of the number modulo 9
	int l = num.length;
	struct NUMBER result;

	int s = 0;
	for (int k = 0; k < l; k++) {
		s += num.digits[k];
	}

	result = init_NUMBER (s);
	if (result.length > 1) {
		return mod9 (result);
	}
	else {
		return (s == 9) ? 0 : s; // 9 = 0 (mod 9)
	}
}


int factorize_rec (int n, list* ptx_head) { // calculate prime factors recursively and append them in the list (in descending order), return the number of these factors
	int p = 2;
	while (p*p <= n) { // it suffices to reach the square root
		if (n % p == 0) {
			add_front (ptx_head, p);
			return factorize_rec (n/p, ptx_head) + 1; // count the factors
		}
		p++;
	}
	add_front (ptx_head, n); // n is prime
	return 1;
}

int* factorize (int M) { // calculate the prime factors and store them in a vector (in descending order)
	// initialize variables
	list denlist = NULL;
	int l = 0;

	// find factors
	if (M != 1) {
		l += factorize_rec (M, &denlist);
	}

	// copy in the vector
	int* denvec = calloc (l+1, sizeof(int));
	copy (denlist, denvec);
	denvec[l] = -1; // termination character (to retrieve length of vector)

	// free memory
	free_list (denlist);

	return denvec;
}

int* simple_den (int* denvec) { // beautify the denominators of a multiple fraction
	// factors the combine up to 10 are aggregated, followed by the prime factors greater than 10
	// the factors are stored in reversed order
	// WARNING: this function expects a vector that terminate with a "-1"

	// initialize variables
	int l = length_vec (denvec);
	int* denvec_new = calloc (l+1, sizeof(int));
	int j = 0; // counter for denvec_new


	// find factors bigger than 10
	while (1) { // infinite cycle (until broken)
		// find the maximum
		int imax = 0; // index of the maximum
		for (int k = 0; k < l; k++) {
			if (denvec[k] > denvec[imax]) {
				imax = k;
			}
		}

		if (denvec[imax] > 10) {
			denvec_new[j] = denvec[imax];
			denvec[imax] = -1; // don't pick the same element
			j++;
		}
		else {
			break;
		}
	}


	// find factors up to 10
	int* sp = calloc (10, sizeof(int)); // count the number of factor up to 10 (sp = small primes)
	for (int k = 0; k < l; k++) {
		switch (denvec[k]) {
			case 2: {
				sp[2]++;
				break;
			}
			case 3: {
				sp[3]++;
				break;
			}
			case 4: {
				sp[4]++;
				break;
			}
			case 5: {
				sp[5]++;
				break;
			}
			case 6: {
				sp[6]++;
				break;
			}
			case 7: {
				sp[7]++;
				break;
			}
			case 8: {
				sp[8]++;
				break;
			}
			case 9: {
				sp[9]++;
				break;
			}
			case 10: {
				sp[0]++;
				break;
			}
		}
	}

	// combine primes
	while ((sp[2] >= 1) && (sp[5] >= 1)) { // 2x5 = 10
		sp[2] -= 1;
		sp[5] -= 1;
		denvec_new[j] = 10;
		j++;
	}
	for (int k = 0; k < sp[0]; k++) {
		denvec_new[j] = 10;
		j++;
	}

	while (sp[3] >= 2) { // 3x3 = 9
		sp[3] -= 2;
		denvec_new[j] = 9;
		j++;
	}
	for (int k = 0; k < sp[9]; k++) {
		denvec_new[j] = 9;
		j++;
	}

	while (sp[2] >= 3) { // 2x2x2 = 8
		sp[2] -= 3;
		denvec_new[j] = 8;
		j++;
	}
	while ((sp[2] >= 1) && (sp[4] >= 1)) { // 2x4 = 8
		sp[2] -= 1;
		sp[4] -= 1;
		denvec_new[j] = 8;
		j++;
	}
	for (int k = 0; k < sp[8]; k++) {
		denvec_new[j] = 8;
		j++;
	}

	for (int k = 0; k < sp[7]; k++) {
		denvec_new[j] = 7;
		j++;
	}

	while ((sp[2] >= 1) && (sp[3] >= 1)) { // 2x3 = 6
		sp[2] -= 1;
		sp[3] -= 1;
		denvec_new[j] = 6;
		j++;
	}
	for (int k = 0; k < sp[6]; k++) {
		denvec_new[j] = 6;
		j++;
	}

	for (int k = 0; k < sp[5]; k++) {
		denvec_new[j] = 5;
		j++;
	}

	while (sp[2] >= 2) { // 2x2 = 4
		sp[2] -= 2;
		denvec_new[j] = 4;
		j++;
	}
	for (int k = 0; k < sp[4]; k++) {
		denvec_new[j] = 4;
		j++;
	}

	for (int k = 0; k < sp[3]; k++) {
		denvec_new[j] = 3;
		j++;
	}

	for (int k = 0; k < sp[2]; k++) {
		denvec_new[j] = 2;
		j++;
	}

	denvec_new[j] = -1; // termination character (to retrieve length of vector)


	// free memory
	free (sp);

	return denvec_new;
}

struct mixedNUMBER mix_div (int n1, int n2) { // divide two numbers and return a mixed number
	// initialize variables
	int* denvec = simple_den (factorize (n2)); // factorize the denominator
	int l = length_vec (denvec);

	struct mixedNUMBER result;
	result.pfrac.circle = -1;
	result.pfrac.length = l;
	result.pfrac.num = calloc (l, sizeof(int));
	result.pfrac.den = denvec;

	// calculate numerators
	int m = n1;
	for (int k = l-1; k >= 0; k--) {
		result.pfrac.num[k] = m % result.pfrac.den[k];
		m /= result.pfrac.den[k];
	}
	result.pint = m;

	return result;
}

int find_prime (int* vec, int l) { // find a prime number different from the elements of the vector
	// initialize variables
	int* primes = calloc (l, sizeof(int)); // store primes [dynamic programming]
	int verif, isprime, j = 0, p = 7; // verif = flag controlling the termination of the cycle, isprime = flag checking whether p is prime, j = primes counter (give position of last encountered prime), p = current prime (start from 7)

	// find the prime
	while (1) { // infinite cycle (until broken)
		// check if p is prime
		isprime = 1;
		if ((p % 2 == 0) || (p % 3 == 0) || (p % 5 == 0)) { // check divisibility by 2,3,5
			isprime = 0;
		}
		for (int k = 0; k < j; k++) { // check divisibility by all other smaller primes
			if (p % primes[k] == 0) {
				isprime = 0;
				break;
			}
		}

		if (isprime == 1) {
			if (j == l) { // checked as many primes as the number of denominators [pigeonhole principle]
				break;
			}

			// check if p is already in the denominators
			verif = 1;
			for (int k = 0; k < l; k++) {
				if (p == vec[k]) {
					verif = 0;
					primes[j] = p;
					j++;
					break;
				}
			}

			if (verif == 1) {
				break;
			}
		}

		p++;
	}

	// free memory
	free (primes);

	return p;
}