#pragma once


// RegisterDlg 对话框

class RegisterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RegisterDlg)

public:
	RegisterDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~RegisterDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REGISTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	//编辑框输入的用户名
	CString userName;
	// 编辑框输入的密码
	CString password;
	// 编辑框输入的密保答案
	CString answer;
};

BOOL GetMd5Hash(CONST BYTE *pbData, DWORD dwDataLen, CString &strHash, DWORD &err);