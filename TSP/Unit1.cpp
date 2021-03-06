//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Populacja.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CGAUGES"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

bool STOP,PAUZA;
void Rysuj(TImage*,Populacja*);

void __fastcall TForm1::FormCreate(TObject *Sender)
{
DoubleBuffered=true;
StatusBar1->Panels->Items[0]->Text="Otw?rz plik z danymi";
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Otwrz1Click(TObject *Sender)
{
OpenDialog1->Execute();
strcpy(sciezka,OpenDialog1->FileName.c_str());

  if(strlen(sciezka)==0)
  StatusBar1->Panels->Items[0]->Text="Nie wybrano pliku...";
  else
  {
  StatusBar1->Panels->Items[0]->Text=NazwaProblemu(sciezka).c_str();
  StatusBar1->Panels->Items[1]->Text="";
  SpeedButton1->Enabled=true;
  UpDown1->Enabled=true;
  UpDown2->Enabled=true;
  UpDown3->Enabled=true;
  UpDown4->Enabled=true;
  UpDown5->Enabled=true;
  ComboBox1->Enabled=true;
  ComboBox2->Enabled=true;
  ComboBox3->Enabled=true;
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SpeedButton1Click(TObject *Sender)
{
STOP=false;

SpeedButton1->Enabled=false;
SpeedButton2->Enabled=true;
SpeedButton3->Enabled=true;
UpDown1->Enabled=false;
UpDown2->Enabled=false;
UpDown3->Enabled=false;
UpDown4->Enabled=false;
UpDown5->Enabled=false;
ComboBox1->Enabled=false;
ComboBox2->Enabled=false;
ComboBox3->Enabled=false;

LICZBA_MIAST=N();
ROZMIAR_POPULACJI=UpDown1->Position;
int liczba_iteracji=UpDown5->Position;

srand(time(0));

Populacja *POP=new Populacja();
if(ComboBox3->Text=="NN") POP->InicjujPopulacjeNN();
if(ComboBox3->Text=="Losowo") POP->InicjujPopulacje();

POP->pk=(float)UpDown2->Position/100;
POP->pm=(float)UpDown3->Position/100;
POP->popt=(float)UpDown4->Position/100;

if(ComboBox1->Text=="OX") POP->k=1;
if(ComboBox1->Text=="PMX") POP->k=2;
if(ComboBox1->Text=="CX") POP->k=3;
if(ComboBox1->Text=="GSX") POP->k=4;

if(ComboBox2->Text=="Inwersja") POP->m=1;
if(ComboBox2->Text=="Zamiana") POP->m=2;
if(ComboBox2->Text=="Perturbacja") POP->m=3;

float droga,droga1=0;
FILE* f;
FILE* f1;
f=fopen("RAPORT.txt","w");
f1=fopen("LOG.txt","a");

for(int i=1;i<=liczba_iteracji;i++)
{
POP->OcenaPopulacji();
POP->Reprodukcja();
POP->OperacjeGenetyczne();

if(i%100==0) Rysuj(Image1,POP);
CGauge1->Progress =(int)(100*i/liczba_iteracji);
droga=POP->Best.PoliczDroge();
if(droga1!=droga){droga1=droga;
fprintf(f,"%d ; %f\n",i,droga);}
StatusBar1->Panels->Items[1]->Text=droga;
Application->ProcessMessages();

while(PAUZA) {Application->ProcessMessages();}


if(STOP) break;
}

AnsiString krzyzowanie="";
switch(POP->k)
{
case 1: krzyzowanie="OX"; break;
case 2: krzyzowanie="PMX"; break;
case 3: krzyzowanie="CX"; break;
case 4: krzyzowanie="GSX"; break;
}

AnsiString mutacja="";
switch(POP->m)
{
case 1: mutacja="Inwersja"; break;
case 2: mutacja="Zamiana"; break;
case 3: mutacja="Perturbacja"; break;
}
AnsiString log="";
log+=AnsiString(NazwaProblemu(sciezka).c_str())+"; ";
log+=AnsiString(ROZMIAR_POPULACJI)+"; ";
log+=ComboBox3->Text+"; ";
log+=AnsiString(liczba_iteracji)+"; ";
log+=ComboBox1->Text+"; "+AnsiString(UpDown2->Position)+"; ";
log+=ComboBox2->Text+"; "+AnsiString(UpDown3->Position)+"; ";
log+="2OPT; "+AnsiString(UpDown4->Position)+"; ";
log+="DROGA; "+AnsiString(droga)+"\n";


fprintf(f1,"%s",log.c_str());
fclose (f);
fclose (f1);
Rysuj(Image1,POP);
delete POP;


SpeedButton1->Enabled=true;
SpeedButton2->Enabled=false;
SpeedButton3->Enabled=false;
UpDown1->Enabled=true;
UpDown2->Enabled=true;
UpDown3->Enabled=true;
UpDown4->Enabled=true;
UpDown5->Enabled=true;
ComboBox1->Enabled=true;
ComboBox2->Enabled=true;
ComboBox3->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SpeedButton2Click(TObject *Sender)
{
PAUZA=false;
STOP=true;
StatusBar1->Panels->Items[2]->Text="";
CGauge1->Progress =0;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::UpDown1Click(TObject *Sender, TUDBtnType Button)
{
Label1->Caption="Populacja: "+AnsiString(UpDown1->Position);         
}
//---------------------------------------------------------------------------
void __fastcall TForm1::UpDown2Click(TObject *Sender, TUDBtnType Button)
{
Label2->Caption="Pk: "+AnsiString(UpDown2->Position)+"%";
}
//---------------------------------------------------------------------------
void __fastcall TForm1::UpDown3Click(TObject *Sender, TUDBtnType Button)
{
Label3->Caption="Pm: "+AnsiString(UpDown3->Position)+"%";
}
//---------------------------------------------------------------------------
void __fastcall TForm1::UpDown4Click(TObject *Sender, TUDBtnType Button)
{
Label4->Caption="Popt: "+AnsiString(UpDown4->Position)+"%";
}
//---------------------------------------------------------------------------
void __fastcall TForm1::UpDown5Click(TObject *Sender, TUDBtnType Button)
{
Label5->Caption="Iteracje: "+AnsiString(UpDown5->Position);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::SpeedButton3Click(TObject *Sender)
{
if(PAUZA) {PAUZA=false; StatusBar1->Panels->Items[2]->Text="";}
else {PAUZA=true; StatusBar1->Panels->Items[2]->Text="PAUZA";}
}
//---------------------------------------------------------------------------

void Rysuj(TImage* Image1,Populacja* POP)
{
int skalaX=655;
int skalaY=461;
int przesuniecie=10;
int x1,x2,y1,y2;
int a,b;
Punkt P=MaxXY();

    TRect Rect(0,0,685,491);
    Image1->Canvas->FillRect(Rect);

    for(int i=0;i<LICZBA_MIAST-1;i++)
    {
    a=POP->Best.ch[i]-1;
    b=POP->Best.ch[i+1]-1;
    x1=przesuniecie+(int)(XY[b].x/P.x*skalaX);
    y1=przesuniecie+(int)(XY[b].y/P.y*skalaY);
    x2=przesuniecie+(int)(XY[a].x/P.x*skalaX);
    y2=przesuniecie+(int)(XY[a].y/P.y*skalaY);
    Image1->Canvas->MoveTo(x1,y1);
    Image1->Canvas->LineTo(x2,y2);
    }

    a=POP->Best.ch[LICZBA_MIAST-1]-1;
    b=POP->Best.ch[0]-1;
    x1=przesuniecie+(int)(XY[b].x/P.x*skalaX);
    y1=przesuniecie+(int)(XY[b].y/P.y*skalaY);
    x2=przesuniecie+(int)(XY[a].x/P.x*skalaX);
    y2=przesuniecie+(int)(XY[a].y/P.y*skalaY);
    Image1->Canvas->MoveTo(x1,y1);
    Image1->Canvas->LineTo(x2,y2);

    Image1->Canvas->Brush->Color=clRed;

    for(int i=0;i<LICZBA_MIAST-1;i++)
    {
    b=POP->Best.ch[i+1]-1;
    x1=przesuniecie+(int)(XY[b].x/P.x*skalaX);
    y1=przesuniecie+(int)(XY[b].y/P.y*skalaY);
    Image1->Canvas->MoveTo(x1,y1);
    Image1->Canvas->Ellipse(x1-3,y1-3,x1+3,y1+3);
    }

    b=POP->Best.ch[0]-1;
    x1=przesuniecie+(int)(XY[b].x/P.x*skalaX);
    y1=przesuniecie+(int)(XY[b].y/P.y*skalaY);
    Image1->Canvas->MoveTo(x1,y1);
    Image1->Canvas->Ellipse(x1-3,y1-3,x1+3,y1+3);

    Image1->Canvas->Brush->Color=clWhite;

}
void __fastcall TForm1::Info1Click(TObject *Sender)
{
  AnsiString info="";
  info+="Program rozwi?zuj?cy problem komiwoja?era\n";
  info+="za pomoc? algorytmu genetycznego\n\n";
  info+="Autor: Pawe? Wicher";
  ShowMessage(info);
}
//---------------------------------------------------------------------------

