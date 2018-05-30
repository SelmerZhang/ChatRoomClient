// SetNewPasswordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ChatRoomClient.h"
#include "SetNewPasswordDlg.h"
#include "afxdialogex.h"
#include "RegisterDlg.h"


// SetNewPasswordDlg 对话框

IMPLEMENT_DYNAMIC(SetNewPasswordDlg, CDialogEx)

SetNewPasswordDlg::SetNewPasswordDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SETNEWPSW_Dlg, pParent)
	, newPassword(_T(""))
{

	userName = _T("");
}

SetNewPasswordDlg::~SetNewPasswordDlg()
{
}

void SetNewPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, newPassword);
	DDV_MaxChars(pDX, newPassword, 20);
}


BEGIN_MESSAGE_MAP(SetNewPasswordDlg, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &SetNewPasswordDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &SetNewPasswordDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// SetNewPasswordDlg 消息处理程序


void SetNewPasswordDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void SetNewPasswordDlg::OnBnClickedOk()
{
	UpdateData();
	Message setMsg;
	memset(&setMsg, 0, sizeof(Message));
	setMsg.type = MSG_FINDPSW;
	setMsg.data.findpswMsg.userName,
	setMsg.data.findpswMsg.if_success = 2;
	//对密码进行Md5加密
	CONST BYTE* temppsw = (CONST BYTE*)newPassword.GetBuffer(40);
	CString md5Password;
	DWORD err;
	GetMd5Hash(temppsw, 40, md5Password, err);
	strcpy_s(setMsg.data.loginMsg.password, 40, md5Password);
	strcpy_s(setMsg.data.findpswMsg.userName, 20, userName);
	theApp.chatSocket->Send(&setMsg, sizeof(Message));
	CDialogEx::OnOK();
}
