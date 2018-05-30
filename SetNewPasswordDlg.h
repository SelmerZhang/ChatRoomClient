#pragma once


// SetNewPasswordDlg 对话框

class SetNewPasswordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SetNewPasswordDlg)

public:
	SetNewPasswordDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SetNewPasswordDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETNEWPSW_Dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	CString newPassword;
	CString userName;
};
