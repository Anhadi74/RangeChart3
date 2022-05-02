//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	Chart = new TRangeChart(this);
	Chart->Parent = this;
	Chart->Left = 5;
    Chart->Top = 5;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
	int s = Chart->Add("Test grahic", 512);
	uint16_t arr[512];
	uint16_t j=0;
	for (int i = 0; i < 512; i++) {
		//if (!(i%32)) j+=10;
		//arr[i] = j;
		j = j + random(100) - 30;
		arr[i] = j;
	}
	//Chart->BeginUpdate();
	Chart->Series[s]->CopyBuffer(arr);
	Chart->Series[s]->Color = clRed;

	//Chart->OnMouseDown = FormMouseDown;
	//Chart->MaxYValue = 300;
	//Chart->EndUpdate();

	//Caption = int(psClear);
    //Chart->Delete(0);
	//Chart->get
	//Chart->Canvas->Pen->Color = clBlack;
	/*TButton* Btn = new TButton(this);
	Btn->Parent = this;
	Btn->Left = 100;
    Btn->Caption = "dfsfs";*/
	//Button1->tbu
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
	//Chart->MaxXValue = 1024;
	//Chart->Series[0]->Log = !Chart->Series[0]->Log;
	Chart->Series[0]->Visible = not Chart->Series[0]->Visible;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
    //Chart->Width = 600;
	Chart->Series[0]->Log = !Chart->Series[0]->Log;
}
//---------------------------------------------------------------------------


