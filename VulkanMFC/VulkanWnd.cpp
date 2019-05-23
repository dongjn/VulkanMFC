#include "stdafx.h"
#include "VulkanWnd.h"
#include"vulkan_context.h"
LPCSTR VulkanWnd::myClassName = nullptr;
using namespace seraphim;
VulkanWnd::VulkanWnd(CWnd* parent,LONG l,LONG t,LONG r,LONG b)
{

	if (myClassName == nullptr) {
		try
		{
			myClassName = AfxRegisterWndClass(
				CS_VREDRAW | CS_HREDRAW,
				::LoadCursor(NULL, IDC_ARROW),
				(HBRUSH) ::GetStockObject(DKGRAY_BRUSH),
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


VulkanWnd::~VulkanWnd()
{
}
BEGIN_MESSAGE_MAP(VulkanWnd, CWnd)
	ON_WM_SIZE()
//	ON_WM_DRAWITEM()
ON_WM_PAINT()
END_MESSAGE_MAP()


void VulkanWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);


	// TODO: 在此处添加消息处理程序代码
}


//void VulkanWnd::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	CWnd::OnDrawItem(nIDCtl, lpDrawItemStruct);
//}


void VulkanWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CWnd::OnPaint()
	VulkanContext::get()->present();

}
