// ChatDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ChatRoomClient.h"
#include "ChatDlg.h"
#include "afxdialogex.h"
#include "SendFile.h"

// ChatDlg 对话框

IMPLEMENT_DYNAMIC(ChatDlg, CDialogEx)

ChatDlg::ChatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CHATWND_DLG, pParent)
	, sendEdit(_T(""))
{

	peerUser = _T("");
	m_caption = _T("");
}

ChatDlg::~ChatDlg()
{
}

void ChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_CHAT_DISPLAY, msgDisplay);
	//  DDX_Text(pDX, IDC_CHAT_SNEDMSG, sendEdit);
	//  DDV_MaxChars(pDX, sendEdit, 1024);
	DDX_Control(pDX, IDC_CHAT_DISPLAY, msgDisplay);
	DDX_Text(pDX, IDC_CHAT_SNEDMSG, sendEdit);
	DDX_Control(pDX, IDC_CHAT_SNEDMSG, sendedit);
}


BEGIN_MESSAGE_MAP(ChatDlg, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &ChatDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &ChatDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_SENDFILE, &ChatDlg::OnBnClickedButtonSendfile)
END_MESSAGE_MAP()


// ChatDlg 消息处理程序


BOOL ChatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetWindowText(m_caption);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void ChatDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void ChatDlg::OnBnClickedButtonSend()       //发送消息，根据对方用户状态决定发送在线消息还是离线消息
{
	UpdateData();
	if (sendEdit.IsEmpty())
	{
		AfxMessageBox(_T("发送内容不能为空！"));
		return;
	}
	if (this->chatSocket == NULL)
	{
		AfxMessageBox(_T("请关闭本窗口并重新选择用户"));
		return;
	}
	Message* sendMsg = new Message;
	strcpy_s(sendMsg->from_user, 20, theApp.m_userName);
	strcpy_s(sendMsg->to_user, 20, peerUser);
	CString temp;
	CTime time = CTime::GetCurrentTime();
	temp = time.Format("%H:%M:%S");
	//姓名 +_T("\n\t") 时间
	sendEdit = theApp.m_userName + _T(" ： ") + _T("  ") + temp + _T("\r\n   ") + sendEdit + _T("\r\n");
	strcpy_s(sendMsg->data.chatMsg, 1024, sendEdit);
	int if_online = -1;
	POSITION ps = UserInfoList.GetHeadPosition();
	while (ps != NULL)
	{
		UserInfo* pTemp = (UserInfo*)UserInfoList.GetNext(ps);
		if ((CString)(pTemp->username) == this->peerUser)
		{
			if_online = pTemp->online;
			break;
		}
	}
	if (if_online == 1)      //对方在线，发送在线消息
	{
		sendMsg->type = MSG_SEND;
		this->chatSocket->Send(sendMsg, sizeof(Message));
		sendedit.SetWindowTextA("");
		display(sendMsg->data.chatMsg);
	}
	else if(if_online == 0)   //对方离线，发送离线消息
	{
		sendMsg->type = MSG_OFFLINE;
		this->toSeverSocket->Send(sendMsg, sizeof(Message));
		sendedit.SetWindowTextA("");
		display(sendMsg->data.chatMsg);
	}
	else if (if_online == -1)   //对方离线，发送离线消息
	{

		AfxMessageBox(_T("用户信息出错！"));
	}
	return;
}


void ChatDlg::display(char* content)
{
	msgDisplay.ReplaceSel(content);
}

void ChatDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	this->DestroyWindow();
	CDialogEx::OnCancel();
}


void ChatDlg::OnBnClickedButtonSendfile()
{
	// TODO: 在此添加控件通知处理程序代码
	SendFile* sendFileDlg = new SendFile;
	sendFileDlg->peerUser = this->peerUser;
	sendFileDlg->fileSocket = theApp.fileSocket;
	sendFileDlg->sending = 0;
	//找到对应用户信息,获取其发送文件地址
	SOCKADDR peerSockaddr;
	POSITION ps = UserInfoList.GetHeadPosition();
	while (ps != NULL)
	{
		UserInfo* pTemp = (UserInfo*)UserInfoList.GetNext(ps);
		if ((CString)(pTemp->username) == peerUser)
		{
			peerSockaddr = pTemp->fileaddr;
			break;
		}
	}
	SOCKADDR_IN* temp1;
	temp1 = (SOCKADDR_IN*)&peerSockaddr;
	sendFileDlg->sendIP = new char[20];
	inet_ntop(AF_INET, &temp1->sin_addr, sendFileDlg->sendIP, 16);
	sendFileDlg->sendPort = temp1->sin_port;
	theApp.fileSocket->sendFileDlg = sendFileDlg;
	sendFileDlg->fileSocket = theApp.fileSocket;
	sendFileDlg->DoModal();
}
