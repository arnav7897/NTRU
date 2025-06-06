#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "poly.h"

int extract_inverse(FILE *fp, int modulus, poly *res) {
    char line[65536];  // large enough to store all coefficients in one line
    char target[64];
    sprintf(target, "Inverse modulo %d:", modulus);

    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, target)) {
            char *start = strchr(line, '[');
            if (!start) return 0;
            start++;  // move past the '['

            int i = 0;
            while (i < N && *start) {
                // Skip spaces, commas, brackets
                while (*start == ' ' || *start == ',' || *start == '\n' || *start == ']') start++;
                if (*start == '\0') break;

                res->coff[i++] = atoi(start);

                // Skip current number
                while (*start && *start != ' ' && *start != ',' && *start != ']') start++;
            }

            // Fill remaining with zeros if coefficients < N
            while (i < N) res->coff[i++] = 0;

            return 1;
        }
    }

    return 0; // Not found
}


int main() {
    FILE *fp = fopen("../assets/output.txt", "r");
    if (!fp) {
        perror("Cannot open output.txt");
        return 1;
    }

    poly Fp, Fq;

    if (extract_inverse(fp, Q, &Fq)) {
        printf("Inverse in Fq (mod %d): ",Q);
        for (int i = 0; i < N; i++) printf("%d ", Fq.coff[i]);
        printf("\n");
    } else {
        printf("Inverse modulo %d not found.\n",Q);
    }

    rewind(fp);

    if (extract_inverse(fp, P, &Fp)) {
        printf("Inverse in Fp (mod %d): ",P);
        for (int i = 0; i < N; i++) printf("%d ", Fp.coff[i]);
        printf("\n");
    } else {
        printf("Inverse modulo %d not found.\n",P);
    }

    fclose(fp);
    FILE *ptr;
    ptr =fopen("../assets/secret.txt","r");
    poly f,g;
    for(int i =0 ;i<N;i++){
        fscanf(ptr,"%d",&f.coff[i]);
    }
    for(int i =0 ;i<N;i++){
        fscanf(ptr,"%d",&g.coff[i]);
    }
    fclose(ptr); 
    poly h,e,d;
    // key generation
    key_generation(&h,&Fq,&g);
    // encryption
    poly m;
    tertnery(&m,1);
    print_poly("message ",&m);
    ntru_encryption(&e,&m,&h);
    // decrytion
    ntru_decryption(&d,&e,&Fp,&f);
    poly_equal_mod(&d,&m,P);
        
    FILE *ptr3; // storing public parameters in public.txt
    ptr3 = fopen("../assets/public.txt","w");
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
    ptr4 = fopen("../ntru_matrix.txt","w");
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
    fclose(ptr4);
    return 0;
}