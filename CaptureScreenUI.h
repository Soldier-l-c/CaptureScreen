#pragma once
#include <windows.h>
#include <QtWidgets/QMainWindow>
#include "ui_QtGuiApplication3.h"

class QtGuiApplication3 : public QMainWindow
{
	Q_OBJECT

public:
	QtGuiApplication3(QWidget *parent = Q_NULLPTR);

	bool CaptureRect(const QSize& rect);
	void InitDC();
	void InitBminFo();
protected:
	
signals:
	void UpdateFream();

public slots:
	void Update();
private:
	Ui::QtGuiApplication3Class ui;

	struct _BMInfo {
			BOOL			truecolour;
			BITMAPINFO		bmi;
			// Colormap info - comes straight after BITMAPINFO - **HACK**
			RGBQUAD			cmap[256];
		} m_bminfo;

	//HDC			m_hrootdc_PixelEngine;
	HDC			m_hmemdc;
	HDC			m_hrootdc_Desktop;
	HBITMAP		m_membitmap;
	HBITMAP		m_oldbitmap;
	void* m_DIBbits;
};
