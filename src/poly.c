#include "poly.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include <stdlib.h>
#include <string.h>
#include "poly.h"

#define KARATSUBA_THRESHOLD 16 

void poly_multiply_naive(poly* res, poly* a, poly* b, int mod) {
    memset(res->coff, 0, sizeof(res->coff));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int deg = (i + j) % N;
            res->coff[deg] += a->coff[i] * b->coff[j];
        }
    }
    for (int i = 0; i < N; i++) {
        res->coff[i] = ((res->coff[i] % mod) + mod) % mod;
    }
}

void poly_add(int* res, int* a, int* b, int len) {
    for (int i = 0; i < len; i++) res[i] = a[i] + b[i];
}
void poly_sub(int* res, int* a, int* b, int len) {
    for (int i = 0; i < len; i++) res[i] = a[i] - b[i];
}

void karatsuba(const int* A, const int* B, int* R, int len) {
    if (len <= KARATSUBA_THRESHOLD) {
        for (int i = 0; i < 2 * len; i++) R[i] = 0;
        for (int i = 0; i < len; i++) {
            for (int j = 0; j < len; j++) {
                R[i + j] += A[i] * B[j];
            }
        }
        return;
    }

    int half = len / 2;

    int *A0 = (int*)calloc(half, sizeof(int));
    int *A1 = (int*)calloc(half, sizeof(int));
    int *B0 = (int*)calloc(half, sizeof(int));
    int *B1 = (int*)calloc(half, sizeof(int));

    memcpy(A0, A, half * sizeof(int));
    memcpy(A1, A + half, half * sizeof(int));
    memcpy(B0, B, half * sizeof(int));
    memcpy(B1, B + half, half * sizeof(int));

    int *Z0 = (int*)calloc(2 * half, sizeof(int));
    int *Z2 = (int*)calloc(2 * half, sizeof(int));
    int *Z1 = (int*)calloc(2 * half, sizeof(int));
    int *A_sum = (int*)calloc(half, sizeof(int));
    int *B_sum = (int*)calloc(half, sizeof(int));

    karatsuba(A0, B0, Z0, half);       // Z0 = A0 * B0
    karatsuba(A1, B1, Z2, half);       // Z2 = A1 * B1
    poly_add(A_sum, A0, A1, half);     // A_sum = A0 + A1
    poly_add(B_sum, B0, B1, half);     // B_sum = B0 + B1
    karatsuba(A_sum, B_sum, Z1, half); // Z1 = (A0+A1)*(B0+B1)

    for (int i = 0; i < 2 * half; i++) {
        Z1[i] -= Z0[i] + Z2[i];  // Z1 = Z1 - Z0 - Z2
    }

    memset(R, 0, sizeof(int) * 2 * len);
    for (int i = 0; i < 2 * half; i++) {
        R[i] += Z0[i];
        R[i + half] += Z1[i];
        R[i + 2 * half] += Z2[i];
    }

    free(A0); free(A1); free(B0); free(B1);
    free(Z0); free(Z1); free(Z2);
    free(A_sum); free(B_sum);
}


void poly_multiply_mod(poly* res, poly* a, poly* b, int mod) {
    if (N <= KARATSUBA_THRESHOLD) {
        poly_multiply_naive(res, a, b, mod);
        return;
    }

    int len = 1;
    while (len < N) len <<= 1;

    int *A = (int*)calloc(len, sizeof(int));
    int *B = (int*)calloc(len, sizeof(int));
    int *R = (int*)calloc(2 * len, sizeof(int));

    for (int i = 0; i < N; i++) {
        A[i] = a->coff[i];
        B[i] = b->coff[i];
    }

    karatsuba(A, B, R, len);

    for (int i = 0; i < N; i++) {
        res->coff[i] = 0;
    }

    for (int i = 0; i < 2 * N; i++) {
        res->coff[i % N] = (res->coff[i % N] + R[i]) % mod;
    }

    for (int i = 0; i < N; i++) {
        res->coff[i] = (res->coff[i] + mod) % mod;
    }

    free(A); free(B); free(R);
}


void tertnery(poly* res,int mode){
    int count = D;
    memset(res->coff,0,sizeof(res->coff));
    while(count >0){
        int degg = rand() %N;
        res->coff[degg] = -1;
        count--;
    }
    count =D;
    if(mode==0){ // t(d,d)
        while(count>0){
            int degg = rand()%N;
            if(res->coff[degg] == 0){
                res->coff[degg] = 1;
                count--;
            }
        }
    }else{ // t(d+1,d)
        while(count>-1){
            int degg = rand()%N;
            if(res->coff[degg] == 0){
                res->coff[degg] = 1;
                count--;
            }
        }
    }
}

void print_poly(char name[],poly *polynomial){
    printf("%s ",name);
    for(int i= 0;i<N;i++){
        printf(" %d ",polynomial->coff[i]);
    }
    printf("\n");
}

void poly_add_mod(poly* res, poly* x, poly* y,int mod){
    for(int i=0;i<N;i++){
        res->coff[i] = x->coff[i] +y->coff[i];
    }
     for(int i = 0;i<N;i++){
        res->coff[i] = ((res->coff[i] % mod)+mod)%mod;
    }
}
    // KEY generation steps -->
    // f,g --> as t(d+1,d) and t(d,d) respectively
    // calculating inverses --> Fq = f inv mod q ,, Fp = f inv mod p 
    // and calculating h(x) = Fq(x)*g(x) mod q 
void key_generation(poly * result, poly* Fq, poly* g){
    poly_multiply_mod(result,Fq,g,Q); // h(x) = Fq(x)*g(x) mod q 
    print_poly("public key ", result);
}

// encryption steps-->
// calculating a random ==> T(d,d) for a noise and randomness
// message chossing and it must be centerlifted coff => (-p/2,p/2)
// e(x) = P*h(x)*r(x) + m(x) mod q
void ntru_encryption(poly* result, poly* message,poly* public_key_polynomial){
    poly r,temp;
    tertnery(&r,0); // random as T(D,D)
    poly_multiply_mod(&temp,public_key_polynomial,&r,Q); // temp=h(x)*r(x) mod Q
    for(int i = 0;i<N;i++){
        temp.coff[i] =(P*temp.coff[i])%Q; // P*temp (mod Q )
    }
    poly_add_mod(result,&temp,message,Q); // e(x) = temp + m(x) (mod Q)
    print_poly("cipher-polynomial ",result);
}

// decryption steps->
// a(x) = f(x)*cipher(x) mod Q;
// centerlift a(x) coff wrt Q;
// d(x) = Fp(x)*a(x) (mod p)
// d(x) = m(x)
void ntru_decryption(poly* result, poly *cipher, poly*Fp, poly* f){
    poly a;
    poly_multiply_mod(&a,f,cipher,Q);
    for(int i = 0;i<N;i++){
        if(a.coff[i]> Q/2){
            a.coff[i] = a.coff[i] - Q;
        }
    }

    poly_multiply_mod(result,Fp,&a,P);
    for (int i = 0; i < N; i++) {
        if (result->coff[i] > P/2) {
            result->coff[i] = result->coff[i] - P;
        }
    }
    print_poly("final decryption",result);
}

void poly_equal_mod(poly * a , poly* b,int mod){
    for(int i =0;i<N;i++){
        if( ((((a->coff[i])%mod)+mod)%mod) == ((((b->coff[i])%mod)+mod)%mod)){
            continue;
        }
        else{
            printf("not equal at %d a is %d and b is %d \n",i,a->coff[i],b->coff[i]);
            return ;
        }
    }
    printf("Equal \n");
}