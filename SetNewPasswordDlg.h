#pragma once


// SetNewPasswordDlg �Ի���

class SetNewPasswordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SetNewPasswordDlg)

public:
	SetNewPasswordDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SetNewPasswordDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETNEWPSW_Dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	CString newPassword;
	CString userName;
};
