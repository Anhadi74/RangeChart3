//---------------------------------------------------------------------------

#pragma hdrstop

#include "uRangeChart.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//TRangeSeries***************************************************************
void __fastcall TRangeSeries::Calc()
{
	if (FCount) {
		FOverflow = false;
		int y;
		for (int i = 0; i < FCount; i++) {
			// Определение координаты У
			if (!FLog) {
				y = FRect.Bottom - int(FBuffer[i] * FStepY);
			} else
			{
				y = FRect.Bottom - log(FBuffer[i]) * FStepY;
			}

			if (y < FRect.Top) {
					y = FRect.Top;
					FOverflow = true;
			} else {
				if (y > FRect.Bottom) {
					y = FRect.Bottom;
				}
			}
			FPoints[i].y = y;
			// Определение координаты Х
			FPoints[i].x = FRect.Left + round(i * FStepX);
		}  
	}
}

uint16_t __fastcall TRangeSeries::GetValue(uint16_t Index)
{
	if (Index >= FCount) {
		Index = FCount - 1;
	}
	return FBuffer[Index];
}

void __fastcall TRangeSeries::SetColor(TColor Value)
{
	if (Value != FColor) {
		FColor = Value;
		FRangeChart->Repaint();
	}
}

void __fastcall TRangeSeries::SetLog(bool Value)
{
	if (FLog != Value) {
		FLog = Value;
		Calc();
		FRangeChart->Repaint();
	}
}

void __fastcall TRangeSeries::SetParams(float StepX, float StepY, TRect Rect)
{
	FStepX = StepX;
    FStepY = StepY;
	FRect = Rect;
	Calc();
}

void __fastcall TRangeSeries::SetValue(uint16_t Index, uint16_t Value)
{
    if (Index >= FCount) {
		Index = FCount - 1;
	}
	FBuffer[Index] = Value;
}

void __fastcall TRangeSeries::SetVisible(bool Value)
{
	if (Value != FVisible) {
		FVisible = Value;
		FRangeChart->Repaint();
	}
}

void __fastcall TRangeSeries::CopyBuffer(uint16_t* Buf)
{
	memcpy(FBuffer, Buf, FCount * 2);
	Calc();
	FRangeChart->Repaint();
}

void __fastcall TRangeSeries::Paint(TCanvas* Canvas)
{
	if (FVisible) {
		Canvas->Pen->Color = FColor;
		Canvas->Polyline(FPoints, FCount-1);
	}
}

__fastcall TRangeSeries::TRangeSeries(TCustomControl* RangeChart,
	AnsiString Caption, uint16_t Count)
{
	FRangeChart = RangeChart;
	FPoints = new TPoint[Count];
	FBuffer = new uint16_t[Count];
	FCount = Count;
    FCaption = Caption;
}

__fastcall TRangeSeries::~TRangeSeries()
{
    delete [] FPoints;
	delete [] FBuffer;
}

//TBaseRangeChart************************************************************

void __fastcall TBaseRangeChart::SetAutoScale(bool Value)
{
	if (FAutoScale != Value) {
		FAutoScale = Value;
		/*
		int value = 1;
		int i;
		do
		{
			i = FList->Count;
			while ((i-- > 0) && !((TRangeSeries*)FList->Items[i])->Overflow) {};
			if (i > -1) {
				value = FMaxYValue * 2;
				if (value <= 65535) {
					SetMaxYValue(value);
				}
			}
		} while ((i != -1) && (value < 65535));
		*/
	}
}

int __fastcall TBaseRangeChart::Add(AnsiString Caption, uint16_t Count)
{
	if (FMaxXValue < Count) {
		FMaxXValue = Count;
		SetParamsSeries();
	}
	TRangeSeries* RS = new TRangeSeries(this, Caption, Count);
	int index = FList->Add(RS);
	RS->SetParams(FStepX, FStepY, FRect);
	if (FItemIndex == -1) {
		SetItemIndex(index);
	}
	Paint();
	return index;
}

void __fastcall TBaseRangeChart::Clear()
{
	FItemIndex = -1;
	bool tmp = FOutputBlocking;
	FOutputBlocking = true;
	for (int i = 0; i < FList->Count; i++) {
		Delete(0);
	}
	FOutputBlocking = tmp;
	Paint();
}

