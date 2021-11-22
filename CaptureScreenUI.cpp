#include "CaptureScreenUI.h"
#include <QtWin>
#include <QApplication>
#include <qdesktopwidget.h>

#define WIDTH_FREAM 640 //QApplication::desktop()->availableGeometry().width()
#define HEIGHT_FREAM 640 //QApplication::desktop()->availableGeometry().height()

#define SCREEN_OFF_SETX 200
#define SCREEN_OFF_SETY 200

QtGuiApplication3::QtGuiApplication3(QWidget* parent)
	: QMainWindow(parent, Qt::WindowStaysOnTopHint)
{
	ui.setupUi(this);
	connect(this, &QtGuiApplication3::UpdateFream, this, &QtGuiApplication3::Update);

	__InitDC();
	__InitBminFo();

	m_spThread = std::make_shared<std::thread>
		([this]
		{
			while (m_bRun)
			{
				Sleep(30);
				__CaptureRect({ WIDTH_FREAM, HEIGHT_FREAM });
				emit UpdateFream();
			}
		});
}

QtGuiApplication3::~QtGuiApplication3()
{
	m_bRun = false;

	if (m_spThread)
	{
		m_spThread->join();
	}
}

void QtGuiApplication3::__InitDC()
{
	m_hrootdc_Desktop = GetDC(NULL);
	m_hmemdc = CreateCompatibleDC(m_hrootdc_Desktop);
	m_membitmap = CreateCompatibleBitmap(m_hrootdc_Desktop, WIDTH_FREAM, HEIGHT_FREAM);
}

void QtGuiApplication3::__InitBminFo()
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
	QPixmap bkPixmap;
	{
		std::lock_guard<std::mutex>lock(m_mBitMap);
		bkPixmap = std::move(QtWin::fromHBITMAP(m_membitmap));
	}

	this->resize(bkPixmap.size());
	QPalette pal(palette());
	pal.setBrush(QPalette::Window, QBrush(bkPixmap));
	setPalette(pal);
}

bool QtGuiApplication3::__CaptureRect(const QSize& rect)
{
	std::lock_guard<std::mutex>lock(m_mBitMap);

	SelectObject(m_hmemdc, m_membitmap);

	const BOOL blitok = BitBlt(m_hmemdc, 0, 0, rect.width(), rect.height(),
		m_hrootdc_Desktop,
		SCREEN_OFF_SETX,
		SCREEN_OFF_SETY,
		(CAPTUREBLT | SRCCOPY));

	return blitok ? true : false;
}
