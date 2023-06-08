#include <stdio.h>
#include <stdlib.h>


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


int length_str (char* s) { // return the length of the string
	int k = 0;
	while (s[k] != '\0') {
		k++;
	}
	return k;
}

char* substr (char* s, int i, int j) { // return the substring between positions i and j
	char* sub = calloc (j-i+2, sizeof(char));

	for (int k = i; k <= j; k++) {
		sub[k-i] = s[k];
	}
	sub[j-i+1] = '\0';

	return sub;
}

char* trim (char* s) { // deletes whitespaces
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

int str_to_int (char* s) { // convert a string of digits to (int)
	int l = length_str (s);
	int n = 0;
	for (int k = 0; k < l; k++) {
		n += ( (int)(s[k]) - 48 ) * power (10, l-k-1); // convert character to int [see ASCII]
	}
	return n;
}


// ____________________________________________________________________________________________________


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

struct FRACTION init_FRACTION (int a, int b) {
	struct FRACTION frac;
	frac.num = a;
	frac.den = b;
	return frac;
}

void print_FRACTION (struct FRACTION frac) {
	printf("%d/%d", frac.num, frac.den);
}


struct multFRACTION { // a multiple fraction
	int* num;
	int* den;
	int length;
	int circle; // <0 = no circle; 0 = circle on the left; >0 = circle on the right
};

struct multFRACTION init_multFRACTION (char* s) {
	// this function does not sanitize input:
	// it expects a string with one slash and a balanced number of commas alternated with numbers
	// the string may contain one circle ('o') at the beginning or at the end
	// whitespaces are ignored

	s = trim (s);

	int circle_pos = -1, slash_pos = -1, commas = 0;

	int k = 0;
	while (s[k] != '\0') {
		switch (s[k]) {
			case 'o': {
				circle_pos = k;
			} break;
			case '/': {
				slash_pos = k;
			} break;
			case ',': {
				commas += 1;
			} break;
		}
		k++;
	}
	commas /= 2;


	struct multFRACTION frac;

	int circle = circle_pos;
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

	int i = start, c = 0;
	for (k = start + 1; k <= slash_pos; k++) {
		if ((s[k] == ',') || (k == slash_pos)) {
			num[l-c-1] = str_to_int (substr (s, i+1, k-1));
			i = k;
			c++;
		}
	}
	frac.num = num;

	c = 0;
	for (k = slash_pos + 1; k <= end; k++) {
		if ((s[k] == ',') || (k == end)) {
			den[l-c-1] = str_to_int (substr (s, i+1, k-1));
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