#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

int dc(int Vs, float C, float R, float V_awal, float t_akhir);
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


int main(){
    float R,L,C,t_akhir,V_awal;
    int frequency,Vmag,pilihan;

	printf("Masukkan nilai resistansi (Ohm) : "); scanf("%f", &R);
	printf("Masukkan nilai induktansi (H) : "); scanf("%f", &L);
	printf("Masukkan nilai kapasitansi (C) : "); scanf("%f", &C);	
	
	printf("Masukkan nilai tegangan sumber (V) : "); scanf("%d", &Vmag);
	printf("Masukkan waktu pengamatan (s) : "); scanf("%f", &t_akhir);

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
        dc(Vmag, C, R, V_awal, t_akhir);
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


int dc(int Vs, float C, float R, float V_awal, float t_akhir){

    FILE *v_dc;

    v_dc = fopen("v_dc.csv", "w");

    float V_C, V_R, I_R=Vs/R, I_C=Vs/R, Vbefore, t=0, delt = 0.001;
    
    Vbefore = V_awal;
    fprintf (v_dc, "%f,%d,%f,%f,%f\n", Vbefore, Vs, I_C, I_R, t);
    for (t = 0; t <= t_akhir ; t += delt){
        V_C = (Vs*(delt)+(R*C*Vbefore))/((R*C) + delt);
        I_C = C*(V_C-Vbefore)/delt;
        Vbefore = V_C;
        V_R = Vs-Vbefore;
        I_R = V_R/R;
        fprintf (v_dc, "%f,%f,%f,%f,%f\n", V_C, V_R, I_C, I_R, t);
    }

    fclose(v_dc);

    return(0);
}