#include <stdio.h>
#include <stdlib.h>


void swap (int* ptx_a, int* ptx_b) { // swap two numbers
	int t = *ptx_a;
	*ptx_a = *ptx_b;
	*ptx_b = t;
}

int min (int a, int b) { // return the minimum between a and b
	return (a < b) ? a : b ;
}

int max (int a, int b) { // return the maximum between a and b
	return (a > b) ? a : b ;
}

int power (int b, int e) { // calculate power b^e
	int p = 1;
	for (int k = 0; k < e; k++) {
		p *= b;
	}
	return p;
}

int MCD (int a, int b) { // calculate MCD with Euclid's algorithm
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

int mcm (int a, int b) { // calculate mcm
	int d = MCD (a, b);
	return (a * b) / d;
}


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


void print_vec (int v[], int l) { // output the elements of the vector
	for (int k = 0; k < l; k++) {
		printf ("%d ", v[k]);
	}
}

void print_vec_rev (int v[], int l) { // output the elements of the vector in reversed order
	for (int k = l-1; k >= 0; k--) {
		printf ("%d ", v[k]);
	}
}


int length_str (char s[]) { // return the length of the string
	int k = 0;
	while (s[k] != '\0') {
		k++;
	}
	return k;
}

void print_str (char s[]) { // output the string
	int l = length_str (s);
	for (int k = 0; k < l; k++) {
		printf ("%c", s[k]);
	}
}

char* substr (char s[], int i, int j) { // return the substring between positions i and j
	char* sub = calloc (j-i+2, sizeof(char));

	for (int k = i; k <= j; k++) {
		sub[k-i] = s[k];
	}
	sub[j-i+1] = '\0';

	return sub;
}

char* trim (char s[]) { // delete whitespaces
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

int str_to_int (char s[]) { // convert a string of digits to (int)
	int l = length_str (s);
	int n = 0;
	for (int k = 0; k < l; k++) {
		n += ( (int)(s[k]) - 48 ) * power (10, l-k-1); // convert character to int [see ASCII]
	}
	return n;
}


// ____________________________________________________________________________________________________

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
		while (head->next != NULL) {
			head = head->next;
		}

		list new = malloc (sizeof(struct node));
		new->info = v;
		new->next = NULL;
		head->next = new;
	}
	else { // add_front
		list new = malloc (sizeof(struct node));
		new->info = v;
		new->next = *ptx_head;
		*ptx_head = new;
	}
}

