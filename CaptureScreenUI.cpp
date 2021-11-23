#include "CaptureScreenUI.h"
#include <QtWin>
#include <QApplication>
#include <qdesktopwidget.h>
#include <QMouseEvent>

//#define CAPTURE_MOVED_WITH_MOUSE
//#define MANGNIFYING

#ifdef CAPTURE_MOVED_WITH_MOUSE
#define WIDTH_FREAM 100 
#define HEIGHT_FREAM 20 
#else
#define WIDTH_FREAM 640 //QApplication::desktop()->availableGeometry().width()
#define HEIGHT_FREAM 640 //QApplication::desktop()->availableGeometry().height()
#endif // CAPTURE_MOVED_WITH_MOUSE

#ifdef MANGNIFYING
#define MANGINFY_SIZE 8
#else
#define MANGINFY_SIZE 1
#endif // MANGNIFYING

QtGuiApplication3::QtGuiApplication3(QWidget* parent)
	: QMainWindow(parent, Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint)
{
	ui.setupUi(this);
	connect(this, &QtGuiApplication3::UpdateFream, this, &QtGuiApplication3::Update);

	setStyleSheet("border:1px solid red;");

	__InitDC();
	__InitBminFo();

	m_spThread = std::make_unique<std::thread>
		([this]
		{
			while (m_bRun)
			{
				Sleep(50);
				if (m_bIsMove)continue;

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

	QSize picSize(bkPixmap.size() * MANGINFY_SIZE);
	bkPixmap = bkPixmap.scaled(picSize, Qt::KeepAspectRatio);

	this->resize(bkPixmap.size());
	QPalette pal(palette());
	pal.setBrush(QPalette::Window, QBrush(bkPixmap));
	setPalette(pal);
}

bool QtGuiApplication3::__CaptureRect(const QSize& rect)
{
	std::lock_guard<std::mutex>lock(m_mBitMap);

	SelectObject(m_hmemdc, m_membitmap);
	
	POINT mousePos;
	GetCursorPos(&mousePos);

	const BOOL blitok = BitBlt(m_hmemdc, 0, 0, rect.width(), rect.height(),
		m_hrootdc_Desktop,
#ifdef CAPTURE_MOVED_WITH_MOUSE
		(mousePos.x - WIDTH_FREAM/2)> 0 ? (mousePos.x - WIDTH_FREAM / 2) : 0,
		(mousePos.y - HEIGHT_FREAM/2)> 0 ? (mousePos.y - HEIGHT_FREAM / 2) :0,
#else 
		m_nOffsetX,
		m_nOffsetY,
#endif // CAPTURE_MOVED_WITH_MOUSE

		(CAPTUREBLT | SRCCOPY));

	return blitok ? true : false;
}


bool QtGuiApplication3::__IsPointInDragnWidget(const QWidget* widget, const QPoint& point)
{
	//判断位置
	QRect rect = widget->rect();
	return widget->geometry().contains(this->mapFromGlobal(QCursor::pos()));
}

void QtGuiApplication3::mousePressEvent(QMouseEvent* event)
{
	m_poPressedPoint = event->pos();		//触发鼠标按压事件的点
	m_bIsMove = true;
	QMainWindow::mousePressEvent(event);
}

void QtGuiApplication3::mouseMoveEvent(QMouseEvent* event)
{
	if ((event->buttons() == Qt::LeftButton) && m_bIsMove == true)
	{
		QPoint currPoint = this->pos();
		this->move(currPoint - (m_poPressedPoint - event->pos()));
	}
	QMainWindow::mouseMoveEvent(event);
}

void QtGuiApplication3::mouseReleaseEvent(QMouseEvent* event)
{
	m_bIsMove = false;
	QMainWindow::mouseReleaseEvent(event);
}

void QtGuiApplication3::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Up)
	{
		this->move(this->pos() - QPoint(0, 1));
	}
	else if (event->key() == Qt::Key_Down)
	{
		this->move(this->pos() - QPoint(0, -1));
	}
	else if(event->key() == Qt::Key_Left)
	{
		this->move(this->pos() - QPoint(1, 0));
	}
	else if (event->key() == Qt::Key_Right)
	{
		this->move(this->pos() - QPoint(-1, 0));
	}
	QMainWindow::keyPressEvent(event);
}

void QtGuiApplication3::mouseDoubleClickEvent(QMouseEvent* event)
{
	if ((event->buttons() == Qt::LeftButton))
	{
		close();
	}
}