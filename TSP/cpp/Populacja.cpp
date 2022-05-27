#include "Populacja.h"
/*******************************************************************************
******************************** KONSTRUKTOR ***********************************
*******************************************************************************/

Populacja::Populacja()
{
N=LICZBA_MIAST;
losob=ROZMIAR_POPULACJI;
bestlen=worstlen=fitsum=0;
XY=new Punkt[N];
Trasy=new float*[N];
for(int i=0;i<N;i++) Trasy[i]=new float[N];
pop=new Chromosom[losob];
pop1=new Chromosom[losob];
fit=new float[losob];
dl=new float[losob];
prob=new float[losob];

for(int i=0;i<losob;i++) fit[i]=dl[i]=prob[i]=0;
}

/*******************************************************************************
********************************** DESTRUKTOR **********************************
*******************************************************************************/

Populacja::~Populacja()
{
delete []pop;
delete []pop1;
delete []dl;
delete []fit;
delete []prob;
for(int i=0;i<N;i++) delete[] Trasy[i];
delete[] Trasy;
delete []XY;
}

/*******************************************************************************
****************************** INICJUJ POPULACJE *******************************
*******************************************************************************/

void Populacja::InicjujPopulacje()
{
Zbior* Z;
WczytajPunkty();
WypelnijTrasy();

  for(int i=0;i<losob;i++)
  {
  Z=new Zbior(N);
    for(int j=0;j<N;j++)
      pop[i].ch[j]=Z->ZwrocLosowy();
  delete Z;
  }

}

/*******************************************************************************
**************************** INICJUJ POPULACJE NN ******************************
*******************************************************************************/

void Populacja::InicjujPopulacjeNN()
{
Chromosom buf;
float **T;
float max,min;
int min_j=0,wiersz=0;

WczytajPunkty();
WypelnijTrasy();

    T=new float*[N];
    for(int i=0;i<N;i++) T[i]=new float[N];

    for(int i=0;i<LICZBA_MIAST;i++)
        for(int j=0;j<LICZBA_MIAST;j++)
            T[i][j]=Trasy[i][j];


    for(int i=0;i<N;i++)
    {
    buf.ch[i]=wiersz+1;                //tworzy trase wg heurystyki Najblizszego Sasiada
    
        for(int j=0;j<N;j++)
        if(max<T[wiersz][j]) max=T[wiersz][j];
        
        min=max;
        
        for(int j=0;j<N;j++)
        if(min>T[wiersz][j] && T[wiersz][j]>0) {min=T[wiersz][j]; min_j=j;}
        
        for(int j=0;j<N;j++)
        {T[wiersz][j]=0; T[j][wiersz]=0;}
        
        wiersz=min_j;
    }
    
    for(int i=0;i<N;i++) delete[] T[i];
    delete[] T;
    
    for(int j=0;j<N;j++)
    pop[0].ch[j]=buf.ch[j];


    for(int i=1;i<losob;i++)           //wypelnia populacje bazowa trasami utworzonymi za pomaca NN
        for(int j=0;j<N;j++)
            if(pop[i-1].ch[j]==N) pop[i].ch[j]=1;
            else pop[i].ch[j]=pop[i-1].ch[j]+1;
    
}

/*******************************************************************************
******************************* OCENA POPULACJI ********************************
*******************************************************************************/

void Populacja::OcenaPopulacji()
{
bestlen=worstlen=fitsum=0;

    for(int i=0;i<losob;i++)
    {
    dl[i]=pop[i].PoliczDroge();                  //wypelnia wektor dlugosci drogi
    if(dl[i]>worstlen) worstlen=dl[i];           //wyznacza najdluzsza droge
    }
    
    bestlen=worstlen;
    
    for(int i=0;i<losob;i++)
        if(dl[i]<bestlen)
        {
        bestlen=dl[i];                           //wyznacza najkrotsza droge
        for(int j=0;j<N;j++)
        Best.ch[j]=pop[i].ch[j];                 //zachowuje najlepszego osobnika
        }

    for(int i=0;i<losob;i++)
      fit[i]=worstlen/dl[i];                     //wypelnia wektor dostosowan



    for(int i=0;i<losob;i++)
    {
    fit[i]=pow((double)fit[i],(double)1.01);        //skalowanie dostosowania
    fitsum+=fit[i];                              //wyznacza sume dostosowan przeskalowanych
    }


    for(int i=0;i<losob;i++)
    prob[i]=fit[i]/fitsum;                       //dostosowanie wzgledne, czyli prawdopodobienstwa
      
    for(int i=1;i<losob;i++)
    prob[i]+=prob[i-1];                          //wyznacza dystrybuante    
}

