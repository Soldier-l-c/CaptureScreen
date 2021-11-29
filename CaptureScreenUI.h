#pragma once
#ifdef WIN32
#include <windows.h>
#endif // WIN32

#include <QtWidgets/QMainWindow>

#include <iostream>
#include <mutex>
#include <thread>
#include "ui_CaptureScreenUI.h"

#define SCREEN_OFF_SETX 0
#define SCREEN_OFF_SETY 0

class CaptureScreenUI : public QMainWindow
{
	Q_OBJECT

	using BMInfo = struct 
	{
		BOOL			truecolour;
		BITMAPINFO		bmi;
		// Colormap info - comes straight after BITMAPINFO - **HACK**
		RGBQUAD			cmap[256];
	};

public:
	CaptureScreenUI(QWidget *parent = Q_NULLPTR);
	~CaptureScreenUI();

protected:
	bool __CaptureRect(const QSize& rect);
	void __InitDC();
	void __InitBminFo();
	bool __IsPointInDragnWidget(const QWidget* widget, const QPoint& point);

protected:
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void keyPressEvent(QKeyEvent* event);
	virtual void mouseDoubleClickEvent(QMouseEvent*) override;

signals:
	void UpdateFream();

public slots:
	void Update();

private:
	Ui::CaptureScreenUI ui;
	BMInfo	m_bminfo;

	HDC		m_hmemdc{ nullptr };
	HDC		m_hrootdc_Desktop{ nullptr };
	HBITMAP	m_membitmap{ nullptr };

	std::atomic_int32_t m_nOffsetX{ SCREEN_OFF_SETX };
	std::atomic_int32_t m_nOffsetY{ SCREEN_OFF_SETY };

	std::atomic_bool m_bRun{ true };
	std::unique_ptr<std::thread> m_spThread;

	std::mutex m_mBitMap;

	QPoint m_poPressedPoint;
	QPoint m_poLastPoint;
	std::atomic_bool m_bIsMove{ false };
};
