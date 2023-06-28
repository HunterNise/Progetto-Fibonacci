#include "../../mylib.h"


int mult (list head) { // multiply the elements of a list
	if (!head) {
		return 1;
	}
	return mult(head->next) * head->info;
}

void copy_rev (list head, int* v, int k) { // copy a list into a vector in reversed order
	if (!head) {
		return;
	}
	v[k] = head->info;
	copy_rev (head->next, v, k-1);
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
	copy_rev (fact, v, l-1);

	printf ("%d", n);
	printf (" = ");
	print_vec (v, l);

	cast_out_9 (n, fact);


	free (v);
	return 0;
}



// take a number and find his prime factors