#include "../../mylib.h"


int main (void) {
	setbuf (stdout, NULL); // for buggy output
	
	
	char s[100];
	struct multFRACTION frac;

	gets (s);
	frac = init_multFRACTION (s);

	struct FRACTION result = mult_to_simple (frac);
	print_FRACTION (result);


	free (frac.num);
	free (frac.den);
	fflush (stdout); // for buggy output
	return 0;
}



// convert a (struct multFRACTION) to a (struct FRACTION)