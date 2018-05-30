#pragma once


// FindPasswordDlg 对话框

class FindPasswordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(FindPasswordDlg)

public:
	FindPasswordDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~FindPasswordDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FINDPASSWORD_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString userName;
	CString pswAnswer;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
