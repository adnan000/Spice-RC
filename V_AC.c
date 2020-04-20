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

float Magnitude(float real, float imag);
float Angle(float real, float imag);
float Real(float mag, float angle);
float Imag(float mag, float angle);

kompleks_t CalculateImpedance(float R, float L, float C, int freq);
kompleks_t CalculateCurrent(kompleks_t V, kompleks_t Z);
void CalculateVoltage(float R, float L, float C, int freq, kompleks_t I, kompleks_t* VR, kompleks_t* VL, kompleks_t* VC);

void PrintComplex(kompleks_t kompleks);
void PrintPhasor(kompleks_t kompleks, int freq);
//void SortFrequency(freqresponse_t* freqresponse, int N); //BONUS

int main(){
	float R, L, C; //deklarasi variabel
    int Vmag, nfreq, i;
	kompleks_t V, *I, *Z, VR, VL, VC;
	
	/* silakan ubah format specifier */
	printf("Masukkan nilai resistansi (Ohm) : "); scanf("%f", &R);
	printf("Masukkan nilai induktansi (H) : "); scanf("%f", &L);
	printf("Masukkan nilai kapasitansi (C) : "); scanf("%f", &C);	
	
	printf("Masukkan nilai tegangan (V) : "); scanf("%d", &Vmag);
	
	printf("\n");
	printf("Masukkan jumlah frekuensi : "); scanf("%d", &nfreq);
	
	/*VR = (kompleks_t*) malloc(nfreq *sizeof(kompleks_t));
	VL = (kompleks_t*) malloc(nfreq *sizeof(kompleks_t));
	VC = (kompleks_t*) malloc(nfreq *sizeof(kompleks_t));*/
	I = (kompleks_t*) malloc(nfreq *sizeof(kompleks_t));
	Z = (kompleks_t*) malloc(nfreq *sizeof(kompleks_t));

	freqresponse_t *frq;
	frq = (freqresponse_t*)malloc(nfreq*sizeof(freqresponse_t));//Deklarasi array dinamis

	V.mag = Vmag;
	V.angle = 0;
	V.imag = Imag(V.mag, V.angle);
	V.real = Real(V.mag, V.angle);
	
	for (i = 0 ; i < nfreq ; i ++){
		printf("Masukkan frekuensi %d (rad/s) : ", i + 1);
		scanf("%d", &frq[i].freq);
	}
	printf("\n");
	printf("Simulasi respon frekuensi ...\n");
	
	printf("\n");
	for (i = 0; i < nfreq;i++){
		printf("Frekuensi %d : %d rad/s\n",i + 1,frq[i].freq);
		
		Z[i] = CalculateImpedance(R, L, C, frq[i].freq);
		I[i] = CalculateCurrent(V, Z[i]);
		CalculateVoltage(R, L, C, frq[i].freq, I[i], &VR, &VL, &VC);
		printf("Impedansi rangkaian : "); PrintComplex(Z[i]); printf(" Ohm\n");
		printf("Arus rangkaian : "); PrintPhasor(I[i], frq[i].freq); printf("A\n");
		printf("Tegangan resistor : "); PrintPhasor(VR, frq[i].freq); printf(" V\n");
		printf("Tegangan induktor : "); PrintPhasor(VL, frq[i].freq); printf(" V\n");
		printf("Tegangan kapasitor : "); PrintPhasor(VC, frq[i].freq); printf(" V\n");
		printf("\n");
			
	}
	//SortFrequency(...);
	
	/*printf("Urutan frekuensi : ");
	
		printf("%d ",...);
	
	printf("%d\n",...);*/
	
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

/*
	Urutkan frekuensi menurut besar (magnitude) arus (BONUS)
	Input : array freqresponse, jumlah elemen N
	Output : array freqresponse yang sudah tersort

void SortFrequency(freqresponse_t* freqresponse, int N){
	// your code here
}*/