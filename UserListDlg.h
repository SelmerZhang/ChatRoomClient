#pragma once
#include "afxwin.h"
#include "FileSocket.h"


// UserListDlg �Ի���

class UserListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(UserListDlg)

public:
	UserListDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~UserListDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USERLIST_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListBox onlineList;
	CListBox offlineList;
	afx_msg void OnBnClickedUserlistExit();
	CString myUserName;
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnDblclkUserlistOnline();
	afx_msg void OnLbnDblclkUserlistOffline();
};
