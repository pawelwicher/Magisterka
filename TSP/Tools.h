#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <conio.h>
#include <string.h>
#include <windows.h>
#include <vector>

#ifndef TOOLS
#define TOOLS

using namespace std;

struct Punkt
{
float x;
float y;
};

extern int LICZBA_MIAST;
extern int ROZMIAR_POPULACJI;
extern Punkt *XY;
extern float **Trasy;
extern char sciezka[MAX_PATH];

string NazwaProblemu(char[]);
int N();
void WczytajPunkty();
Punkt MaxXY();
float Odleglosc(Punkt&,Punkt&);
void WypelnijTrasy();


class Zbior
{
public:
vector<int>tab;

Zbior(){}
Zbior(int dl){for(int i=1;i<=dl;i++) tab.push_back(i);}

int ZwrocLosowy()
{
int los=rand()%tab.size(),tmp=tab[los];
tab[los]=tab[tab.size()-1];
tab.pop_back();
return tmp;
}

void DodajK(int liczba){tab.push_back(liczba);}

void DodajP(int liczba){tab.push_back(0); for(int i=tab.size()-1;i>0;i--) tab[i]=tab[i-1]; tab[0]=liczba;}

bool CzyJest(int liczba)
{
bool jest=false;
for(int i=0;i<tab.size();i++)
if(tab[i]==liczba){jest=true; break;}

return jest;
}

int Indeks(int liczba)
{
int nr=-1;
for(int i=0;i<tab.size();i++)
if(tab[i]==liczba){nr=i; break;}

return nr;
}

void Usun(int liczba)
{
int nr=Indeks(liczba);

if(nr!=-1)
{
tab[nr]=tab[tab.size()-1];
tab.pop_back();
}
}

void Drukuj()
{
for(int i=0;i<tab.size();i++)
cout<<tab[i]<<" ";
cout<<"\n";
}

};

#endif
