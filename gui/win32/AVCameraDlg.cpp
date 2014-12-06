
// AVCameraDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AVCamera.h"
#include "AVCameraDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAVCameraDlg 对话框

UINT __cdecl CameraThreadProc( LPVOID pParam )
{
	VideoMgr::Camera* pObject = (VideoMgr::Camera*)pParam;
	pObject->thread_task();
	return 0;
}

CAVCameraDlg::CAVCameraDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAVCameraDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAVCameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAVCameraDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CAVCameraDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CAVCameraDlg::OnBnClickedButtonPause)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CAVCameraDlg::OnBnClickedButtonStop)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CAVCameraDlg 消息处理程序

BOOL CAVCameraDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	_camera.reset(new VideoMgr::Camera());
	AfxBeginThread(CameraThreadProc, _camera.get());
	UpdateStatus(VideoMgr::CREATED);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAVCameraDlg::OnPaint()
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
HCURSOR CAVCameraDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAVCameraDlg::UpdateStatus(VideoMgr::CameraSatus status)
{
	switch (status)
	{
	case VideoMgr::CREATED:
	case VideoMgr::STOPPED:
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_PAUSE)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(false);
		break;
	case VideoMgr::PAUSED:
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_PAUSE)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(true);
		break;
	case VideoMgr::RECORDING:
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_PAUSE)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(true);
		break;
	}
}

void CAVCameraDlg::OnBnClickedButtonStart()
{
	UpdateStatus(VideoMgr::RECORDING);
	_camera->start("d:\\test.mp4", 640, 480, 1815484);
}

void CAVCameraDlg::OnBnClickedButtonPause()
{
	UpdateStatus(VideoMgr::PAUSED);
	_camera->pause();
}

void CAVCameraDlg::OnBnClickedButtonStop()
{
	UpdateStatus(VideoMgr::STOPPED);
	_camera->stop();
}

void CAVCameraDlg::OnClose()
{
	_camera->exit();
	CDialogEx::OnClose();
}
