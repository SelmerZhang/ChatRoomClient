#pragma once

#include "afxwin.h"


// ChatDlg 对话框

class ClientSocket;

class ChatDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ChatDlg)

public:
	ChatDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ChatDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATWND_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP();
public:
	ClientSocket* toSeverSocket;
	ClientSocket* chatSocket;
	CString peerUser;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonSend();
	void display(char* content);
	CEdit msgDisplay;
	CString sendEdit;
	CString m_caption;
	CEdit sendedit;
	virtual void OnCancel();
	afx_msg void OnBnClickedButtonSendfile();
};
