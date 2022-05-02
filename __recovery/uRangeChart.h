//---------------------------------------------------------------------------
//	Версия 0.3 от 02.05.2022г.
//  Нереалзована логарифмическая шкала
//	Отсуттвует автомасштабирование
//  Рализована часть элементов управления
//  Глифы загружаются из файла, необходимо использовать ресурсный файл
//  Возможно необходима буферизация вывода (частично компенсируется
//		предварительным расчетом координат точек)
//  Неоптимальны вызовы прорисовки
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
	uint16_t*  		FBuffer = nullptr;	// Буфер данных
	AnsiString  	FCaption;			// Подпись линии
	TColor      	FColor = clBlack; 	// Цвет линии
	uint16_t       	FCount = 0; 		// Количество элементов в буфере
	bool            FLog = false;       // Флаг - Логарифмическая шкала
	bool            FOverflow = false;  // Флаг - Переполнение по оси У (возможно и не надо)
	TPoint*     	FPoints = nullptr;	// Массив точек
	TCustomControl* FRangeChart;        // Родительский компонент
	TRect           FRect;              // Область прорисовки
	float         	FStepX = 1; 		// Шаг по X
	float         	FStepY = 1; 		// Шаг по Y
	bool            FVisible = true;    // Флаг - Видимость
	void __fastcall Calc(); // Предварительный расчет точек TPoint
	uint16_t __fastcall GetValue(uint16_t Index);
	void __fastcall SetColor(TColor Value);
	void __fastcall SetLog(bool Value);
	void __fastcall SetValue(uint16_t Index, uint16_t Value);  // Установка значения любой точки ??? возможно и не надо
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
	void __fastcall SetParams(float StepX, float StepY, TRect Rect); // Передача графикам основных параметров
	__fastcall TRangeSeries(TCustomControl* RangeChart, AnsiString Caption,
		uint16_t Count);
	__fastcall ~TRangeSeries();
};

class TBaseRangeChart : public TCustomControl
{
__published:
	__property OnMouseDown;  // Экспиремент - повышение области видимости события
protected:
	void __fastcall SetBounds(int Left, int Top, int Width, int Height) override;
protected:
	bool 		FAutoScale = false; // Флаг - автомасштабирования по оси У (возможно и не надо)
	AnsiString 	FCaption = ""; // Подпись текщей кривой
	TList*   	FList;  	// Список кривых
	int         FItemIndex = -1; // Индекс текущей кривой
	uint16_t 	FMaxXValue; // Максимальное значение по оси Х
	uint32_t 	FMaxYValue; // Максимальное значение по оси У
	float	 	FStepX;     // Шаг по оси Х
	float    	FStepY;     // Шаг по оси У
	TRect    	FRect; 		// Область графика
	bool        FOutputBlocking = true; // Блокировка вывода
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
	void __fastcall BeginUpdate() {FOutputBlocking = true;}; // Запрет вывода на время работы с данными и т.д.
	void __fastcall Clear();
	void __fastcall CreateWnd() override;  // Метод создания Wnd для определение возможности вывода
	void __fastcall Delete(unsigned int Index);
	void __fastcall DestroyWnd() override; // Метод запрещающий вывод
	void __fastcall EndUpdate() {FOutputBlocking = false;}; // Разрешение вывода
	void __fastcall Paint() override; // Вывод графика

	__fastcall TBaseRangeChart(TComponent* Owner) override;
	__fastcall ~TBaseRangeChart();
};

class TRangeChart : public TBaseRangeChart
{
__published:

protected:
	TBitBtn*	FDownBtn;  		// Кнопка масштаба по оси У - вниз
	TBitBtn*	FUpBtn;         // Кнопка масштаба по оси У - вверх
	TCheckBox*  FCheckBoxLog;   // Флаг логарифмической шкалы
	int         FPosX;          // Позиция курсора
	void __fastcall DownBtnClick(TObject *Sender);
	void __fastcall MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall PositionTracking();
	void __fastcall UpBtnClick(TObject *Sender);
public:
	void __fastcall Paint() override; // Прорисовка крсора и подписей
	__fastcall TRangeChart(TComponent* Owner) override;
	__fastcall ~TRangeChart();
};



#endif
