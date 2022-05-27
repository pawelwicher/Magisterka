#include "Chromosom.h"

#ifndef POPULACJA
#define POPULACJA

class Populacja
{
public:
int losob; //liczba osobnikow
int N; //rozmiar problemu
Chromosom *pop,*pop1; //populacja bazowa,tymczasowa
Chromosom Best; //osobnik o najkrotszej trasie - do selekcji elitarnej
float *dl; //wektor dlugosci dróg
float *fit; //wektor dopasowan
float *prob; //wektor pawdopodobienstwa
float bestlen,worstlen,fitsum;
float pk,pm,popt; //prawdopodobienstwo krzyzowania, mutacji, 2opt
int k; //rodzaj krzyzowania
int m; //rodzaj mutacji

Populacja();
~Populacja();
void InicjujPopulacje();
void InicjujPopulacjeNN();
void OcenaPopulacji();
void Reprodukcja();
void OperacjeGenetyczne();
void OX(Chromosom&,Chromosom&,Chromosom&,Chromosom&);
void PMX(Chromosom&,Chromosom&,Chromosom&,Chromosom&);
void CX(Chromosom&,Chromosom&,Chromosom&,Chromosom&);
void GSX(Chromosom&,Chromosom&,Chromosom&);
void Mutacja1(Chromosom&);
void Mutacja2(Chromosom&);
void Mutacja3(Chromosom&);
void OPT(Chromosom&);
void Odwroc(Chromosom&,int,int);
int Znajdz(Chromosom&,int);
};

#endif
