#include "../../mylib.h"


struct DIVresult { // integer and fractional parts
	struct NUMBER pint; // integer part
	struct NUMBER pfrac_num; // numerators
	struct NUMBER pfrac_den; // denominators
};


void shift (struct NUMBER num, int a) { // shift the digits and add a new one in the units place
	int l = num.length;
	for (int k = l-1; k > 0; k--) {
		num.digits[k] = num.digits[k-1];
	}
	num.digits[0] = a;
}

void detract (struct NUMBER* ptx_num1, struct NUMBER num2) { // detract the second number from the first [see sottrazione.c]
	// initialize variables
	struct NUMBER num1 = *ptx_num1;
	int l1 = num1.length,	    l2 = num2.length;
	int lmax = l1; // maximum length of the result
	int* d = calloc (lmax, sizeof(int)); // result digits

	// calculate result digits
	int borrow = 0, carry = 0, c1 = 0, c2 = 0, s = 0; // c1 = current digit of the first number (idem c2 for the second one), s = partial difference of current digits (included the carry)
	for (int k = 0; k < lmax; k++) {
		c1 = (k < l1) ? num1.digits[k] : 0 ; // check if it's within the boundaries of the array
		c2 = (k < l2) ? num2.digits[k] : 0 ;

		borrow = (c1 < c2) ? 1 : 0; // check if borrow is needed
		s = (c1 + borrow*10) - (c2 + carry);

		d[k] = get_last_digit (s);
		carry = borrow;
	}

	// update digits
	free (ptx_num1->digits);
	ptx_num1->digits = d;
}


struct DIVresult division (struct NUMBER num1, struct NUMBER num2) {
	// initialize variables
	struct DIVresult result;
	int l1 = num1.length,	    l2 = num2.length;
	int lmax = l1; // maximum length of the result

		for (int k = 0; k < lmax-l1; k++) {
			printf ("0");
		}
		print_NUMBER (num1);	printf ("\n");
		for (int k = 0; k < lmax-l2; k++) {
			printf ("0");
		}
		print_NUMBER (num2);	printf ("\n");
		printf ("\n");

	// allocate memory
	int* d = calloc (lmax, sizeof(int)); // integer part digits

	int* r = calloc (lmax, sizeof(int)); // fractional part digits
	for (int k = 0; k < lmax; k++) {
		r[k] = num1.digits[k];
	}

	struct NUMBER small; // partial dividend
	small.length = l2 + 1;
	small.digits = calloc (small.length, sizeof(int));


	// calculate integer and fractional parts
	for (int k = lmax-1; k >= 0; k--) {

		shift (small, r[k]); // add the next digit
			print_NUMBER (small);	printf (" - ");

		// this is the black box
		int q = 0;
		while (geq (small, num2)) { // division between small and num2
			detract (&small, num2);
			q++;
		}
			print_NUMBER (num2);   printf (" * %d = ", q);   print_NUMBER (small);   printf ("\n");

		d[k] = q;
		for (int j = 0; j < small.length; j++) {
			r[k+j] = small.digits[j];
		}
	}
	result.pint.digits 		= d;
	result.pfrac_num.digits = r;
	result.pfrac_den.digits = num2.digits;

	// calculate integer and fractional parts lengths
	int k = lmax-1;
	while (d[k] == 0 && k > 0) { // ignore trailing 0s (0s at the beginning of the number)
		k--;
	}
	result.pint.length = k+1;

	k = lmax-1;
	while (r[k] == 0 && k >= 0) { // ignore trailing 0s (0s at the beginning of the number)
		k--;
	}
	result.pfrac_num.length = k+1;

	result.pfrac_den.length = l2;


	// free memory
	free (small.digits);

	return result;
}

void cast_out_9 (struct NUMBER num1, struct NUMBER num2, struct DIVresult result) { // check the correctness of the operation by casting out nines
	// calculate remainders
	int m1 = mod9 (num1),	    m2 = mod9 (num2),	    mq = mod9 (result.pint), // quotient	    mr = mod9 (result.pfrac_num); // remainder
	int M1 = (m2 * mq) % 9,	    M2 = (M1 + mr) % 9;

	// output
		print_NUMBER (num1);				printf (" %% 9 = %d\n", m1);
		print_NUMBER (num2);				printf (" %% 9 = %d\n", m2);
		print_NUMBER (result.pint);			printf (" %% 9 = %d\n", mq);
		print_NUMBER (result.pfrac_num);	printf (" %% 9 = %d\n", mr);

		printf ("\n");
		printf ( (M2 == m1) ? "correct" : "wrong");
}


int main (void) {
	setbuf (stdout, NULL); // for buggy output


	// initialize variables
	int n1, n2;
	struct NUMBER num1, num2;

	scanf ("%d%d", &n1, &n2);
	num1 = init_NUMBER (n1);
	num2 = init_NUMBER (n2);
		printf ("\n----------\n\n");

	// call functions
	struct DIVresult result;
	result = division (num1, num2);

		printf ("\n");
		if (result.pfrac_num.length != 0) { // don't print fractional part if there is no remainder
			print_NUMBER (result.pfrac_num);	printf ("/");
			print_NUMBER (result.pfrac_den);	printf (" ");
		}
		print_NUMBER (result.pint);
		printf ("\n\n----------\n\n");

	cast_out_9 (num1, num2, result);


	// free memory
	free (num1.digits);
	free (num2.digits);
	free (result.pint.digits);
	free (result.pfrac_num.digits);
	free (result.pfrac_den.digits);

	return 0;
}



// divide two numbers
// also check the result by casting out nines

// INPUT: 2 numbers
// OUTPUT: the steps of the operation
//         1 mixed number, result of the operation *//         the remainders by 9
//         the result of casting out nines ("correct" or "wrong")

// * the integer represents the quotient and the simple fraction represents the remainder
//   the fraction is not simplified to better show who is the remainder of the division



// examples


// 365 / 2 = 1/2 182

// 365 / 3 = 2/3 121

// 1346 / 4 = 1/2 336

// 5439 / 5 = 4/5 1087

// 9000 / 7 = 5/7 1285

// 10000 / 8 = 1250

// 120037 / 9 = 4/9 13337

// 12532 / 11 = 3/11 1139

// 123586 / 13 = 8/13 9506


// 18456 / 19 = 7/19 971

// 13976 / 23 = 15/23 607

// 24059 / 31 = 3/31 776

// 780005 / 59 = 25/59 13220

// 5917200 / 97 = 6/97 61002


// 1349 / 257 = 64/257 5

// 30749 / 307 = 49/307 100

// 574930 / 563 = 107/563 1021

// 5950000 / 743 = 56/743 8008

// 17849 / 1973 = 92/1973 9

// 1235689 / 4007 = 1533/4007 308