#pragma once
#include <windows.h>

#include <QtWidgets/QMainWindow>

#include <iostream>
#include <mutex>
#include <thread>
#include "ui_QtGuiApplication3.h"


class QtGuiApplication3 : public QMainWindow
{
	Q_OBJECT

	using BMInfo =
	struct _BMInfo 
	{
		BOOL			truecolour;
		BITMAPINFO		bmi;
		// Colormap info - comes straight after BITMAPINFO - **HACK**
		RGBQUAD			cmap[256];
	};

public:
	QtGuiApplication3(QWidget *parent = Q_NULLPTR);
	~QtGuiApplication3();
protected:
	bool __CaptureRect(const QSize& rect);
	void __InitDC();
	void __InitBminFo();

signals:
	void UpdateFream();

public slots:
	void Update();
private:
	Ui::QtGuiApplication3Class ui;
	BMInfo	m_bminfo;

	HDC		m_hmemdc;
	HDC		m_hrootdc_Desktop;
	HBITMAP	m_membitmap;
	void*	m_DIBbits;

	std::atomic_bool m_bRun{ true };
	std::shared_ptr<std::thread> m_spThread;


	std::mutex m_mBitMap;
};
