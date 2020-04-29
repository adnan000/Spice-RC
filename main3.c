#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "koolplot.h"

#define M_PI 3.14159265358979323846

typedef struct{
	float real,imag;
	float mag,angle;
	// Silakan tambahkan
} kompleks_t;

typedef struct{
	int freq;
	float arus;
} freqresponse_t;

int dc(int Vs, float C, float R, float L, float V_awal, float t_akhir);
int ac(float R, float L, float C, int Vmag, float t_akhir, int frequency);

float Magnitude(float real, float imag);
float Angle(float real, float imag);
float Real(float mag, float angle);
float Imag(float mag, float angle);

kompleks_t CalculateImpedance(float R, float L, float C, int freq);
kompleks_t CalculateCurrent(kompleks_t V, kompleks_t Z);
void CalculateVoltage(float R, float L, float C, int freq, kompleks_t I, kompleks_t* VR, kompleks_t* VL, kompleks_t* VC);

void PrintComplex(kompleks_t kompleks);
void PrintPhasor(kompleks_t kompleks, int freq);

float overDamped(float R, float L, float C, float Vf, float Vi, float tf, float T);
float underDamped(float R, float L, float C, float Vf, float Vi, float tf, float T);
float criticallyDamped(float R, float L, float C, float Vf, float tf, float T);

int main(){
    float R,L,C,t_akhir,V_awal;
    int frequency,Vmag,pilihan;
	printf("\t  .--R--L--C--.\n");
	printf("\t  |+          |\n");
	printf("\t  V           |\n");
	printf("\t  |-          |\n");
	printf("\t  '-----------'\n\n");
	printf("Masukkan nilai resistansi (Ohm) : "); scanf("%f", &R);
	printf("Masukkan nilai induktansi (H) : "); scanf("%f", &L);
	printf("Masukkan nilai kapasitansi (C) : "); scanf("%f", &C);	
	
	printf("Masukkan nilai tegangan sumber (V) : "); scanf("%d", &Vmag);
	printf("Masukkan waktu pengamatan (ms) : "); scanf("%f", &t_akhir);

    system("@cls||clear");
	
    printf("Pilih tipe analisis :\n1.AC\n2.DC\n\nMasukkan pilihan : "); scanf("%d", &pilihan);

    system("@cls||clear");
	printf("\n");

    if (pilihan==1){
        printf("Masukkan frekuensi (rad/s) : "); scanf("%d", &frequency);
		system("@cls||clear");
        ac(R,L,C,Vmag,t_akhir,frequency);
    }
    if (pilihan==2){
	    printf("Masukkan nilai tegangan saat t<0 (V) : "); scanf("%f", &V_awal);
        system("@cls||clear");
        dc(Vmag, C, R, L, V_awal, t_akhir);
    }
}