/*******************************************************************************
******************************** REPRODUKCJA ***********************************
*******************************************************************************/

void Populacja::Reprodukcja()
{
float los;

    for(int i=0;i<losob;i++)
    {
    los=(float)(rand()%1000)/1000;
    for(int j=0;j<losob;j++)
        if(los<prob[j])
        {
        for(int k=0;k<N;k++) pop1[i].ch[k]=pop[j].ch[k];
        break;
        }
    }
}

/*******************************************************************************
***************************** OPERACJE GENETYCZNE ******************************
*******************************************************************************/

void Populacja::OperacjeGenetyczne()
{
int r1,r2,iworst=0;
Chromosom p1,p2;
float worstlen=0,tmp;

    if(k==1 || k==2 ||k==3)
        for(int i=0;i<losob;i+=2)
        {
        r1=rand()%losob;
        r2=rand()%losob;

            if((float)(rand()%1000)/1000<pk)
            {
            if(k==1)
              OX(pop1[r1],pop1[r2],p1,p2);
            if(k==2)
              PMX(pop1[r1],pop1[r2],p1,p2);
            if(k==3)
              CX(pop1[r1],pop1[r2],p1,p2);
            for(int j=0;j<N;j++)
                {
                pop[i].ch[j]=p1.ch[j];
                pop[i+1].ch[j]=p2.ch[j];
                }
            }
            else
            {
                for(int j=0;j<N;j++)
                {
                pop[i].ch[j]=pop1[r1].ch[j];
                pop[i+1].ch[j]=pop1[r2].ch[j];
                }
            }
        }

    if(k==4)
        for(int i=0;i<losob;i++)
        {
        r1=rand()%losob;
        r2=rand()%losob;

            if((float)(rand()%1000)/1000<pk)
            {
            GSX(pop1[r1],pop1[r2],p1);
                for(int j=0;j<N;j++)
                    pop[i].ch[j]=p1.ch[j];
            }
            else
                for(int j=0;j<N;j++)
                pop[i].ch[j]=pop1[r1].ch[j];
        }

    for(int i=0;i<losob;i++)
        if((float)(rand()%1000)/1000<popt) OPT(pop[i]);

    for(int i=0;i<losob;i++)
        if((float)(rand()%1000)/1000<pm)
        {
            if(m==1) Mutacja1(pop[i]);
            if(m==2) Mutacja2(pop[i]);
            if(m==3) Mutacja3(pop[i]);
        }

    for(int i=0;i<losob;i++)
        {
        tmp=pop[i].PoliczDroge();
        if(worstlen<tmp) {worstlen=tmp; iworst=i;}
        }

    for(int i=0;i<N;i++)            //selekcja elitarna - najlepszy osobnik zostaje w populacji (w miejsce najgorszego)
        pop[iworst].ch[i]=Best.ch[i];



}

/*******************************************************************************
******************************* KRZYZOWANIE OX *********************************
*******************************************************************************/

void Populacja::OX(Chromosom &r1,Chromosom &r2,Chromosom &p1,Chromosom &p2)
{
int c1,c2; //punkty ciecia
int i,j,k=0,l=0;
bool p=true,q=true;
Chromosom buf1,buf2,buf3,buf4;

    c1=1+rand()%N;
    c2=c1+rand()%(N-c1+1);
    
    for(i=c1-1;i<c2;i++) {p1.ch[i]=r1.ch[i]; p2.ch[i]=r2.ch[i];}
    //przepisanie trasy miedzy punktami ciecia        
    
    for(i=0;i<N;i++)
    {
        for(j=c1-1;j<c2;j++)
            {
            if(p1.ch[j]==r2.ch[i]) p=false;
            if(p2.ch[j]==r1.ch[i]) q=false;
            }        
    if(p==true) buf1.ch[i]=r2.ch[i];
    if(q==true) buf3.ch[i]=r1.ch[i];
    p=true;
    q=true;
    }
    
    for(i=c2;i<N;i++)
        {
        if(buf1.ch[i]!=0) {buf2.ch[k]=buf1.ch[i]; k++;}
        if(buf3.ch[i]!=0) {buf4.ch[l]=buf3.ch[i]; l++;}
        }
    for(i=0;i<c2;i++)
        {
        if(buf1.ch[i]!=0) {buf2.ch[k]=buf1.ch[i]; k++;}
        if(buf3.ch[i]!=0) {buf4.ch[l]=buf3.ch[i]; l++;}
        }
    
    k=l=0;
    
    for(i=c2;i<N;i++) {p1.ch[i]=buf2.ch[k]; k++; p2.ch[i]=buf4.ch[l]; l++;}
    for(i=0;i<c1-1;i++) {p1.ch[i]=buf2.ch[k]; k++; p2.ch[i]=buf4.ch[l]; l++;}
}

