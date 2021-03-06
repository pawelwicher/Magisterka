#include "Tools.h"

int LICZBA_MIAST;
int ROZMIAR_POPULACJI;
Punkt *XY;
float **Trasy;
char sciezka[MAX_PATH];

/*******************************************************************************
****************************** NAZWA PROBLEMU **********************************
*******************************************************************************/

string NazwaProblemu(char NazwaPliku[])
{
string napis,buf=NazwaPliku;

string::size_type loc=buf.find("trasy",0);
for(int i=(int)loc+6;i<buf.length()-4;i++)
napis+=buf[i];

return napis; 
}

/*******************************************************************************
************************************** N ***************************************
*******************************************************************************/

int N()
{
FILE *f=fopen(sciezka,"rt");
int n=0,nr;
float a,b;
    for(int i=0;!feof(f);i++)
    {
    fscanf(f,"%f%f%f",&nr,&a,&b);
    n++;
    }
fclose(f);
return n;   
}

/*******************************************************************************
******************************** WCZYTAJ PUNKTY ********************************
*******************************************************************************/

void WczytajPunkty()
{
FILE *f=fopen(sciezka,"rt");
float nr;
    for(int i=0;!feof(f) && i<LICZBA_MIAST;i++)
    fscanf(f,"%f%f%f",&nr,&XY[i].x,&XY[i].y);
fclose(f);   
}

/*******************************************************************************
************************************ MaxXY ************************************
*******************************************************************************/

Punkt MaxXY()
{
Punkt P;
float maxX=0,maxY=0;

    for(int i=0;i<LICZBA_MIAST;i++)
    {
    if(XY[i].x>maxX) maxX=XY[i].x;
    if(XY[i].y>maxY) maxY=XY[i].y;
    }
P.x=maxX;
P.y=maxY;

return P;
}

/*******************************************************************************
*********************************** ODLEGLOSC **********************************
*******************************************************************************/

float Odleglosc(Punkt &P1,Punkt &P2)
{return sqrt((P1.x-P2.x)*(P1.x-P2.x)+(P1.y-P2.y)*(P1.y-P2.y));}

/*******************************************************************************
******************************** WYPELNIJ TRASY ********************************
*******************************************************************************/

void WypelnijTrasy()
{     
for(int i=0;i<LICZBA_MIAST;i++)
    for(int j=0;j<LICZBA_MIAST;j++)
        Trasy[i][j]=Odleglosc(XY[i],XY[j]);
}
