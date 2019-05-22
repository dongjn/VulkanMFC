
// VulkanMFCDlg.h: 头文件
//

#pragma once
#include"SkiaWnd.h"
#include"VulkanWnd.h"
#include<memory>
using std::shared_ptr;
namespace seraphim {
	class VulkanContext;
};


// CVulkanMFCDlg 对话框
class CVulkanSkiaCDlg : public CDialogEx
{
// 构造
public:
	CVulkanSkiaCDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VULKANMFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	SkiaWnd *leftWnd;
	VulkanWnd* rightWnd;
	shared_ptr<seraphim::VulkanContext> vulkanContext{ nullptr };
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnCancel();
	virtual BOOL CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam = NULL);
	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnRthUpdateClick();
	afx_msg void OnBnClickedUpdteleft();
	afx_msg void OnBnClickedCopyToLeft();
	afx_msg void OnBnClickedCopyToRitht();
};