/*******************************************************************************
****************************** KRZYZOWANIE PMX *********************************
*******************************************************************************/

void Populacja::PMX(Chromosom &r1,Chromosom &r2,Chromosom &p1,Chromosom &p2)
{
int c1,c2,liczba,pozycja;

    c1=1+rand()%N;
    c2=c1+rand()%(N-c1+1);
       
    for(int i=0;i<N;i++)
        p1.ch[i]=p2.ch[i]=0;
        
    for(int i=c1;i<c2;i++)
        {
        p1.ch[i]=r2.ch[i];
        p2.ch[i]=r1.ch[i];
        }

    for(int i=0;i<c1;i++)
        if(Znajdz(p1,r1.ch[i])==-1)
            p1.ch[i]=r1.ch[i];
        
    for(int i=c2;i<N;i++)
        if(Znajdz(p1,r1.ch[i])==-1)
            p1.ch[i]=r1.ch[i];
    
    for(int i=0;i<c1;i++)
        if(Znajdz(p2,r2.ch[i])==-1)
            p2.ch[i]=r2.ch[i];
    
    for(int i=c2;i<N;i++)
        if(Znajdz(p2,r2.ch[i])==-1)
            p2.ch[i]=r2.ch[i];
    
    for(int i=0;i<N;i++)
        if(p1.ch[i]==0)
            {
            liczba=r1.ch[i];
            do
                {
                pozycja=Znajdz(p1,liczba);
                liczba=p2.ch[pozycja];
                }
            while(Znajdz(p1,liczba)!=-1);
            p1.ch[i]=liczba;
            }
        
    for(int i=0;i<N;i++)
        if(p2.ch[i]==0)
            {
            liczba=r2.ch[i];
            do
                {
                pozycja=Znajdz(p2,liczba);
                liczba=p1.ch[pozycja];
                }
            while(Znajdz(p2,liczba)!=-1);
            p2.ch[i]=liczba;
            }
}

/*******************************************************************************
****************************** KRZYZOWANIE CX **********************************
*******************************************************************************/

void Populacja::CX(Chromosom &r1,Chromosom &r2,Chromosom &p1,Chromosom &p2)
{
int k,pozycja;

    for(int i=0;i<N;i++)
        p1.ch[i]=p2.ch[i]=0;
        
    p1.ch[0]=r1.ch[0];
    k=0;
    while(true)
        {
        pozycja=Znajdz(r1,r2.ch[k]);
        if(p1.ch[pozycja]!=0) break;
        p1.ch[pozycja]=r1.ch[pozycja];
        k=pozycja;
        }

    for(int i=0;i<N;i++)
        if(p1.ch[i]==0)
            p1.ch[i]=r2.ch[i];
                    
    p2.ch[0]=r2.ch[0];
    k=0;
    while(true)
        {
        pozycja=Znajdz(r2,r1.ch[k]);
        if(p2.ch[pozycja]!=0) break;
        p2.ch[pozycja]=r2.ch[pozycja];
        k=pozycja;
        }

    for(int i=0;i<N;i++)
        if(p2.ch[i]==0)
            p2.ch[i]=r1.ch[i];
}

/*******************************************************************************
****************************** KRZYZOWANIE GSX *********************************
*******************************************************************************/

