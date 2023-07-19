#include "../../mylib.h"


int mult (list head) { // multiply the elements of a list
	if (!head) {
		return 1;
	}
	return mult(head->next) * head->info;
}

void cast_out_9 (int n, list fact) {
	int m = n % 9;
	int M = mult(fact) % 9;

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

	list fact = NULL; // list of prime factors (stored in descending order)
	int l = factorize (n, &fact);
	int* v = calloc (l, sizeof(int)); // vector of prime factors (stored in ascending order)
	copy_rev (fact, v);

	printf ("%d", n);
	printf (" = ");
	print_vec (v, l);

	cast_out_9 (n, fact);


	free (v);
	return 0;
}



// take a number and find his prime factors



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