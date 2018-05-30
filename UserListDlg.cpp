// UserListDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ChatRoomClient.h"
#include "UserListDlg.h"
#include "afxdialogex.h"
#include "ChatDlg.h"


// UserListDlg 对话框

IMPLEMENT_DYNAMIC(UserListDlg, CDialogEx)

UserListDlg::UserListDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_USERLIST_DLG, pParent)
{
	myUserName = _T("");
}

UserListDlg::~UserListDlg()
{
}

void UserListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_USERLIST_ONLINE, onlineList);
	DDX_Control(pDX, IDC_USERLIST_OFFLINE, offlineList);
}


BEGIN_MESSAGE_MAP(UserListDlg, CDialogEx)
	ON_BN_CLICKED(IDC_USERLIST_EXIT, &UserListDlg::OnBnClickedUserlistExit)
	ON_LBN_DBLCLK(IDC_USERLIST_ONLINE, &UserListDlg::OnLbnDblclkUserlistOnline)
	ON_LBN_DBLCLK(IDC_USERLIST_OFFLINE, &UserListDlg::OnLbnDblclkUserlistOffline)
END_MESSAGE_MAP()


// UserListDlg 消息处理程序


void UserListDlg::OnBnClickedUserlistExit()
{
	// TODO: 在此添加控件通知处理程序代码
	this->OnCancel();
}


BOOL UserListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetWindowText(myUserName);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void UserListDlg::OnLbnDblclkUserlistOnline()     //双击某在线用户创建聊天窗口
{
	UpdateData();
	INT nIndex = 0;
	CString peerName;
	nIndex = onlineList.GetCurSel();
	onlineList.GetText(nIndex, peerName);   //确定接收方姓名
	ClientSocket* chatSocket = new ClientSocket;  //创建聊天套接字
	chatSocket->m_UserName = peerName;
	if (!chatSocket)
	{
		AfxMessageBox(_T("内存不足！"));
		return;
	}
	if (!chatSocket->Create())
	{
		AfxMessageBox(_T("创建套接字失败！"));
		return;
	}
	//找到对应用户信息,获取其地址
	SOCKADDR peerSockaddr;
	POSITION ps = UserInfoList.GetHeadPosition();
	while (ps != NULL)
	{
		UserInfo* pTemp = (UserInfo*)UserInfoList.GetNext(ps);
		if ((CString)(pTemp->username) == peerName)
		{
			peerSockaddr = pTemp->addr;
			break;
		}
	}
	SOCKADDR_IN* temp1;
	temp1 = (SOCKADDR_IN*)&peerSockaddr;
	char* ipaddr = new char[20];
	inet_ntop(AF_INET, &temp1->sin_addr, ipaddr, 16);
	UINT port = temp1->sin_port;
	if (!chatSocket->Connect(ipaddr, port))   //连接对应用户
	{
		AfxMessageBox(_T("连接用户失败！"));
		return;
	}
	ChatDlg* newChat = new ChatDlg;   //创建聊天对话框
	newChat->m_caption = _T("与") + peerName + _T(" 的聊天窗口");
	newChat->peerUser = peerName;
	newChat->chatSocket = chatSocket;
	newChat->toSeverSocket = theApp.chatSocket;
	chatSocket->chatWindow = newChat;
	//创建模态对话框
	newChat->Create(IDD_CHATWND_DLG);
	newChat->ShowWindow(SW_SHOWNORMAL);
	return;
}


void UserListDlg::OnLbnDblclkUserlistOffline()
{
	UpdateData();
	INT nIndex = 0;
	CString peerName;
	nIndex = offlineList.GetCurSel();
	offlineList.GetText(nIndex, peerName);   //确定接收方姓名
	ChatDlg* newChat = new ChatDlg;   //创建聊天对话框
	newChat->m_caption = _T("与") + peerName + _T(" 的聊天窗口");
	newChat->peerUser = peerName;
	newChat->chatSocket = theApp.chatSocket;
	newChat->toSeverSocket = theApp.chatSocket;
	//创建模态对话框
	newChat->Create(IDD_CHATWND_DLG);
	newChat->ShowWindow(SW_SHOWNORMAL);
	return;
}