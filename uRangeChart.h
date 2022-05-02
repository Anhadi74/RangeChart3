//---------------------------------------------------------------------------
//	������ 0.3 �� 02.05.2022�.
//  ������������ ��������������� �����
//	���������� �������������������
//  ���������� ����� ��������� ����������
//  ����� ����������� �� �����, ���������� ������������ ��������� ����
//  �������� ���������� ����������� ������
//---------------------------------------------------------------------------
#ifndef uRangeChartH
#define uRangeChartH
#include <Vcl.Controls.hpp>
#include <Vcl.Graphics.hpp>
#include <Windows.hpp>
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------

class TRangeSeries : public TObject
{
protected:
	uint16_t*  		FBuffer = nullptr;	// ����� ������
	AnsiString  	FCaption;			// ������� �����
	TColor      	FColor = clBlack; 	// ���� �����
	uint16_t       	FCount = 0; 		// ���������� ��������� � ������
	bool            FLog = false;       // ���� - ��������������� �����
	bool            FOverflow = false;  // ���� - ������������ �� ��� � (�������� � �� ����)
	TPoint*     	FPoints = nullptr;	// ������ �����
	TCustomControl* FRangeChart;        // ������������ ���������
	TRect           FRect;              // ������� ����������
	float         	FStepX = 1; 		// ��� �� X
	float         	FStepY = 1; 		// ��� �� Y
	bool            FVisible = true;    // ���� - ���������
	void __fastcall Calc(); // ��������������� ������ ����� TPoint
	uint16_t __fastcall GetValue(uint16_t Index);
	void __fastcall SetColor(TColor Value);
	void __fastcall SetLog(bool Value);
	void __fastcall SetValue(uint16_t Index, uint16_t Value);  // ��������� �������� ����� ����� ??? �������� � �� ����
	void __fastcall SetVisible(bool Value);
public:
	__property AnsiString Caption = {read = FCaption};
	__property uint16_t Count = {read = FCount};
	__property TColor Color = {read = FColor, write = SetColor};
	__property bool Log = {read = FLog, write = SetLog};
	__property bool Overflow = {read = FOverflow};
	__property uint16_t Values[uint16_t Index] = {read = GetValue, write = SetValue};
	__property bool Visible = {read = FVisible, write = SetVisible};
	void __fastcall CopyBuffer(uint16_t* Buf);
	void __fastcall Paint(TCanvas* Canvas);
	void __fastcall SetParams(float StepX, float StepY, TRect Rect); // �������� �������� �������� ����������
	__fastcall TRangeSeries(TCustomControl* RangeChart, AnsiString Caption,
		uint16_t Count);
	__fastcall ~TRangeSeries();
};

class TBaseRangeChart : public TCustomControl
{
__published:
	__property OnMouseDown;  // ����������� - ��������� ������� ��������� �������
protected:
	void __fastcall SetBounds(int Left, int Top, int Width, int Height) override;
protected:
	bool 		FAutoScale = false; // ���� - ������������������� �� ��� � (�������� � �� ����)
	AnsiString 	FCaption = ""; // ������� ������ ������
	TList*   	FList;  	// ������ ������
	int         FItemIndex = -1; // ������ ������� ������
	uint16_t 	FMaxXValue; // ������������ �������� �� ��� �
	uint32_t 	FMaxYValue; // ������������ �������� �� ��� �
	float	 	FStepX;     // ��� �� ��� �
	float    	FStepY;     // ��� �� ��� �
	TRect    	FRect; 		// ������� �������
	bool        FOutputBlocking = true; // ���������� ������
	void __fastcall SetAutoScale(bool Value);
	int __fastcall GetCount() {return FList->Count;};
	void __fastcall SetItemIndex(int Value);
    void __fastcall SetMaxXValue(uint32_t Value);
	void __fastcall SetMaxYValue(uint32_t Value);
	TRangeSeries* __fastcall GetSeries(unsigned int Index);
	void __fastcall SetParamsSeries();
public:
	__property bool AutoScale = {read = FAutoScale, write = SetAutoScale};
	__property int Count = {read = GetCount};
	__property int ItemIndex = {read = FItemIndex, write = SetItemIndex};
	__property uint32_t MaxXValue = {read = FMaxXValue, write = SetMaxXValue};
	__property uint32_t MaxYValue = {read = FMaxYValue, write = SetMaxYValue};
	__property TRangeSeries* Series[unsigned int Index] = {read = GetSeries};
	int __fastcall Add(AnsiString Caption, uint16_t Count);
	void __fastcall BeginUpdate() {FOutputBlocking = true;}; // ������ ������ �� ����� ������ � ������� � �.�.
	void __fastcall Clear();
	void __fastcall CreateWnd() override;  // ����� �������� Wnd ��� ����������� ����������� ������
	void __fastcall Delete(unsigned int Index);
	void __fastcall DestroyWnd() override; // ����� ����������� �����
	void __fastcall EndUpdate() {FOutputBlocking = false;}; // ���������� ������
	void __fastcall Paint() override; // ����� �������

	__fastcall TBaseRangeChart(TComponent* Owner) override;
	__fastcall ~TBaseRangeChart();
};

class TRangeChart : public TBaseRangeChart
{
__published:

protected:
	TBitBtn*	FDownBtn;  		// ������ �������� �� ��� � - ����
	TBitBtn*	FUpBtn;         // ������ �������� �� ��� � - �����
	TCheckBox*  FCheckBoxLog;   // ���� ��������������� �����
	int         FPosX;          // ������� �������
	void __fastcall DownBtnClick(TObject *Sender);
	void __fastcall MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall PositionTracking();
	void __fastcall UpBtnClick(TObject *Sender);
public:
	void __fastcall Paint() override; // ���������� ������ � ��������
	__fastcall TRangeChart(TComponent* Owner) override;
	__fastcall ~TRangeChart();
};



#endif