void print_rec (list head) { // output the elements of the list recursively
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


struct NUMBER { // a number broken into digits (the digits are stored in reversed order)
	int* digits;
	int length;
};

struct NUMBER init_NUMBER (int n) { // convert an (int) to a (struct NUMBER)
	// this function does not sanitize input:
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

void print_NUMBER (struct NUMBER num) { // output the digits of the (struct NUMBER)
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

struct FRACTION init_FRACTION (int a, int b) { // convert two numbers to a (struct FRACTION)
	struct FRACTION frac;
	frac.num = a;
	frac.den = b;
	return frac;
}

void print_FRACTION (struct FRACTION frac) { // output numerator and denominator of the (struct FRACTION)
	int a = frac.num, b = frac.den;
	printf("%d/%d", a, b);
}

void simplify (struct FRACTION* frac) { // simplify numerator and denominator of the (struct FRACTION)
	int a = frac->num, b = frac->den;
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
	// this function does not sanitize input:
	// it expects a string with one slash and a balanced number of commas alternated with numbers
	// the string may contain one circle ('o') at the beginning or at the end
	// whitespaces are ignored

	s = trim (s);

	int circle = -1, slash = -1, commas = 0;

	for (int k = 0; s[k] != '\0'; k++) {
		switch (s[k]) {
			case 'o': {
				circle = k;
			} break;
			case '/': {
				slash = k;
			} break;
			case ',': {
				commas += 1;
			} break;
		}
	}
	commas /= 2;


	struct multFRACTION frac;

	frac.circle = circle;

	int l = commas + 1;
	frac.length = l;

	int* num = calloc (l, sizeof(int));
	int* den = calloc (l, sizeof(int));


	int start = -1, end = length_str (s);
	if (circle == 0) {
		start += 1;
	}
	else if (circle > 0) {
		end -= 1;
	}

	int i = start, c = 0; // i = counter for substring beginning, c = counter for array position
	for (int k = start+1; k <= slash; k++) {
		if ((s[k] == ',') || (k == slash)) {
			num[(l-1)-c] = str_to_int (substr (s, i+1, k-1));
			i = k;
			c++;
		}
	}
	frac.num = num;

	c = 0;
	for (int k = slash+1; k <= end; k++) {
		if ((s[k] == ',') || (k == end)) {
			den[(l-1)-c] = str_to_int (substr (s, i+1, k-1));
			i = k;
			c++;
		}
	}
	frac.den = den;


	return frac;
}

void print_multFRACTION (struct multFRACTION frac) {
	int l = frac.length;

	if (frac.circle == 0) {
		printf ("o");
	}

	for (int k = l-1; k >= 0; k--) {
		if (k != l-1) {
			printf (",");
		}
		printf ("%d", (frac.num)[k]);
	}
	printf ("/");
	for (int k = l-1; k >= 0; k--) {
		if (k != l-1) {
			printf (",");
		}
		printf ("%d", (frac.den)[k]);
	}

	if (frac.circle > 0) {
		printf ("o");
	}
}

struct FRACTION mult_to_simple (struct multFRACTION frac) { // convert a (struct multFRACTION) to a (struct FRACTION)
	struct FRACTION result;
	int l = frac.length;

	int a, b;
	if (frac.circle < 0) { // no circle
		b = 1;
		for (int k = 0; k < l; k++) {
			b *= frac.den[k];
		}

		a = 0;
		for (int k = 0; k < l; k++) {
			int n = frac.num[k];
			for (int i = k+1; i < l; i++) {
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
		for (int k = 0; k < l; k++) {
			b *= frac.den[k];
		}

		a = 0;
		for (int k = 0; k < l; k++) {
			int n = frac.num[k];
			for (int i = 0; i < k; i++) {
				n *= frac.num[i];
			}

			for (int i = k+1; i < l; i++) {
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

void print_mixedNUMBER (struct mixedNUMBER mix) {
	print_multFRACTION (mix.pfrac);
	if (mix.pint != 0) {
		printf ("|%d", mix.pint);
	}
}


// ____________________________________________________________________________________________________


int geq (struct NUMBER num1, struct NUMBER num2) { // check if the first (struct NUMBER) is greater or equal to the second
	int l1 = num1.length, l2 = num2.length;

	if (l1 > l2) {
		int k = l1-1;
		while (k > l2-1) {
			if (num1.digits[k] != 0) {
				return 1;
			}
			k--;
		}
	}
	else if (l2 > l1) {
		int k = l2-1;
		while (k > l1-1) {
			if (num2.digits[k] != 0) {
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
	return 1;
}


int mod9 (struct NUMBER num) { // returns the number modulo 9
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


int factorize (int n, list* ptx_head) { // calculate the prime factors and store them in a vector
	int p = 2;
	while (p*p <= n) { // it suffices to reach the square root
		if (n % p == 0) {
			add_front (ptx_head, p);
			return factorize (n/p, ptx_head) + 1;
		}
		p++;
	}
	add_front (ptx_head, n);
	return 1;
}

int* simple_den (int M) { // beautify the denominators of a multiple fraction
	// factors the combine up to 10 are aggregated, followed by the prime factors greater than 10
	// the factors are stored in reversed order
	// the array returned contain a -1 at the end as a termination character
	
	list denlist = NULL;
	int l = 0;

	for (int f = 10; f >= 2; f--) {
		if (M % f == 0) {
			add_back (&denlist, f);
			M /= f;
			l++;
			
			f = 11; // restart the cicle
		}
	}
	
	if (M != 1) {
		l += factorize (M, &denlist);
	}

	int* denvec = calloc (l+1, sizeof(int));
	copy (denlist, denvec);
	denvec[l] = -1;

	free_list (denlist);
	return denvec;
}

struct mixedNUMBER mix_div (int n1, int n2) { // divide two numbers and return a mixed number
	// initialize variables
	int* denvec = simple_den (n2); // array of denominators
	int l = 0;
	while (denvec[l] != -1) {
		l++;
	}

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