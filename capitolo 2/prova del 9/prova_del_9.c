#include "../../mylib.h"


int rem (struct NUMBER num) {
	int l = num.length;
	int s = 0;

	for (int k = 0; k < l; k++) {
		s += num.digits[k];
	}
	while (s > 9) {
		s -= 9;
	}
	return s;
}

int cast_out_9 (struct NUMBER num1, struct NUMBER num2, struct NUMBER result, int sel) {
	int r1 = rem (num1);
	int r2 = rem (num2);
	int rr = rem (result);

	switch (sel) {
		case 0:
			return (r1 + r2) == rr;
		case 1:
			return (r1 * r2) == rr;
	}
}


int main (void) {
	setbuf (stdout, NULL);


	int n1, n2, r, sel;
	struct NUMBER num1, num2, result;

	printf ("somma (0) o prodotto (1): ");
	scanf ("%d", &sel);

	printf("primo numero: ");
	scanf ("%d", &n1);
	printf("secondo numero: ");
	scanf ("%d", &n2);
	printf ("risultato: ");
	scanf ("%d", &r);

	num1 = init_NUMBER (n1);
	num2 = init_NUMBER (n2);
	result = init_NUMBER (r);

	int a = cast_out_9 (num1, num2, result, sel);
	printf ("prova del 9: %d", a);


	free (num1.digits);
	free (num2.digits);
	free (result.digits);
	return 0;
}