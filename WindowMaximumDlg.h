
// WindowMaximumDlg.h : 头文件
//

#pragma once


// CWindowMaximumDlg 对话框
class CWindowMaximumDlg : public CDialogEx
{
    // 构造
public:
    CWindowMaximumDlg(CWnd* pParent = NULL);	// 标准构造函数

    // 对话框数据
    enum { IDD = IDD_WINDOWMAXIMUM_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


    // 实现
protected:
    HICON m_hIcon;

    // 生成的消息映射函数
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnMax();
    virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
    BOOL m_bFullScreenFlag;
    HWND m_hWndParent;
    WINDOWPLACEMENT m_wpPrev;
    HWND m_hWndWindow;
    CToolTipCtrl m_toolTip;
public:
    afx_msg void OnClose();
};