void __fastcall TBaseRangeChart::CreateWnd()
{
	TCustomControl::CreateWnd();
	FOutputBlocking = false;
}

void __fastcall TBaseRangeChart::Delete(unsigned int Index)
{
	if (Index < FList->Count) {
		delete (TRangeSeries*)FList->Items[Index];
		FList->Delete(Index);
	}
	Paint();
}

void __fastcall TBaseRangeChart::DestroyWnd()
{
	FOutputBlocking = true;
	TCustomControl::DestroyWnd();
}

TRangeSeries* __fastcall TBaseRangeChart::GetSeries(unsigned int Index)
{
	if (Index < FList->Count) {
		return (TRangeSeries*)FList->Items[Index];
	}
    return nullptr;
}

void __fastcall TBaseRangeChart::SetBounds(int Left, int Top, int Width, int Height)
{
	TCustomControl::SetBounds(Left, Top, Width, Height);
	FRect = Rect(100, 25, Width-10, Height - 25);
	SetParamsSeries();
}

void __fastcall TBaseRangeChart::SetItemIndex(int Value)
{
	if ((Value > -1)&&(FItemIndex != Value)&&(Value < FList->Count)) {
		FItemIndex = Value;
		FCaption = ((TRangeSeries*)FList->Items[Value])->Caption;
	} else {
		FItemIndex = -1;
		FCaption = "";
	}
	Paint();
}

void __fastcall TBaseRangeChart::SetParamsSeries()
{
	FStepX = (float)(FRect.Right - FRect.Left) / FMaxXValue;
	FStepY = (float)(FRect.Bottom - FRect.Top) / FMaxYValue;
	for (int i = 0; i < FList->Count; i++) {
		((TRangeSeries*)FList->Items[i])->SetParams(FStepX, FStepY, FRect);
	}
}

void __fastcall TBaseRangeChart::SetMaxXValue(uint32_t Value)
{
	if (FMaxXValue != Value) {
        int max;
		int tmp;
		// Максимальное значение не может быть меньше кол-ва элементов массивов
		for (int i = 0; i < FList->Count; i++) {
			tmp = ((TRangeSeries*)FList->Items[i])->Count;
			if (max < tmp) {
				max = tmp;
			}
		}
		if (max < Value) {
			FMaxXValue = Value;
			SetParamsSeries();
			Repaint();
		}
	}
}

void __fastcall TBaseRangeChart::SetMaxYValue(uint32_t Value)
{
	if (FMaxYValue != Value) {
		FMaxYValue = Value;
		SetParamsSeries();
		Repaint();
	}
}

void __fastcall TBaseRangeChart::Paint()
{
	if (FOutputBlocking) {
        return;
	}
	TCustomControl::Paint();
	int textWidth;
	int textStep;
	if (FItemIndex > -1) {
		textWidth = Canvas->TextWidth(FCaption);
		textStep = (FRect.Right + FRect.Left) / 2 - textWidth;
		Canvas->TextOut(textStep, 5, FCaption);
	}

	Canvas->Pen->Color = clGray;
	Canvas->Pen->Style = psDot;
	int count = 10;
	if (FMaxXValue % 10) {
		count = 8;
	}
	// Прорисовка вертикальных линий и подписей

	textStep = FMaxXValue / count;
	int textValue = 0;
	int pos;
	float step = textStep * FStepX;
	for (int i = 0; i <= count; i++) {
		pos = step * i + FRect.Left;
		Canvas->MoveTo(pos, FRect.Bottom + 4);
		Canvas->LineTo(pos, FRect.Top - 3);
		textWidth = Canvas->TextWidth(textValue);
		Canvas->TextOut(pos - textWidth / 2, FRect.Bottom + 4, textValue);
		textValue+= textStep;
	}
	// Прорисовка горизонатльных линий и подписей
	count = 5;
	textStep = FMaxYValue / count;
	textValue = FMaxYValue;
	step = textStep * FStepY;
	for (int i = 0; i <= count; i++) {
		pos = step * i + FRect.Top;
		Canvas->MoveTo(FRect.Left - 4, pos);
		Canvas->LineTo(FRect.Right + 3, pos);
		AnsiString s = "   " + IntToStr(textValue);
		textWidth = Canvas->TextWidth(s);
		Canvas->TextOut(FRect.Left - textWidth - 5, pos - 4, s);
		textValue-= textStep;
	}
	Canvas->Pen->Style = psSolid;
	// Вывод графиков
	for (int i = 0; i < FList->Count; i++) {
		((TRangeSeries*)FList->Items[i])->Paint(Canvas);
	}
}

