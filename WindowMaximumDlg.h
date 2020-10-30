
// WindowMaximumDlg.h : ͷ�ļ�
//

#pragma once


// CWindowMaximumDlg �Ի���
class CWindowMaximumDlg : public CDialogEx
{
    // ����
public:
    CWindowMaximumDlg(CWnd* pParent = NULL);	// ��׼���캯��

    // �Ի�������
    enum { IDD = IDD_WINDOWMAXIMUM_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


    // ʵ��
protected:
    HICON m_hIcon;

    // ���ɵ���Ϣӳ�亯��
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
