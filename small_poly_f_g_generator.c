#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include "poly.h"

int main(){
    FILE *ptr =NULL;
    FILE *ptr2 =NULL;
    ptr = fopen("./assets/input.txt","w");
    if(ptr == NULL){
        printf("error in opening file \n");
    }
    poly f,g;
    srand(time(NULL));
    tertnery(&f,1);
    tertnery(&g,0);
    print_poly("g",&g);
    print_poly("f",&f);
    for(int i =0 ;i<N;i++){
        fprintf(ptr,"%d ",f.coff[i]);
    }
    fclose(ptr);
    ptr2=fopen("./assets/secret.txt","w");
    if(ptr2==NULL){
        printf("error in opening secret.txt");
        exit(0);
    }
    for(int i =0 ;i<N;i++){
        fprintf(ptr2,"%d ",f.coff[i]);
    }
    for(int i =0 ;i<N;i++){
        fprintf(ptr,"%d ",g.coff[i]);
    }
}