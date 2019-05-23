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


	// TODO: �ڴ˴������Ϣ����������
}


//void VulkanWnd::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//
//	CWnd::OnDrawItem(nIDCtl, lpDrawItemStruct);
//}


void VulkanWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()
	VulkanContext::get()->present();

}
