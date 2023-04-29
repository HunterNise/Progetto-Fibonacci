#include "mylib.h"


struct NUMBER sum (struct NUMBER num1, struct NUMBER num2) {
	struct NUMBER result;
	int l1 = num1.length, l2 = num2.length;
	int lmax = max (num1.length, num2.length) + 1;
	
	int* d = calloc (lmax, sizeof(int));
	int carry = 0, c1 = 0, c2 = 0;
	for (int k = 0; k < lmax; k++) {
		c1 = (k < l1) ? num1.digits[k] : 0 ; // check if it's within the boundary of the array
		c2 = (k < l2) ? num2.digits[k] : 0 ;
		
		int s = c1 + c2 + carry;
		d[k] = get_last_digit (s);
		carry = (int) (s / 10);
	}
	result.digits = d;
	
	int k = lmax-1;
	while (d[k] == 0 && k >= 0) { // ignore trailing 0s (0s at the beginning of the number)
		k--;
	}
	result.length = (k >= 0) ? k+1 : 1 ; // exception: if result = 0 then k = -1, but length should be 1
	
	return result;
}


int main (void) {
	int n1, n2;
    struct NUMBER num1, num2;
	
	scanf ("%d%d", &n1, &n2);
	num1 = init (n1);
	num2 = init (n2);
	
	struct NUMBER result = sum (num1, num2);
	print (result);
	
	free (num1.digits);
	free (num2.digits);
	free (result.digits);
    return 0;
}