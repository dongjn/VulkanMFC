#pragma once
#include <afxwin.h>
#include<string>
#include<memory>

#include<SkBitmap.h>
class SkiaWnd :public CWnd
{
private:

	static LPCTSTR  myClassName;
private:
	uint8_t* bmpBuf{nullptr};
	SkBitmap mBitmap;// {nullptr};
	
public:

	SkiaWnd(CWnd* parent);
	void DrawSkia();
	
	virtual ~SkiaWnd();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