int ac(float R, float L, float C, int Vmag, float t_akhir, int frequency){
	float  t, I_temp, VR_temp, VL_temp, VC_temp; //deklarasi variabel
	kompleks_t V, I, Z, VR, VL, VC;

	FILE *v_ac;

    v_ac = fopen("v_ac.csv", "w");

	V.mag = Vmag;
	V.angle = 0;
	V.imag = Imag(V.mag, V.angle);
	V.real = Real(V.mag, V.angle);
	
	printf("\n");
	printf("Simulasi respon frekuensi ...\n");
	
	printf("\n");
		
		Z = CalculateImpedance(R, L, C, frequency);
		I = CalculateCurrent(V, Z);
		CalculateVoltage(R, L, C, frequency, I, &VR, &VL, &VC);
		printf("Impedansi rangkaian : "); PrintComplex(Z); printf(" Ohm\n");
		printf("Arus rangkaian : "); PrintPhasor(I, frequency); printf("A\n");
		printf("Tegangan resistor : "); PrintPhasor(VR, frequency); printf(" V\n");
		printf("Tegangan induktor : "); PrintPhasor(VL, frequency); printf(" V\n");
		printf("Tegangan kapasitor : "); PrintPhasor(VC, frequency); printf(" V\n");
		printf("\n");

		for (t=0;t<t_akhir;t+=0.001){
			I_temp = I.mag*cos((frequency*t)+(M_PI/180)*(I.angle));
			VR_temp = VR.mag*cos((frequency*t)+(M_PI/180)*(VR.angle));
			VL_temp = VL.mag*cos((frequency*t)+(M_PI/180)*(VL.angle));
			VC_temp = VC.mag*cos((frequency*t)+(M_PI/180)*(VC.angle));
			fprintf (v_ac, "%f,%f,%f,%f,%f\n", I_temp, VR_temp, VL_temp, VC_temp, t);
		}
	
	
	
	fclose(v_ac);

		int pil;
		printf("Pilih grafik yang ingin ditampilkan:\n1.Tegangan Induktor\n2.Arus\n"); scanf("%d", &pil);
        
		if(pil==1){
			plotdata t(0.0, t_akhir);    
			plotdata V = VL.mag*cos((frequency*t)+(M_PI/180)*(VL.angle));
			plot(t, V);
		}
		if(pil==2){
			float x=I.mag;
			float y=I.angle;
			plotdata t(0.0, t_akhir);    
			plotdata I = x*cos((frequency*t)+(M_PI/180)*(y));
			plot(t, I);
		}
	return 0;
}

float Magnitude(float real, float imag){
	return sqrt(real*real+imag*imag);
}

float Angle(float real, float imag){
	return atan2(imag,real);
}

float Real(float mag, float angle){
	return mag*cos(angle);
}

float Imag(float mag, float angle){
	return mag*sin(angle);
}

/*
	Hitung impedansi dari nilai R, L, C, dan frekuensi
	Input : RLC
	Output : impedansi
*/
kompleks_t CalculateImpedance(float R, float L, float C, int freq){
	kompleks_t Z;
    Z.imag = (freq*L) - (1/(freq*C));
    Z.real = R;
    Z.mag = Magnitude(Z.real, Z.imag);
    Z.angle = Angle(Z.real, Z.imag);
	return Z;
}

/*
	Hitung arus dari tegangan dan impedansi
	Input : V, Z
	Output : I
*/
kompleks_t CalculateCurrent(kompleks_t V, kompleks_t Z){
	kompleks_t I;
    I.mag = V.mag/Z.mag;
    I.angle = (V.angle - Z.angle)*(180/M_PI);
    I.real = Real(I.mag, I.angle);
    I.imag = Imag(I.mag, I.angle);
	return I;
}

/*
	Hitung VR, VL, VC dari arus dan RLC
	Input : I, RLC
	Output : VR, VL, VC
*/
void CalculateVoltage(float R, float L, float C, int freq, kompleks_t I, kompleks_t *VR, kompleks_t *VL, kompleks_t *VC){
    VR->mag = R*I.mag;
    VR->angle = I.angle;
    VR->real = Real(R*I.mag, I.angle);
    VR->imag = Imag(R*I.mag, I.angle);
    VL->mag = freq*L*I.mag;
    VL->angle = I.angle+90;
    VL->real = Real(freq*L*I.mag, I.angle+90);
    VL->imag = Imag(freq*L*I.mag, I.angle+90);
    VC->mag = (1/(freq*C))*I.mag;
    VC->angle = I.angle-90;
    VC->real = Real((1/(freq*L))*I.mag, I.angle-90);
    VC->imag = Imag((1/(freq*L))*I.mag, I.angle-90);
}

/*
	Cetak bilangan kompleks dalam rektangular
	Input : bilangan kompleks
	Output : representasi bilangan kompleks dicetak ke layar
*/
void PrintComplex(kompleks_t kompleks){
	// your code here
	if (kompleks.imag>0) {
		printf("%.2f + j%.2f", kompleks.real, kompleks.imag); //imajiner positif
    }
	if (kompleks.imag<0){
		printf("%.2f - j%.2f", kompleks.real, (-1)*kompleks.imag); //imajiner negatif
    }
	if (kompleks.imag==0) {
		printf("%.2f",kompleks.real); //imajiner 0
    }
}

