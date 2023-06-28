#include "../../mylib.h"


struct FRACTION mult_to_simple (struct multFRACTION frac) {
	// initialize variables
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
	return result;
}


int main (void) {
	setbuf (stdout, NULL);


	char s[100];
	struct multFRACTION frac;

	gets (s);
	frac = init_multFRACTION (s);

	struct FRACTION result = mult_to_simple (frac);
	print_FRACTION (result);


	free (frac.num);
	free (frac.den);
	return 0;
}



// convert a (struct multFRACTION) to a (struct FRACTION)