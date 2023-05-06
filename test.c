#include "mylib.h"


int main (void) {
	setbuf (stdout, NULL);
	
	char s[100];
	gets (s);
	
	struct multFRACTION f;
	f = init_multFRACTION (s);
	printf ("%d %d\n", f.length, f.circle);
	print_multFRACTION (f);
	
	return 0;
}