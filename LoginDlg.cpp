// LoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ChatRoomClient.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "RegisterDlg.h"
#include "FindPasswordDlg.h"


// LoginDlg 对话框

IMPLEMENT_DYNAMIC(LoginDlg, CDialogEx)

LoginDlg::LoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOGIN, pParent)
	, userName(_T(""))
	, password(_T(""))
{

}

LoginDlg::~LoginDlg()
{
}

void LoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, userName);
	DDV_MaxChars(pDX, userName, 20);
	DDX_Text(pDX, IDC_EDIT2, password);
	DDV_MaxChars(pDX, password, 20);
}


BEGIN_MESSAGE_MAP(LoginDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &LoginDlg::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_REGISTER, &LoginDlg::OnBnClickedButtonRegister)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &LoginDlg::OnBnClickedButtonQuit)
	ON_BN_CLICKED(IDC_BUTTON_FINDPSW, &LoginDlg::OnBnClickedButtonFindpsw)
END_MESSAGE_MAP()


// LoginDlg 消息处理程序



void LoginDlg::OnBnClickedButtonLogin()
{
	UpdateData();
	Message* sendMsg = new Message;
	memset(sendMsg, 0, sizeof(Message));
	sendMsg->type = MSG_LOGIN;
	strcpy_s(sendMsg->data.loginMsg.userName, 20, userName);
	//对密码进行Md5加密
	CONST BYTE* temppsw = (CONST BYTE*)password.GetBuffer(40);
	CString md5Password;
	DWORD err;
	GetMd5Hash(temppsw, 40, md5Password, err);
	strcpy_s(sendMsg->data.loginMsg.password, 40, md5Password);
	sendMsg->data.loginMsg.addr = *(theApp.listenAddr);
	sendMsg->data.loginMsg.fileaddr = *(theApp.fileAddr);
	theApp.chatSocket->Send(sendMsg, sizeof(Message));
	theApp.m_userName = userName;
}


void LoginDlg::OnBnClickedButtonRegister()
{
	RegisterDlg* dlg;
	dlg = new RegisterDlg();
	dlg->DoModal();
	delete dlg;
	dlg = 0;
}


void LoginDlg::OnBnClickedButtonQuit()
{
	this->OnCancel();
}


void LoginDlg::OnBnClickedButtonFindpsw()
{
	FindPasswordDlg* dlg;
	dlg = new FindPasswordDlg;
	dlg->DoModal();
	delete dlg;
	dlg = 0;
}

void LoginDlg::closewnd()
{
	this->OnOK();
}

