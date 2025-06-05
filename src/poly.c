#include "poly.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

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

void poly_multiply_mod(poly* res,poly* a,poly* b,int mod){
    for (int i = 0; i < N; i++) res->coff[i] = 0;
    for(int i =0;i<N;i++){
        for(int j=0;j<N;j++){
            int degree = (i+j)%N; // x^N ka mod idar ho gaya 
            res->coff[degree] += (a->coff[i])*(b->coff[j]); 
        }
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
    poly a,d;
    poly_multiply_mod(&a,f,cipher,Q);
    for(int i = 0;i<N;i++){
        if(a.coff[i]> Q/2){
            a.coff[i] = a.coff[i] - Q;
        }
    }

    poly_multiply_mod(&d,Fp,&a,P);
    for (int i = 0; i < N; i++) {
        if (d.coff[i] > P/2) {
            d.coff[i] = d.coff[i] - P;
        }
    }
    print_poly("final decryption",&d);
}