#pragma once


// RegisterDlg �Ի���

class RegisterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RegisterDlg)

public:
	RegisterDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~RegisterDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REGISTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	//�༭��������û���
	CString userName;
	// �༭�����������
	CString password;
	// �༭��������ܱ���
	CString answer;
};

BOOL GetMd5Hash(CONST BYTE *pbData, DWORD dwDataLen, CString &strHash, DWORD &err);