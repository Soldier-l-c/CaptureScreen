#pragma once
#include <windows.h>

#include <QtWidgets/QMainWindow>

#include <iostream>
#include <mutex>
#include <thread>
#include "ui_QtGuiApplication3.h"

#define SCREEN_OFF_SETX 0
#define SCREEN_OFF_SETY 0

class QtGuiApplication3 : public QMainWindow
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
	QtGuiApplication3(QWidget *parent = Q_NULLPTR);
	~QtGuiApplication3();

protected:
	bool __CaptureRect(const QSize& rect);
	void __InitDC();
	void __InitBminFo();
	bool __IsPointInDragnWidget(const QWidget* widget, const QPoint& point);
protected:
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;

	//ÉùÃ÷
	virtual void mouseDoubleClickEvent(QMouseEvent*) override;

signals:
	void UpdateFream();

public slots:
	void Update();

private:
	Ui::QtGuiApplication3Class ui;
	BMInfo	m_bminfo;

	HDC		m_hmemdc{ nullptr };
	HDC		m_hrootdc_Desktop{ nullptr };
	HBITMAP	m_membitmap{ nullptr };

	std::atomic_int32_t m_nOffsetX{ SCREEN_OFF_SETX };
	std::atomic_int32_t m_nOffsetY{ SCREEN_OFF_SETY };

	std::atomic_bool m_bRun{ true };
	std::shared_ptr<std::thread> m_spThread;

	std::mutex m_mBitMap;

	QPoint m_poPressedPoint;
	QPoint m_poLastPoint;
	bool m_bIsMove{ false };
};
