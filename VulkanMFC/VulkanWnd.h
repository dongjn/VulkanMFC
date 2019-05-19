#pragma once
#include <afxwin.h>

class VulkanWnd :public CWnd
{
private:
	static LPCTSTR myClassName;
public:
	VulkanWnd(CWnd* parent);
	virtual ~VulkanWnd();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
//	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnPaint();
};