/*
	Cetak bilangan kompleks dalam domain waktu
	Input : bilangan kompleks, frekuensi
	Output : representasi domain waktu dicetak ke layar (sudut dalam derajat)
*/
void PrintPhasor(kompleks_t kompleks, int freq){
	// your code here
	if (kompleks.angle>0) {
		printf("%.2f cos(%dt + %.2f)", kompleks.mag, freq, kompleks.angle); //sudut positif
    }
	if (kompleks.angle<0) {
		printf("%.2f cos(%dt - %.2f)", kompleks.mag, freq, (-1)*kompleks.angle); //sudut negatif
	}
	if (kompleks.angle==0){
		printf("%.2f cos(%dt)", kompleks.mag, freq); //sudut 0
    }
}


int dc(int Vs, float C, float R, float L, float V_awal, float t_akhir){

	int condition;
	float T, Vc;

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

	printf("Masukkan DC sweeping time (ms) : "); scanf("%f", &T);
    
	if(condition == 1){underDamped(R,L,C,Vs,V_awal,t_akhir,T);}
    if(condition == 2){overDamped(R,L,C,Vs,V_awal,t_akhir,T);}
    if(condition == 3){criticallyDamped(R,L,C,Vs,t_akhir,T);}

    return(0);
}

float overDamped(float R, float L, float C, float Vf, float Vi, float tf, float T){
    float Vc, s1, s2, t;
	FILE *v_dc;
    v_dc = fopen("v_dc.csv", "w");

    s1 = (-R/2*L) + sqrt(pow(R/2*L, 2) - 1/L*C);
    s2 = (-R/2*L) - sqrt(pow(R/2*L, 2) - 1/L*C);

	for(t=0 ; t<= tf; t=t+T){
    	Vc = (Vf - Vi)*(exp(s1*t)/(s1/s2 - 1) + exp(s2*t)/(s2/s1 - 1)) + Vf;
		printf("Vc(%f) = %f\n", t, Vc);
		fprintf (v_dc, "%f,%f\n", Vc, t);
	}

	fclose(v_dc);

	plotdata t_axis(0.0, tf);    
	plotdata V = (Vf - Vi)*(exp(s1*t_axis)/(s1/s2 - 1) + exp(s2*t_axis)/(s2/s1 - 1)) + Vf;
	plot(t_axis, V);

return Vc;
}

float underDamped(float R, float L, float C, float Vf, float Vi, float tf, float T){
    float Vc, w, s, t;
	FILE *v_dc;
    v_dc = fopen("v_dc.csv", "w");

    w = (sqrt(1/(L*C)) - (pow(R/(2*L), 2)));
    s = (-R/2*L);

	for(t=0 ; t<= tf; t=t+T){
    	Vc = (Vf - (Vf - Vi)*(exp(s*t)*(cos(w*t) - (s/w)*sin(w*t))));
		printf("Vc(%f) = %f\n", t, Vc);
		fprintf (v_dc, "%f,%f\n", Vc, t);
	}

	fclose(v_dc);

	plotdata t_axis(0.0, tf);    
	plotdata V = fabs(Vf - (Vf - Vi)*(exp(s*t_axis)*(cos(w*t_axis) - (s/w)*sin(w*t_axis))));
	plot(t_axis, V);

return Vc;
}

float criticallyDamped(float R, float L, float C, float Vf, float tf, float T){
    float Vc, t;
	FILE *v_dc;
    v_dc = fopen("v_dc.csv", "w");

	for(t=0 ; t<= tf; t=t+T){
    	Vc = (C*exp(-t/(2*L/R)) + Vf);
		printf("Vc(%f) = %f\n", t, Vc);
		fprintf (v_dc, "%f,%f\n", Vc, t);
	}

	fclose(v_dc);

	plotdata t_axis(0.0, tf);    
	plotdata V = (C*exp(-t_axis/(2*L/R)) + Vf);
	plot(t_axis, V);

	return Vc;
}