void Populacja::GSX(Chromosom &r1,Chromosom &r2,Chromosom &p)
{

Zbior Z,TMP;
bool fa=true,fb=true;
int t=1+rand()%N; cout<<t<<"\n\n";
int x,y;

for(int i=0;i<N;i++)
if(r1.ch[i]==t) {x=i; break;}

for(int i=0;i<N;i++)
if(r2.ch[i]==t) {y=i; break;}

cout<<x<<"\n";
cout<<y<<"\n\n";

Z.DodajP(t);

  do
  {
  x=abs((x-1)%N);
  y=abs((y+1)%N);


    if(fa==true)
      if(!Z.CzyJest(r1.ch[x])) Z.DodajP(r1.ch[x]); else fa=false;

    if(fb==true)
      if(!Z.CzyJest(r2.ch[y])) Z.DodajK(r2.ch[y]); else fb=false;
  }
  while(fa==true || fb==true);

  int k=0;

  if(Z.tab.size()<N)
  {
  for(int i=1;i<=N;i++)
  if(!Z.CzyJest(i)) {TMP.DodajK(i); k++;}

  for(int i=0;i<k;i++)
  Z.DodajK(TMP.ZwrocLosowy());
  }

for(int i=0;i<N;i++)
p.ch[i]=Z.tab[i];

}

/*******************************************************************************
********************************** MUTACJA1 ************************************
*******************************************************************************/

void Populacja::Mutacja1(Chromosom &Ch)
{
int c1,c2,k=0;                        //geny do zamiany
int buf;
int *tmp;

    c1=1+rand()%(N-1);
    c2=1+c1+rand()%(N-c1);

    c1--;
    c2--;

    tmp=new int[c2-c1];

    for(int i=c1;i<c2;i++)
    {tmp[k]=Ch.ch[i]; k++;}

    k--;

    for(int i=c1;i<c2;i++)
    {Ch.ch[i]=tmp[k]; k--;}

    delete tmp;
}

/*******************************************************************************
********************************** MUTACJA2 ************************************
*******************************************************************************/

void Populacja::Mutacja2(Chromosom &Ch)
{
int c1,c2;                        //geny do zamiany
int buf;

    c1=1+rand()%(N-1);
    c2=1+c1+rand()%(N-c1);

    c1--;
    c2--;

    buf=Ch.ch[c1];
    Ch.ch[c1]=Ch.ch[c2];
    Ch.ch[c2]=buf;
}

/*******************************************************************************
********************************** MUTACJA3 ************************************
*******************************************************************************/

void Populacja::Mutacja3(Chromosom &Ch)
{
int c1,c2;                        //geny do zamiany
int buf;
Zbior Z;
        c1=1+rand()%(N-1);
        c2=1+c1+rand()%(N-c1);

        c1--;
        c2--;


        for(int i=c1;i<=c2;i++)
        Z.DodajK(Ch.ch[i]);

        for(int i=c1;i<=c2;i++)
        Ch.ch[i]=Z.ZwrocLosowy();


}


/*******************************************************************************
*********************************** 2OPT ***************************************
*******************************************************************************/

void Populacja::OPT(Chromosom &Ch)
{
bool p=false;
int tmp;
    for(int k=0;k<N && !p; k++)
    {
    p=true;
        for(int i=0;i<N;i++)
        {
            for(int j=i+2;j<N; j++)
                if(Trasy[Ch.ch[i]-1][Ch.ch[(i+1)%N]-1]+Trasy[Ch.ch[j]-1][Ch.ch[(j+1)%N]-1]>
                Trasy[Ch.ch[i]-1][Ch.ch[j]-1]+Trasy[Ch.ch[(i+1)%N]-1][Ch.ch[(j+1)%N]-1])
                {
                tmp=Ch.ch[(i+1)%N];
                Ch.ch[(i+1)%N]=Ch.ch[j];
                Ch.ch[j]=tmp;
                Odwroc(Ch,i+2,j-1);
                p=false;
                }
       }
    }
}

/*******************************************************************************
********************************** ODWROC **************************************
*******************************************************************************/

void Populacja::Odwroc(Chromosom &Ch,int start,int stop)
{
    if(start>=stop || start>=N || stop<0)
    return;

    for(;start<stop;stop--)
    {
    int tmp=Ch.ch[start];
    Ch.ch[start]=Ch.ch[stop];
    Ch.ch[stop]=tmp;
    start++;
    }
}

/*******************************************************************************
********************************** ZNAJDZ **************************************
*******************************************************************************/

int Populacja::Znajdz(Chromosom &c,int liczba)
{
int i;

    for(i=0;i<N;i++)
    if(c.ch[i]==liczba)
        break;

    if(i==N)
        return -1;
    else
        return i;
}

/*******************************************************************************
********************************************************************************
*******************************************************************************/
