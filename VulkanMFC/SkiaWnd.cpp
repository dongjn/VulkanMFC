
//#include "stdafx.h"
#include "SkiaWnd.h"

#include<string>
#include<SkCanvas.h>
#include<SkPaint.h>
#include<limits>
#include"skia_backed_vk.h"
using namespace seraphim;
LPCTSTR   SkiaWnd::myClassName= nullptr;
 
SkiaWnd::SkiaWnd(CWnd* parent,std::shared_ptr<SkiaBackedVK> backed,LONG l,LONG t,LONG r,LONG b):backed(backed)
{
	int32_t u3;
	int64_t u6;
	SkCanvas* canvas;
	if (myClassName == nullptr) {
		try
		{
			myClassName = AfxRegisterWndClass(
				CS_VREDRAW | CS_HREDRAW,
				::LoadCursor(NULL, IDC_ARROW),
				(HBRUSH) ::GetStockObject(GRAY_BRUSH),
				::LoadIcon(NULL, IDI_APPLICATION));
		}
		catch (CResourceException* pEx)
		{
			AfxMessageBox(_T("Couldn't register class! (Already registered?)"));
			pEx->Delete();
		}

	}
	Create(myClassName, "LeftView", 0, { l,t,r,b }, parent, 0);
	ShowWindow(SW_SHOW);

}




SkiaWnd::~SkiaWnd()
{
	


}
BEGIN_MESSAGE_MAP(SkiaWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


void SkiaWnd::OnPaint()
{
	// device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
	auto s0 = (std::numeric_limits<size_t>::max)();
	auto s = 5;
	CRect rect;
	GetWindowRect(&rect);
	CPaintDC dc(this); 
	//BITMAPCOREINFO
	BITMAP bmpSrc;
	auto currenBitmap = dc.GetCurrentBitmap();
	currenBitmap->GetBitmap(&bmpSrc);
	BITMAP bmpIndirect;
	bmpIndirect.bmWidth = rect.Width();
	bmpIndirect.bmHeight = rect.Height();
	bmpIndirect.bmBitsPixel = 32;
	bmpIndirect.bmBits = bmpBuf;
	bmpIndirect.bmWidthBytes = rect.Width() * 4;
	bmpIndirect.bmType = 0;
	bmpIndirect.bmPlanes = 1;
	CBitmap bmp;
	auto bRst = bmp.CreateBitmapIndirect(&bmpIndirect);
	CDC bmpDC;
	bmpDC.CreateCompatibleDC(&dc);
	bmpDC.SelectObject(bmp);
	dc.BitBlt(0, 0, bmpIndirect.bmWidth, bmpIndirect.bmHeight, &bmpDC, 0, 0, SRCCOPY);
	//auto  b = CBitmap::CreateBitmapIndirect(bmp);
}


void SkiaWnd::DrawSkia() {
	SkPaint p;
	p.setAntiAlias(true);
	CRect rect;
	GetClientRect(&rect);

	

	p.setStyle(SkPaint::Style::kFill_Style);
	p.setColor(0x55112244);
	canvas->drawRect({ 0, 0, SkScalar(rect.Width()) , SkScalar(rect.Height()) }, p);
	
	
	p.setColor(0xff0000ff);
	canvas->drawRect({100,100,200,200}, p);
	p.setColor(0xff00ff00);
	canvas->drawRect({ 200,100,300,200 }, p);
	p.setColor(0xffff0000);
	canvas->drawRect({ 300,100,400,200 }, p);
	p.setStyle(SkPaint::Style::kStroke_Style);
	p.setStrokeWidth(2);
	p.setColor(0xffffffff);
	canvas->drawLine(0, 0, rect.Width(), rect.Height(), p);
	canvas->drawLine(0, rect.Height(), rect.Width(), 0, p);
	memset(bmpBuf, 0, cbBuf);
	backed->readPixel(1, bmpBuf, cbBuf);
	Invalidate(TRUE);

}

void SkiaWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	// TODO: 在此处添加消息处理程序代码
	if (this->bmpBuf)
		delete[] bmpBuf;
	cbBuf = cx * cy * 4;
	bmpBuf = new uint8_t[cbBuf];

}


int SkiaWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	CRect rect;

	GetWindowRect(&rect);
	canvas = backed->makeBacked(1, rect.Width(), rect.Height());
	
	//skCanvas.reset(backed->makeCanvas("SkiaWnd",rect.Width(),rect.Height()));
	return 0;
}



void SkiaWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
}
