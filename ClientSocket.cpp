// ClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "ClientSocket.h"
#include "ChatRoomClient.h"
#include "SetNewPasswordDlg.h"
#include "UserListDlg.h"
#include "LoginDlg.h"
#include "FileSocket.h"
// ClientSocket

ClientSocket::ClientSocket()
	:m_UserName(_T("")),
	chatWindow(NULL)
{

}

ClientSocket::~ClientSocket()
{

}

void ClientSocket::OnAccept(int nErrorCode)
{
	//�յ���һ����������
	ClientSocket* toClientSocket;
	toClientSocket = new ClientSocket();
	toClientSocket->m_pList = &this->m_List;
	Accept(*toClientSocket);
	m_List.AddTail(toClientSocket);
	CSocket::OnAccept(nErrorCode);
}

void ClientSocket::OnReceive(int nErrorCode)
{
	char *pMsg = new char[sizeof(Message)];  //�����Messageͬ�ȴ�С���ַ����ռ䲢��pMsgָ����
	if (!pMsg)
	{
		TRACE0("ClientSocket::OnReceive �ڴ治�㣡");
		return;
	}
	memset(pMsg, 0, sizeof(Message));
	Receive(pMsg, sizeof(Message));    //������Ϣ��pMsg��
	Message* rcvMsg = (Message*)pMsg;
	switch (rcvMsg->type)	//�Բ�ͬ���͵���Ϣ����ͬ�Ĵ���
	{
	//ע��ɹ���Ϣ
	case MSG_REGSUCCESS:
	{
		AfxMessageBox(_T("ע��ɹ���"));
		break;
	}
	//ע��ʧ����Ϣ
	case MSG_REGFAIL:
	{
		AfxMessageBox(_T("�û����Ѵ��ڣ�ע��ʧ�ܣ�"));
		break;
	}
	//�һ�������Ϣ
	case MSG_FINDPSW:       //if_success�д洢���һ�����Ľ����0Ϊ��֤ʧ�ܣ�1Ϊ��֤�ɹ���3λ����������ɹ�
	{
		if (rcvMsg->data.findpswMsg.if_success == 0)
		{
			AfxMessageBox(_T("��֤ʧ�ܣ������û������ܱ����Ƿ�����"));
			break;
		}
		else if (rcvMsg->data.findpswMsg.if_success == 1)
		{
			SetNewPasswordDlg* dlg;        //�������������봰��
			dlg = new SetNewPasswordDlg;
			dlg->userName = rcvMsg->data.findpswMsg.userName;
			dlg->DoModal();
			delete dlg;
			dlg = 0;
			break;
		}
		else if (rcvMsg->data.findpswMsg.if_success == 3)
		{
			AfxMessageBox(_T("���������óɹ���"));
			break;
		}
		break;
	}
	//�����û��б���Ϣ
	case MSG_UPDATE:
	{
		UpdateUserList(rcvMsg);
		break;
	}
	case MSG_LOGIN:
	{
		if (rcvMsg->data.loginMsg.if_success == 0)
		{
			AfxMessageBox(_T("������û������󣬵�¼ʧ�ܣ�"));
			break;
		}
		else
		{
			LoginDlg* temp = ((LoginDlg*)theApp.m_pMainWnd);
			temp->closewnd();
			UserListDlg* pdlg_userlist = new UserListDlg;
			pdlg_userlist->myUserName = rcvMsg->data.loginMsg.userName;
			pdlg_userlist->myUserName += _T("���û��б�");
			theApp.m_pMainWnd = pdlg_userlist;
			pdlg_userlist->DoModal();
		}
	}
	case MSG_SEND:
	{
		if (this->chatWindow == NULL)
		{
			ChatDlg* newChat = new ChatDlg;   //��������Ի���
			this->m_UserName = rcvMsg->from_user;
			newChat->m_caption = _T("�� ") + this->m_UserName + _T(" �����촰��");
			newChat->peerUser = rcvMsg->from_user;
			newChat->chatSocket = this;
			newChat->toSeverSocket = theApp.chatSocket;
			this->chatWindow = newChat;
			//����ģ̬�Ի���
			newChat->Create(IDD_CHATWND_DLG);
			newChat->ShowWindow(SW_SHOWNORMAL);
			newChat->display(rcvMsg->data.chatMsg);
		}
		else if (this->chatWindow != NULL)
		{
			this->chatWindow->display(rcvMsg->data.chatMsg);
		}
		break;
	}
	case MSG_OFFLINE:
	{
		POSITION ps = offlineDlg.GetHeadPosition();
		int exist = 0;
		while (ps != NULL)
		{
			ChatDlg* pTemp = (ChatDlg*)UserInfoList.GetNext(ps);
			if (pTemp->peerUser == rcvMsg->from_user)             //�Ѵ��ڸ��û���������Ϣ�Ի���
			{
				pTemp->display(rcvMsg->data.chatMsg);
				exist = 1;
			}
		}
		if (exist == 1)
		{
			break;     //������case
		}
		ChatDlg* offlineChat = new ChatDlg;
		offlineChat->chatSocket = NULL;
		offlineChat->peerUser = rcvMsg->from_user;
		offlineChat->m_caption = _T("���� ") + offlineChat->peerUser + _T(" ��������Ϣ");
		offlineChat->Create(IDD_CHATWND_DLG);
		offlineChat->ShowWindow(SW_SHOWNORMAL);
		offlineDlg.AddTail(offlineChat);
		offlineChat->display(rcvMsg->data.chatMsg);
	}
	default: break;
	}

	CSocket::OnReceive(nErrorCode);

}


void ClientSocket::UpdateUserList(Message* rcvMsg)     //�����û���Ϣ�б�
{
	//������������
	int count = rcvMsg->data.userinfoMsg.count;
	UserInfo* userinfoarray = new UserInfo[count];
	memcpy_s(userinfoarray, count * sizeof(UserInfo), rcvMsg->data.userinfoMsg.userinfo, count * sizeof(UserInfo));
	int i = 0;
	UserInfoList.RemoveAll();
	UserInfo* pTemp;
	for (; i < count; i++)
	{
		pTemp = new UserInfo;
		*pTemp = userinfoarray[i];
		UserInfoList.AddTail(pTemp);
	}
	pTemp = 0;
	//������ʾ����
	CString strTmp;
	UserListDlg* userdlg = (UserListDlg*)theApp.m_pMainWnd;
	userdlg->UpdateData();
	userdlg->onlineList.ResetContent();
	userdlg->offlineList.ResetContent();
	POSITION ps = UserInfoList.GetHeadPosition();
	while (ps != NULL)
	{
		UserInfo* pTemp = (UserInfo*)UserInfoList.GetNext(ps);
		if (pTemp->online == 1)
		{
			userdlg->onlineList.AddString(pTemp->username);
		}
		else if(pTemp->online == 0)
		{
			userdlg->offlineList.AddString(pTemp->username);
		}
	}
}