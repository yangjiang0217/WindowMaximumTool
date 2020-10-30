
// WindowMaximumDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WindowMaximum.h"
#include "WindowMaximumDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


const ATOM gc_HotKeyIdG = GlobalAddAtom("g_HotKeyIdG");   // Ctrl + Alt + Shift + G

// CWindowMaximumDlg �Ի���

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


// CWindowMaximumDlg ��Ϣ�������

BOOL CWindowMaximumDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
    //  ִ�д˲���
    SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
    SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    // TODO:  �ڴ���Ӷ���ĳ�ʼ������

    // ע���ȼ� Ctrl + Alt + Shift + G (���ظ������������յ����)
    BOOL bRet = RegisterHotKey(GetSafeHwnd(), gc_HotKeyIdG, MOD_CONTROL | MOD_ALT | MOD_SHIFT | MOD_NOREPEAT, 'G');
    if (bRet != TRUE)
    {
        MessageBox("�ȼ�ע��ʧ��");
    }

    m_toolTip.Create(this);
    m_toolTip.AddTool(GetDlgItem(IDC_EDIT_HWND), "������16���ƴ��ھ��");
    m_toolTip.AddTool(GetDlgItem(ID_BTN_MAX), "�����Ϸ������������16���ƴ��ھ��");
    m_toolTip.SetDelayTime(200); // �����ӳ�
    m_toolTip.SetTipTextColor(RGB(0, 0, 255)); // ������ʾ�ı�����ɫ
    m_toolTip.SetTipBkColor(RGB(255, 255, 255)); // ������ʾ��ı�����ɫ
    m_toolTip.Activate(TRUE); // �����Ƿ�������ʾ

    return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CWindowMaximumDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    CDialogEx::OnSysCommand(nID, lParam);
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CWindowMaximumDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // ���ڻ��Ƶ��豸������

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // ʹͼ���ڹ����������о���
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // ����ͼ��
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
        // ���游���ھ��
        m_hWndParent = ::GetParent(m_hWndWindow);
        ::SetParent(m_hWndWindow, NULL);
        // ����ȫ��֮ǰ�Ĵ���λ��
        ::GetWindowPlacement(m_hWndWindow, &m_wpPrev);

        RECT rcWindow = { 0 }; // ����ȫ�����λ��

        // ȡ�ô������ڵ���ʾ�����
        HMONITOR hMonitor = ::MonitorFromWindow(m_hWndParent, MONITOR_DEFAULTTONULL);
        if (hMonitor != NULL)
        {
            // ��ȡ��ʾ����Ϣ
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
            // ��ȡ�����С
            ::GetWindowRect(hWndDesktop, &rcWindow);
            TRACE("DesktopRect[%d,%d,%d,%d]\n", rcWindow.left, rcWindow.top,
                rcWindow.right, rcWindow.bottom);
        }
        int nWidth = rcWindow.right - rcWindow.left;
        int nHeight = rcWindow.bottom - rcWindow.top;
        // ���ô���ȫ��
        ::SetWindowPos(m_hWndWindow, HWND_TOP, rcWindow.left, rcWindow.top, nWidth, nHeight, SWP_SHOWWINDOW);
        m_bFullScreenFlag = TRUE;
        SetDlgItemText(ID_BTN_MAX, "��ԭ");
    }
    else
    {
        ::SetParent(m_hWndWindow, m_hWndParent);
        // �ָ�����
        ::SetWindowPlacement(m_hWndWindow, &m_wpPrev);
        m_bFullScreenFlag = FALSE;
        SetDlgItemText(ID_BTN_MAX, "ȫ��");
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
    // TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

    UnregisterHotKey(GetSafeHwnd(), gc_HotKeyIdG);

    CDialogEx::OnClose();
}
