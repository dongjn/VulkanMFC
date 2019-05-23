#pragma once
#include <afxwin.h>
#include<string>
#include<memory>
#include<SkCanvas.h>
#include<SkBitmap.h>
#include"skia_backed_vk.h"
using namespace seraphim;

class SkiaWnd :public CWnd
{
private:

	static LPCTSTR  myClassName;
private:
	uint8_t* bmpBuf{nullptr};
	SkBitmap mBitmap;// {nullptr};
	std::unique_ptr<SkCanvas> skCanvas{nullptr};
	std::shared_ptr<SkiaBackedVK> backed{nullptr};
public:

	SkiaWnd(CWnd* parent, std::shared_ptr<SkiaBackedVK> backed ,LONG l,LONG t,LONG r,LONG b);
	void DrawSkia();
	
	virtual ~SkiaWnd();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};

