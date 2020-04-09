#include <stdio.h>
#include <stdlib.h>

typedef struct comp{
    char kind;
    float val;
    int con1, con2;
}comp;

comp c;
int ncomp;


int main(){

    printf("jumlah komponen : ");
    scanf("%d", &ncomp);

    comp arr_c[ncomp];
    int i;

    for (i = 0; i<ncomp ; i++){
        printf("\nMasukkan komponen ke-%d\n", i+1);
        printf("jenis : ");
        scanf(" %c", &arr_c[i].kind);
        printf("nilai : ");
        scanf(" %f", &arr_c[i].val);
        printf("koneksi : ");
        scanf(" %d %d", &arr_c[i].con1, &arr_c[i].con2);
    }
    
/*Saran aja yaa ini bentuk main sama structnya*/


int* netlistToMatrix(char *netlist);

int V_DC_node(); 

int I_DC_branch();

void printCSV();

void printGraph();
