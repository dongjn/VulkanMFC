
//#include "stdafx.h"
#include "SkiaWnd.h"

#include<string>
#include<SkCanvas.h>
#include<SkPaint.h>
#include<limits>
LPCTSTR   SkiaWnd::myClassName= nullptr;
 
SkiaWnd::SkiaWnd(CWnd* parent)
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
	Create(myClassName, "LeftView", 0, { 0,0,400,400 }, parent, 0);
	ShowWindow(SW_SHOW);

}




SkiaWnd::~SkiaWnd()
{
	


}
BEGIN_MESSAGE_MAP(SkiaWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()


void SkiaWnd::OnPaint()
{


	auto s0 = (std::numeric_limits<size_t>::max)();
	auto s = 5;
	
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴�������Ϣ�����������
					   // ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()
	//BITMAPCOREINFO
	BITMAP bmpSrc;
	auto currenBitmap = dc.GetCurrentBitmap();
	currenBitmap->GetBitmap(&bmpSrc);
	BITMAP bmpIndirect;
	bmpIndirect.bmWidth = mBitmap.width();
	bmpIndirect.bmHeight = mBitmap.height();
	bmpIndirect.bmBitsPixel = 32;
	bmpIndirect.bmBits = bmpBuf;
	bmpIndirect.bmWidthBytes = mBitmap.width() * 4;
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
	SkCanvas canvas(mBitmap);
	SkPaint p;
	p.setStyle(SkPaint::Style::kStroke_Style);
	p.setColor(0xff0000ff);
	//p.setARGB(0, 0, 0, 0);
	canvas.drawRect({0,0,100,100}, p);
	Invalidate();

}

void SkiaWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	if (this->bmpBuf)
		delete[] bmpBuf;
	bmpBuf = new uint8_t[cx* cy * 4];
	//mBitmap.reset(new SkBitmap());
	SkImageInfo info = SkImageInfo::Make(cx, cy, kBGRA_8888_SkColorType, kOpaque_SkAlphaType);
	mBitmap.setInfo(info);
	mBitmap.setPixels(bmpBuf);
	//auto canvas = new SkCanvas(mBitmap);
	// TODO: �ڴ˴�������Ϣ�����������
}