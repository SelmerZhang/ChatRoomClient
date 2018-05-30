// UserListDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChatRoomClient.h"
#include "UserListDlg.h"
#include "afxdialogex.h"
#include "ChatDlg.h"


// UserListDlg �Ի���

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


// UserListDlg ��Ϣ�������


void UserListDlg::OnBnClickedUserlistExit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->OnCancel();
}


BOOL UserListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetWindowText(myUserName);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void UserListDlg::OnLbnDblclkUserlistOnline()     //˫��ĳ�����û��������촰��
{
	UpdateData();
	INT nIndex = 0;
	CString peerName;
	nIndex = onlineList.GetCurSel();
	onlineList.GetText(nIndex, peerName);   //ȷ�����շ�����
	ClientSocket* chatSocket = new ClientSocket;  //���������׽���
	chatSocket->m_UserName = peerName;
	if (!chatSocket)
	{
		AfxMessageBox(_T("�ڴ治�㣡"));
		return;
	}
	if (!chatSocket->Create())
	{
		AfxMessageBox(_T("�����׽���ʧ�ܣ�"));
		return;
	}
	//�ҵ���Ӧ�û���Ϣ,��ȡ���ַ
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
	if (!chatSocket->Connect(ipaddr, port))   //���Ӷ�Ӧ�û�
	{
		AfxMessageBox(_T("�����û�ʧ�ܣ�"));
		return;
	}
	ChatDlg* newChat = new ChatDlg;   //��������Ի���
	newChat->m_caption = _T("��") + peerName + _T(" �����촰��");
	newChat->peerUser = peerName;
	newChat->chatSocket = chatSocket;
	newChat->toSeverSocket = theApp.chatSocket;
	chatSocket->chatWindow = newChat;
	//����ģ̬�Ի���
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
	offlineList.GetText(nIndex, peerName);   //ȷ�����շ�����
	ChatDlg* newChat = new ChatDlg;   //��������Ի���
	newChat->m_caption = _T("��") + peerName + _T(" �����촰��");
	newChat->peerUser = peerName;
	newChat->chatSocket = theApp.chatSocket;
	newChat->toSeverSocket = theApp.chatSocket;
	//����ģ̬�Ի���
	newChat->Create(IDD_CHATWND_DLG);
	newChat->ShowWindow(SW_SHOWNORMAL);
	return;
}