__fastcall TBaseRangeChart::TBaseRangeChart(TComponent* Owner)
	: TCustomControl(Owner)
{
    Color = clWhite;
	FList = new TList;
	FMaxYValue = 200;
    FMaxXValue = 10;
	SetBounds(Left, Top, 500, 200);
}

__fastcall TBaseRangeChart::~TBaseRangeChart()
{
	FOutputBlocking = true;
	Clear();
	delete FList;
}

//TRangeChart****************************************************************

void __fastcall TRangeChart::DownBtnClick(TObject *Sender)
{
	int value = FMaxYValue * 2;
	if (value < 65535) {
		SetMaxYValue(value);
	}
}

void __fastcall TRangeChart::UpBtnClick(TObject *Sender)
{
	int value = FMaxYValue / 2;
	if (value >= 25) {
		SetMaxYValue(value);
	}
}

void __fastcall TRangeChart::MouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
	TPoint point = Point(X, Y);
	if (PtInRect(FRect, point)&&(Shift.Contains(ssLeft))) {
		TPenMode mode = Canvas->Pen->Mode;
		Canvas->Pen->Mode = pmNotXor;
		Canvas->Pen->Color = clBlue;
		Canvas->MoveTo(FPosX, FRect.Bottom);
		Canvas->LineTo(FPosX, FRect.Top);
		FPosX = X;
		Canvas->MoveTo(X, FRect.Bottom);
		Canvas->LineTo(X, FRect.Top);
		Canvas->Pen->Mode = mode;
		Canvas->Pen->Color = clBlack;
        PositionTracking();
	}
}

void __fastcall TRangeChart::Paint()
{
	if (FOutputBlocking) {
        return;
	}
	TBaseRangeChart::Paint();
	TPenMode mode = Canvas->Pen->Mode;
	Canvas->Pen->Color = clBlue;
	Canvas->Pen->Mode = pmNotXor;
	Canvas->MoveTo(FPosX, FRect.Bottom);
	Canvas->LineTo(FPosX, FRect.Top);
	Canvas->Pen->Mode = mode;
	Canvas->Pen->Color = clBlack;
    PositionTracking();
}

void __fastcall TRangeChart::PositionTracking()
{
	if (FItemIndex == -1) {
		return;
	}
	int index = round((FPosX - FRect.Left) / FStepX);
	int value = ((TRangeSeries*)FList->Items[FItemIndex])->Values[index];
	int p = (FRect.Right + FRect.Left) / 2;
	AnsiString s = "[" + IntToStr(value) + "], Pos[" + IntToStr(index) + "]   ";
	Canvas->TextOut(p, 5, s);
}

__fastcall TRangeChart::TRangeChart(TComponent* Owner)
	: TBaseRangeChart(Owner)
{
	OnMouseMove = MouseMove;
	FUpBtn = new TBitBtn(this);
	FUpBtn->SetBounds(4, FRect.Top, FUpBtn->Height, FUpBtn->Height);
	FPosX = FRect.Left;
	FUpBtn->Glyph->LoadFromFile("up.bmp");
	FUpBtn->OnClick = UpBtnClick;
	FUpBtn->Parent = this;

	FDownBtn = new TBitBtn(this);
	FDownBtn->SetBounds(FUpBtn->Height+6, FRect.Top, FUpBtn->Height, FUpBtn->Height);
	FDownBtn->Glyph->LoadFromFile("down.bmp");
	FDownBtn->OnClick = DownBtnClick;
	FDownBtn->Parent = this;

	FCheckBoxLog = new TCheckBox(this);
	FCheckBoxLog->Caption = "Log";
	FCheckBoxLog->Checked = false;
	FCheckBoxLog->Left = 5;
	FCheckBoxLog->Top = 55;
	FCheckBoxLog->Parent = this;
}

__fastcall TRangeChart::~TRangeChart()
{
    //this->DoMouseDown()
}

