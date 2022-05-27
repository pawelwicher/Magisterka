//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include "CGAUGES.h"
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TMainMenu *MainMenu1;
        TGroupBox *GroupBox1;
        TCGauge *CGauge1;
        TSpeedButton *SpeedButton1;
        TSpeedButton *SpeedButton2;
        TStatusBar *StatusBar1;
        TShape *Shape1;
        TPanel *Panel1;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TUpDown *UpDown1;
        TUpDown *UpDown2;
        TUpDown *UpDown3;
        TUpDown *UpDown4;
        TUpDown *UpDown5;
        TOpenDialog *OpenDialog1;
        TImage *Image1;
        TMenuItem *Otwrz1;
        TMenuItem *Info1;
        TComboBox *ComboBox1;
        TComboBox *ComboBox2;
        TLabel *Label6;
        TLabel *Label7;
        TSpeedButton *SpeedButton3;
        TLabel *Label8;
        TComboBox *ComboBox3;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall Otwrz1Click(TObject *Sender);
        void __fastcall SpeedButton1Click(TObject *Sender);
        void __fastcall SpeedButton2Click(TObject *Sender);
        void __fastcall UpDown1Click(TObject *Sender, TUDBtnType Button);
        void __fastcall UpDown2Click(TObject *Sender, TUDBtnType Button);
        void __fastcall UpDown3Click(TObject *Sender, TUDBtnType Button);
        void __fastcall UpDown4Click(TObject *Sender, TUDBtnType Button);
        void __fastcall UpDown5Click(TObject *Sender, TUDBtnType Button);
        void __fastcall SpeedButton3Click(TObject *Sender);
        void __fastcall Info1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
 