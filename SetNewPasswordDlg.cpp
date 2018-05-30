// SetNewPasswordDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChatRoomClient.h"
#include "SetNewPasswordDlg.h"
#include "afxdialogex.h"
#include "RegisterDlg.h"


// SetNewPasswordDlg �Ի���

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


// SetNewPasswordDlg ��Ϣ�������


void SetNewPasswordDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	//���������Md5����
	CONST BYTE* temppsw = (CONST BYTE*)newPassword.GetBuffer(40);
	CString md5Password;
	DWORD err;
	GetMd5Hash(temppsw, 40, md5Password, err);
	strcpy_s(setMsg.data.loginMsg.password, 40, md5Password);
	strcpy_s(setMsg.data.findpswMsg.userName, 20, userName);
	theApp.chatSocket->Send(&setMsg, sizeof(Message));
	CDialogEx::OnOK();
}
