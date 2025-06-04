#ifndef POLY_H
#define POLY_H

//  note for ntru scheme parameter must satisy condition like
// Q > (6D +1)P
// P must be small prime
// Q,P must be co prime

#define N 7
#define Q 41
#define P 3
#define D 2

typedef struct 
{
    int coff[N];
}poly;

// if mode =1 then give T(d+1,d) and if mode =0 gives T(d,d)
void tertnery(poly* res,int mode);

// function to print the polynomial coefficients , name is arbitary value which you want to call it output is {name} {polynomial coefficients} 
void print_poly(char name[],poly *polynomial);

// function for polynomial modulus addition with respect to modulus mod
void poly_add_mod(poly* res, poly* x, poly* y,int mod);

// function for polynomial modulous multiplication woth respect to modulus mod 
void poly_multiply_mod(poly* res,poly* a,poly* b,int mod);

#endif
