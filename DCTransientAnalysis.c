#include <stdio.h>
#include <stdlib.h>
#include <math.h>


float overDamped(float R, float L, float C, float Vf, float Vi, float t){
    float Vc, s1, s2;
    s1 = (-R/2*L) + sqrt(pow(R/2*L, 2) - 1/L*C);
    s2 = (-R/2*L) - sqrt(pow(R/2*L, 2) - 1/L*C);
    Vc = (Vf - Vi)*(exp(s1*t)/(s1/s2 - 1) + exp(s2*t)/(s2/s1 - 1)) + Vf;
return Vc;
}

float underDamped(float R, float L, float C, float Vf, float Vi, float t){
    float Vc, w, s;
    w = (sqrt(1/(L*C)) - (pow(R/(2*L), 2)));
    s = (-R/2*L);
    Vc = (Vf - (Vf - Vi)*(exp(s*t)*(cos(w*t) - (s/w)*sin(w*t))));
return Vc;
}

float criticallyDamped(float R, float L, float C, float Vf, float t){
    float Vc;
    Vc = (C*exp(-t/(2*L/R)) + Vf);
return Vc;
}

int main(){
    float R, L, C, Vf, Vi, t, tf, T, Vc;
    int condition;
    printf("Masukkan nilai resistansi (Ohm) : "); scanf("%f", &R);
	printf("Masukkan nilai induktansi (H) : "); scanf("%f", &L);
	printf("Masukkan nilai kapasitansi (C) : "); scanf("%f", &C);
    printf("Masukkan nilai V final (V) : "); scanf("%f", &Vf);
    printf("Masukkan nilai V awal (V) : "); scanf("%f", &Vi);
    printf("\n");

    if (R/(2*L) > 1/sqrt(L*C)){
        printf("**Case Underdamped**  \n\n");
        condition = 1;
    }
    if (R/(2*L) < 1/sqrt(L*C)){
        printf("**Case Overdamped**  \n\n");
        condition = 2;
    }
    if(R/(2*L) == 1/sqrt(L*C)){
        printf("**Case Criticallydamped**  \n\n");
        condition = 3;
    }
    
    printf("Masukkan waktu pengamatan (s) : "); scanf("%f", &tf);
    printf("Masukkan DC sweeping time (s) : "); scanf("%f", &T);
    for(t=0 ; t<= tf; t=t+T){
        if(condition == 1){Vc = underDamped(R,L,C,Vf,Vi,t);}
        else if(condition == 2){Vc = overDamped(R,L,C,Vf,Vi,t);}
        else if(condition == 3){Vc = criticallyDamped(R,L,C,Vf,t);}
        printf("Vc(%f) = %f\n", t, Vc);
    }
return 0;
}
