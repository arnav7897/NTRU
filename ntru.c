#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "poly.h"

#define N 7

int extract_inverse(FILE *fp, int modulus, poly *res) {
    char line[256];
    char target[64];
    sprintf(target, "Inverse modulo %d:", modulus);

    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, target)) {
            // Find the opening bracket
            char *start = strchr(line, '[');
            if (!start) return 0;
            start++; // move past '['

            // Extract integers
            for (int i = 0; i < N; i++) {
                while (*start == ' ') start++;
                res->coff[i] = atoi(start);
                while (*start && *start != ' ') start++;
            }

            return 1; // Success
        }
    }

    return 0; // Not found
}

int main() {
    FILE *fp = fopen("./assets/output.txt", "r");
    if (!fp) {
        perror("Cannot open output.txt");
        return 1;
    }

    poly Fp, Fq;

    if (extract_inverse(fp, 41, &Fq)) {
        printf("Inverse in Fq (mod 41): ");
        for (int i = 0; i < N; i++) printf("%d ", Fq.coff[i]);
        printf("\n");
    } else {
        printf("Inverse modulo 41 not found.\n");
    }

    // rewind and read again for p = 3
    rewind(fp);

    if (extract_inverse(fp, 3, &Fp)) {
        printf("Inverse in Fp (mod 3): ");
        for (int i = 0; i < N; i++) printf("%d ", Fp.coff[i]);
        printf("\n");
    } else {
        printf("Inverse modulo 3 not found.\n");
    }

    fclose(fp);
    FILE *ptr;
    ptr =fopen("./assets/secret.txt","r");
    poly f,g;
    for(int i =0 ;i<N;i++){
        fscanf(ptr,"%d",&f.coff[i]);
    }
    for(int i =0 ;i<N;i++){
        fscanf(ptr,"%d",&g.coff[i]);
    }
    // KEY generation steps -->
    // f,g --> as t(d+1,d) and t(d,d) respectively
    // calculating inverses --> Fq = f inv mod q ,, Fp = f inv mod p 
    // and calculating h(x) = Fq(x)*g(x) mod q 
    poly h;
    poly_multiply_mod(&h,&Fq,&g,Q); // h(x) = Fq(x)*g(x) mod q 
    print_poly("public key ", &h);

    // encryption steps-->
    // calculating a random ==> T(d,d) for a noise and randomness
    // message chossing and it must be centerlifted coff => (-p/2,p/2)
    // e(x) = P*h(x)*r(x) + m(x) mod q
    poly m,r,e,temp;
    tertnery(&m,1); // here (-p/2 , p/2 ) == (-1 , 1) so using ternery only
    tertnery(&r,0); // random as T(D,D)
    poly_multiply_mod(&temp,&h,&r,Q); // temp=h(x)*r(x) mod Q
    for(int i = 0;i<N;i++){
        temp.coff[i] =(P*temp.coff[i])%Q; // P*temp (mod Q )
    }
    poly_add_mod(&e,&temp,&m,Q); // e(x) = temp + m(x) (mod Q)
    print_poly("random ",&r);
    print_poly("message ",&m);
    print_poly("cipher poly ",&e);

    // decryption steps->
    // a(x) = f*e mod Q;
    // centerlift a(x) coff wrt Q;
    // d(x) = Fp*a (mod p)
    // d(x) = m(x)

    poly a,d;
    poly_multiply_mod(&a,&f,&e,Q);
    for(int i = 0;i<N;i++){
        if(a.coff[i]> Q/2){
            a.coff[i] = a.coff[i] - Q;
        }
    }

    poly_multiply_mod(&d,&Fp,&a,P);
    for (int i = 0; i < N; i++) {
        if (d.coff[i] > P/2) {
            d.coff[i] = d.coff[i] - P;
        }
    }
    print_poly("final decryption",&d);
    
    FILE *ptr3; // storing public parameters in public.txt
    ptr3 = fopen("./assets/public.txt","w");
    if(ptr3==NULL){
        printf("error in opening file\n");
        exit(0);
    }
    fprintf(ptr3,"%s ","public key");
    for(int i =0 ;i<N;i++){
        fprintf(ptr3,"%d ",h.coff[i]);
    }
    fprintf(ptr3,"%s","\nencryption ");
    for(int i =0 ;i<N;i++){
        fprintf(ptr3,"%d ",e.coff[i]);
    }
    fclose(ptr3);
   
    // NTRU matrix formation for LLL aplication
    // I H
    // 0 Q*I

    FILE *ptr4;
    ptr4 = fopen("./assets/ntru_matrix.txt","w");
    if(ptr4==NULL){
        printf("error in opening ntru_matrix.txt\n");
        exit(0);
    }
    for(int i =0 ;i<2*N;i++){
        for(int j=0;j<2*N;j++){
            if(j<N && i<N){ // I matrix
                if(j==i){
                    fprintf(ptr4,"%d ",1);
                }else{
                    fprintf(ptr4,"%d ",0);
                }
            }
            if(j>=N && i>=N){ // QI 
                if(i==j){
                    fprintf(ptr4,"%d ",Q);
                }else{
                    fprintf(ptr4,"%d ",0);
                }
            }
            if(j<N && i>=N){ // 0 matrix
                fprintf(ptr4,"%d ",0);
            }
            if(j>=N && i<N){ //h matrix
                fprintf(ptr4,"%d ",h.coff[((j-i)%N +N)%N]);
            }
        }
        fprintf(ptr4,"%c",'\n');
    }
    return 0;
}