#include "../../mylib.h"


void cast_out_9 (int n, int* denvec, int l) {
	int m = n % 9;
	int M = 1;
	for (int k = 0; k < l; k++) {
		M *= denvec[k];
		M = M % 9;
	}

	if (M == m) {
		printf ("\n\ncorrect");
	}
	else {
		printf ("\n\nwrong");
	}
}


int main (void) {
	setbuf (stdout, NULL); // for buggy output


	int n;

	scanf ("%d", &n);

	int* denvec = simple_den (n); // array of denominators
	int l = 0;
	while (denvec[l] != -1) {
		l++;
	}
	
	printf ("%d", n);
	printf (" = ");
	print_vec_rev (denvec, l);

	cast_out_9 (n, denvec, l);


	free (denvec);
	return 0;
}



// find the "rule" of a number
// this is equivalent to finding his prime factors (and beautify the factors)
// also check the result by casting out nines

// INPUT: 1 number
// OUTPUT: the sequence of factors
//         "correct" if the cast out 9 test is passed, "wrong" otherwise



// examples

// 805 = 5×7×23

// 957 = 3×11×29

// 951 = 3×317

// 873 = 9x97

// 1469 = 13×113

// 2543 primo

// 624481 = 11×11×13×397

// 126 = 2×7×9

// 2112 = 4×6×8×11 =
//      = 3×8×8×11

// 4664 = 8×11×53

// 13652 = 4×3413

// 15560 = 4×10×389

// 32600 = 2×10×10×163

// 7546000 = 2×7×7×7×10×10×10×11