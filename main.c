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


/*Fungsi ini sudah termasuk menghitung arus branch dan tegangan node*/
int dc(double Vs, double C, double R, double V_awal, double t_akhir){

    FILE *v_dc;

    v_dc = fopen("v_dc_c.csv", "w");

    double V_C, V_R, I_R=Vs/R, I_C=Vs/R, Vbefore, t=0, delt = 0.001;
    
    Vbefore = V_awal;
    fprintf (v_dc, "%lf,%lf,%lf,%lf,%lf\n", Vbefore, Vs, I_C, I_R, t);
    for (t = 0; t <= t_akhir ; t += delt){
        V_C = (Vs*(delt)+(R*C*Vbefore))/((R*C) + delt);
        I_C = C*(V_C-Vbefore)/delt;
        Vbefore = V_C;
        V_R = Vs-Vbefore;
        I_R = V_R/R;
        fprintf (v_dc, "%lf,%lf,%lf,%lf,%lf\n", V_C, V_R, I_C, I_R, t);
    }

    fclose(v_dc);

    return(0);
}
    
int I_DC_branch();

void printCSV(){
   FILE * fp;

   fp = fopen ("hasil.csv", "w+");
   //fprintf(fp, "%d,%d",V,I);
   
   fclose(fp);
}
    

void printGraph();
