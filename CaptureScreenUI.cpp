#include "CaptureScreenUI.h"
#include <QtWin>
#include <iostream>
#include <thread>
#include <QApplication>
#include <qdesktopwidget.h>

#define WIDTH_FREAM 640 //QApplication::desktop()->availableGeometry().width()
#define HEIGHT_FREAM 640 //QApplication::desktop()->availableGeometry().height()

#define SCREEN_OFF_SETX 0
#define SCREEN_OFF_SETY 0
QtGuiApplication3::QtGuiApplication3(QWidget* parent)
	: QMainWindow(parent, Qt::WindowStaysOnTopHint)
{
	ui.setupUi(this);
	connect(this, &QtGuiApplication3::UpdateFream, this, &QtGuiApplication3::Update);
	InitDC();
	std::thread([this] 
		{
			Sleep(1000);
			while (true)
			{
				CaptureRect({ WIDTH_FREAM, HEIGHT_FREAM });
				Sleep(30);
				emit UpdateFream();
			}
		}).detach();
	InitBminFo();
}

void QtGuiApplication3::InitDC()
{
	m_hrootdc_Desktop = GetDC(NULL);
	m_hmemdc = CreateCompatibleDC(m_hrootdc_Desktop);
	m_membitmap = CreateCompatibleBitmap(m_hrootdc_Desktop, 
		WIDTH_FREAM,
		HEIGHT_FREAM);
}

void QtGuiApplication3::InitBminFo()
{
	memset(&m_bminfo, 0, sizeof(m_bminfo));
	m_bminfo.bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_bminfo.bmi.bmiHeader.biBitCount = 32;
	m_bminfo.bmi.bmiHeader.biPlanes = 1;
	m_bminfo.bmi.bmiHeader.biWidth = WIDTH_FREAM;
	m_bminfo.bmi.bmiHeader.biHeight = HEIGHT_FREAM;
	m_bminfo.bmi.bmiHeader.biSizeImage = abs((m_bminfo.bmi.bmiHeader.biWidth * m_bminfo.bmi.bmiHeader.biHeight * m_bminfo.bmi.bmiHeader.biBitCount) / 8);
	m_bminfo.bmi.bmiHeader.biHeight = -abs(m_bminfo.bmi.bmiHeader.biHeight);
	m_bminfo.bmi.bmiHeader.biCompression = BI_RGB;
	m_bminfo.truecolour = true;
}

void QtGuiApplication3::Update()
{
	auto bkPixmap = QtWin::fromHBITMAP(m_membitmap);

	this->resize(bkPixmap.size());
	QPalette pal(palette());
	pal.setBrush(QPalette::Window, QBrush(bkPixmap));
	setPalette(pal);
}

bool QtGuiApplication3::CaptureRect(const QSize& rect)
{
	(HBITMAP)SelectObject(m_hmemdc, m_membitmap);

	const BOOL blitok = BitBlt(m_hmemdc, 0, 0, rect.width(), rect.height(),
		m_hrootdc_Desktop,
		SCREEN_OFF_SETX,
		SCREEN_OFF_SETY,
		(CAPTUREBLT | SRCCOPY));

	return blitok ? true : false;
}
