#pragma once


// LoginDlg �Ի���

class LoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LoginDlg)

public:
	LoginDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~LoginDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString userName;
	CString password;
	afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnBnClickedButtonRegister();
	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnBnClickedButtonFindpsw();
	void closewnd();
};