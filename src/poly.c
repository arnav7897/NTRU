#include "poly.h"

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

