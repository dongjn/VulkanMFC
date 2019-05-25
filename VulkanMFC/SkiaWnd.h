#pragma once
#include <afxwin.h>
#include<string>
#include<memory>
#include<SkCanvas.h>
#include<SkBitmap.h>
#include <chrono>
#include <stdio.h>
#include <chrono>
#include <cinttypes>
#include <ctime>
#include <sstream>
#include <iomanip>
#include"skia_backed_vk.h"
using namespace seraphim;

class SkiaWnd :public CWnd
{
private:

	static LPCTSTR  myClassName;
private:
	size_t   cbBuf;
	uint8_t* bmpBuf{nullptr};
	std::unique_ptr<SkCanvas> canvas{nullptr};
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

