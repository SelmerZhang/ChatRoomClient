// ChatDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChatRoomClient.h"
#include "ChatDlg.h"
#include "afxdialogex.h"
#include "SendFile.h"

// ChatDlg �Ի���

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


// ChatDlg ��Ϣ�������


BOOL ChatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetWindowText(m_caption);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void ChatDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


void ChatDlg::OnBnClickedButtonSend()       //������Ϣ�����ݶԷ��û�״̬��������������Ϣ����������Ϣ
{
	UpdateData();
	if (sendEdit.IsEmpty())
	{
		AfxMessageBox(_T("�������ݲ���Ϊ�գ�"));
		return;
	}
	if (this->chatSocket == NULL)
	{
		AfxMessageBox(_T("��رձ����ڲ�����ѡ���û�"));
		return;
	}
	Message* sendMsg = new Message;
	strcpy_s(sendMsg->from_user, 20, theApp.m_userName);
	strcpy_s(sendMsg->to_user, 20, peerUser);
	CString temp;
	CTime time = CTime::GetCurrentTime();
	temp = time.Format("%H:%M:%S");
	//���� +_T("\n\t") ʱ��
	sendEdit = theApp.m_userName + _T(" �� ") + _T("  ") + temp + _T("\r\n   ") + sendEdit + _T("\r\n");
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
	if (if_online == 1)      //�Է����ߣ�����������Ϣ
	{
		sendMsg->type = MSG_SEND;
		this->chatSocket->Send(sendMsg, sizeof(Message));
		sendedit.SetWindowTextA("");
		display(sendMsg->data.chatMsg);
	}
	else if(if_online == 0)   //�Է����ߣ�����������Ϣ
	{
		sendMsg->type = MSG_OFFLINE;
		this->toSeverSocket->Send(sendMsg, sizeof(Message));
		sendedit.SetWindowTextA("");
		display(sendMsg->data.chatMsg);
	}
	else if (if_online == -1)   //�Է����ߣ�����������Ϣ
	{

		AfxMessageBox(_T("�û���Ϣ����"));
	}
	return;
}


void ChatDlg::display(char* content)
{
	msgDisplay.ReplaceSel(content);
}

void ChatDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	this->DestroyWindow();
	CDialogEx::OnCancel();
}


void ChatDlg::OnBnClickedButtonSendfile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SendFile* sendFileDlg = new SendFile;
	sendFileDlg->peerUser = this->peerUser;
	sendFileDlg->fileSocket = theApp.fileSocket;
	sendFileDlg->sending = 0;
	//�ҵ���Ӧ�û���Ϣ,��ȡ�䷢���ļ���ַ
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
