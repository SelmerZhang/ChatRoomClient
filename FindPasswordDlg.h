#pragma once


// FindPasswordDlg �Ի���

class FindPasswordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(FindPasswordDlg)

public:
	FindPasswordDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~FindPasswordDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FINDPASSWORD_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString userName;
	CString pswAnswer;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
