#include <stdio.h>
#include <stdlib.h>


int max (int a, int b) { // returns the maximum between a and b
	return (a > b) ? a : b ;
}

int length (int n) { // return the number of digits of n
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


struct NUMBER { // a number broken into digits (the digits are stored in reversed order)
	int* digits;
	int length;
};

struct NUMBER init (int n) { // convert an (int) to a (struct NUMBER)
	struct NUMBER num;
	
	int l = length (n);
	num.length = l;
	
	int* d = calloc (l, sizeof(int));
	for (int k = 0; k < l; k++) {
		d[k] = get_last_digit (n);
		n = (int) (n / 10);
	}
	num.digits = d;
	
	return num;
}

void print (struct NUMBER num) { // output the digits of the number
	int l = num.length;
	int* d = num.digits;
	
	for (int k = l-1; k >= 0; k--) {
		printf ("%d", d[k]);
	}
}