
// VulkanMFCDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "VulkanMFC.h"
#include "VulkanSkiaDlg.h"
#include "afxdialogex.h"
#include"SkiaWnd.h"
#include"vulkan_context.h"
#include"skia_backed_vk.h"
#include"utility.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace seraphim;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CVulkanMFCDlg 对话框



CVulkanSkiaCDlg::CVulkanSkiaCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VULKANMFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVulkanSkiaCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVulkanSkiaCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON1, &CVulkanSkiaCDlg::OnRthUpdateClick)
	ON_BN_CLICKED(IDC_UpdteLeft, &CVulkanSkiaCDlg::OnBnClickedUpdteleft)
	ON_BN_CLICKED(IDC_COPY_TO_LEFT, &CVulkanSkiaCDlg::OnBnClickedCopyToLeft)
	ON_BN_CLICKED(IDC_COPY_TO_RITHT, &CVulkanSkiaCDlg::OnBnClickedCopyToRitht)
END_MESSAGE_MAP()


// CVulkanMFCDlg 消息处理程序

BOOL CVulkanSkiaCDlg::OnInitDialog()
{

	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	
	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CVulkanSkiaCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CVulkanSkiaCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CVulkanSkiaCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CVulkanSkiaCDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	//RECT lRect = { 0,0,cx / 2 ,cy - 100};
	//leftWnd->MoveWindow(&lRect, true);
	//RECT rRect = { cx / 2,0,cx ,cy - 100 };
	//rightWnd->MoveWindow(&rRect, true);
	//vulkanContext->resize(cx, cy);
	//auto skiaBackend = seraphim::SkiaBackedVK::get();
	// TODO: 在此处添加消息处理程序代码
}


void CVulkanSkiaCDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	leftWnd->DrawSkia();
	//CDialogEx::OnCancel();
}


BOOL CVulkanSkiaCDlg::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam)
{
	// TODO: 在此添加专用代码和/或调用基类

	BOOL rst = CDialogEx::CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, lpParam);;

	return  rst;
}


BOOL CVulkanSkiaCDlg::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialogEx::Create(lpszTemplateName, pParentWnd);
}



int CVulkanSkiaCDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	OutputDebugStringA("MATH_CORE_JNI\n");
	OutputDebugStringW(L"MATH_CORE_JNI\n");
	slog(INFO_LEVEL, TAG, "OnSize cx=",1000, "|cy=",1000);
	CRect rect;
	GetClientRect(&rect);
	UINT32 width = rect.Width() / 2 ;
	UINT32 height = rect.Height() - 50;
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	rightWnd = new VulkanWnd(this, 0,0,width  ,height);
	HINSTANCE hInstance = GetModuleHandle(nullptr);
	vulkanContext = seraphim::VulkanContext::make(rightWnd->m_hWnd, hInstance, width, height);
	auto backend = seraphim::SkiaBackedVK::make(vulkanContext);
	leftWnd = new SkiaWnd(this,backend,width,0,width*2 ,height);
	// TODO:  在此添加您专用的创建代码

	return 0;
}



void CVulkanSkiaCDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	// TODO: 在此处添加消息处理程序代码
}


void CVulkanSkiaCDlg::OnRthUpdateClick()
{
	// TODO: 在此添加控件通知处理程序代码
	rightWnd->Invalidate(false);
}


void CVulkanSkiaCDlg::OnBnClickedUpdteleft()
{
	// TODO: 在此添加控件通知处理程序代码
	leftWnd->Invalidate(false);
}


void CVulkanSkiaCDlg::OnBnClickedCopyToLeft()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CVulkanSkiaCDlg::OnBnClickedCopyToRitht()
{
	// TODO: 在此添加控件通知处理程序代码
}
