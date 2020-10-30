
// WindowMaximumDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WindowMaximum.h"
#include "WindowMaximumDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


const ATOM gc_HotKeyIdG = GlobalAddAtom("g_HotKeyIdG");   // Ctrl + Alt + Shift + G

// CWindowMaximumDlg 对话框

CWindowMaximumDlg::CWindowMaximumDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CWindowMaximumDlg::IDD, pParent)
    , m_bFullScreenFlag(FALSE)
    , m_hWndParent(NULL)
    , m_hWndWindow(NULL)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    memset(&m_wpPrev, 0, sizeof(m_wpPrev));
}

void CWindowMaximumDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWindowMaximumDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(ID_BTN_MAX, &CWindowMaximumDlg::OnBnClickedBtnMax)
    ON_WM_CLOSE()
END_MESSAGE_MAP()


// CWindowMaximumDlg 消息处理程序

BOOL CWindowMaximumDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);			// 设置大图标
    SetIcon(m_hIcon, FALSE);		// 设置小图标

    // TODO:  在此添加额外的初始化代码

    // 注册热键 Ctrl + Alt + Shift + G (不重复，不会连续收到多次)
    BOOL bRet = RegisterHotKey(GetSafeHwnd(), gc_HotKeyIdG, MOD_CONTROL | MOD_ALT | MOD_SHIFT | MOD_NOREPEAT, 'G');
    if (bRet != TRUE)
    {
        MessageBox("热键注册失败");
    }

    m_toolTip.Create(this);
    m_toolTip.AddTool(GetDlgItem(IDC_EDIT_HWND), "请输入16进制窗口句柄");
    m_toolTip.AddTool(GetDlgItem(ID_BTN_MAX), "请在上方输入框中输入16进制窗口句柄");
    m_toolTip.SetDelayTime(200); // 设置延迟
    m_toolTip.SetTipTextColor(RGB(0, 0, 255)); // 设置提示文本的颜色
    m_toolTip.SetTipBkColor(RGB(255, 255, 255)); // 设置提示框的背景颜色
    m_toolTip.Activate(TRUE); // 设置是否启用提示

    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CWindowMaximumDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    CDialogEx::OnSysCommand(nID, lParam);
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWindowMaximumDlg::OnPaint()
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
HCURSOR CWindowMaximumDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}



void CWindowMaximumDlg::OnBnClickedBtnMax()
{
    CString strHwnd = "";
    GetDlgItemText(IDC_EDIT_HWND, strHwnd);
    if (strHwnd.IsEmpty())
    {
        return;
    }
    int nHwnd = 0;
    sscanf_s(strHwnd.GetBuffer(), "%x", &nHwnd);
    m_hWndWindow = HWND(nHwnd);
    if (FALSE == m_bFullScreenFlag)
    {
        // 保存父窗口句柄
        m_hWndParent = ::GetParent(m_hWndWindow);
        ::SetParent(m_hWndWindow, NULL);
        // 保存全屏之前的窗口位置
        ::GetWindowPlacement(m_hWndWindow, &m_wpPrev);

        RECT rcWindow = { 0 }; // 窗口全屏后的位置

        // 取得窗口所在的显示器句柄
        HMONITOR hMonitor = ::MonitorFromWindow(m_hWndParent, MONITOR_DEFAULTTONULL);
        if (hMonitor != NULL)
        {
            // 获取显示器信息
            MONITORINFO stMonitorInfo = { 0 };
            stMonitorInfo.cbSize = sizeof(MONITORINFO);
            ::GetMonitorInfo(hMonitor, &stMonitorInfo);
            memcpy(&rcWindow, &stMonitorInfo.rcMonitor, sizeof(RECT));
            TRACE("MonitorRect[%d,%d,%d,%d]\n", stMonitorInfo.rcMonitor.left, stMonitorInfo.rcMonitor.top,
                stMonitorInfo.rcMonitor.right, stMonitorInfo.rcMonitor.bottom);
        }
        else
        {
            HWND hWndDesktop = ::GetDesktopWindow();
            // 获取桌面大小
            ::GetWindowRect(hWndDesktop, &rcWindow);
            TRACE("DesktopRect[%d,%d,%d,%d]\n", rcWindow.left, rcWindow.top,
                rcWindow.right, rcWindow.bottom);
        }
        int nWidth = rcWindow.right - rcWindow.left;
        int nHeight = rcWindow.bottom - rcWindow.top;
        // 设置窗口全屏
        ::SetWindowPos(m_hWndWindow, HWND_TOP, rcWindow.left, rcWindow.top, nWidth, nHeight, SWP_SHOWWINDOW);
        m_bFullScreenFlag = TRUE;
        SetDlgItemText(ID_BTN_MAX, "还原");
    }
    else
    {
        ::SetParent(m_hWndWindow, m_hWndParent);
        // 恢复窗口
        ::SetWindowPlacement(m_hWndWindow, &m_wpPrev);
        m_bFullScreenFlag = FALSE;
        SetDlgItemText(ID_BTN_MAX, "全屏");
    }
}


BOOL CWindowMaximumDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_MOUSEMOVE)
        m_toolTip.RelayEvent(pMsg);
    if (pMsg->message == WM_HOTKEY)
    {
        if (pMsg->wParam == gc_HotKeyIdG)
        {
            POINT pt;
            GetCursorPos(&pt);
            HWND hWnd = ::WindowFromPoint(pt);
            char szHwndHex[16] = { 0 };
            _snprintf_s(szHwndHex, sizeof(szHwndHex) - 1, "0x%X", hWnd);
            SetDlgItemText(IDC_EDIT_HWND, szHwndHex);
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}


void CWindowMaximumDlg::OnClose()
{
    // TODO:  在此添加消息处理程序代码和/或调用默认值

    UnregisterHotKey(GetSafeHwnd(), gc_HotKeyIdG);

    CDialogEx::OnClose();
}
