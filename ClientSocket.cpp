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
	//收到到一个连接请求
	ClientSocket* toClientSocket;
	toClientSocket = new ClientSocket();
	toClientSocket->m_pList = &this->m_List;
	Accept(*toClientSocket);
	m_List.AddTail(toClientSocket);
	CSocket::OnAccept(nErrorCode);
}

void ClientSocket::OnReceive(int nErrorCode)
{
	char *pMsg = new char[sizeof(Message)];  //申请和Message同等大小的字符串空间并用pMsg指向它
	if (!pMsg)
	{
		TRACE0("ClientSocket::OnReceive 内存不足！");
		return;
	}
	memset(pMsg, 0, sizeof(Message));
	Receive(pMsg, sizeof(Message));    //接收消息到pMsg中
	Message* rcvMsg = (Message*)pMsg;
	switch (rcvMsg->type)	//对不同类型的消息做不同的处理
	{
	//注册成功消息
	case MSG_REGSUCCESS:
	{
		AfxMessageBox(_T("注册成功！"));
		break;
	}
	//注册失败消息
	case MSG_REGFAIL:
	{
		AfxMessageBox(_T("用户名已存在，注册失败！"));
		break;
	}
	//找回密码消息
	case MSG_FINDPSW:       //if_success中存储着找回密码的结果，0为验证失败，1为验证成功，3位设置新密码成功
	{
		if (rcvMsg->data.findpswMsg.if_success == 0)
		{
			AfxMessageBox(_T("验证失败！请检查用户名或密保答案是否有误！"));
			break;
		}
		else if (rcvMsg->data.findpswMsg.if_success == 1)
		{
			SetNewPasswordDlg* dlg;        //创建设置新密码窗口
			dlg = new SetNewPasswordDlg;
			dlg->userName = rcvMsg->data.findpswMsg.userName;
			dlg->DoModal();
			delete dlg;
			dlg = 0;
			break;
		}
		else if (rcvMsg->data.findpswMsg.if_success == 3)
		{
			AfxMessageBox(_T("新密码设置成功！"));
			break;
		}
		break;
	}
	//更新用户列表消息
	case MSG_UPDATE:
	{
		UpdateUserList(rcvMsg);
		break;
	}
	case MSG_LOGIN:
	{
		if (rcvMsg->data.loginMsg.if_success == 0)
		{
			AfxMessageBox(_T("密码或用户名错误，登录失败！"));
			break;
		}
		else
		{
			LoginDlg* temp = ((LoginDlg*)theApp.m_pMainWnd);
			temp->closewnd();
			UserListDlg* pdlg_userlist = new UserListDlg;
			pdlg_userlist->myUserName = rcvMsg->data.loginMsg.userName;
			pdlg_userlist->myUserName += _T("的用户列表");
			theApp.m_pMainWnd = pdlg_userlist;
			pdlg_userlist->DoModal();
		}
	}
	case MSG_SEND:
	{
		if (this->chatWindow == NULL)
		{
			ChatDlg* newChat = new ChatDlg;   //创建聊天对话框
			this->m_UserName = rcvMsg->from_user;
			newChat->m_caption = _T("与 ") + this->m_UserName + _T(" 的聊天窗口");
			newChat->peerUser = rcvMsg->from_user;
			newChat->chatSocket = this;
			newChat->toSeverSocket = theApp.chatSocket;
			this->chatWindow = newChat;
			//创建模态对话框
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
			if (pTemp->peerUser == rcvMsg->from_user)             //已存在该用户的离线消息对话框
			{
				pTemp->display(rcvMsg->data.chatMsg);
				exist = 1;
			}
		}
		if (exist == 1)
		{
			break;     //跳出该case
		}
		ChatDlg* offlineChat = new ChatDlg;
		offlineChat->chatSocket = NULL;
		offlineChat->peerUser = rcvMsg->from_user;
		offlineChat->m_caption = _T("来自 ") + offlineChat->peerUser + _T(" 的离线消息");
		offlineChat->Create(IDD_CHATWND_DLG);
		offlineChat->ShowWindow(SW_SHOWNORMAL);
		offlineDlg.AddTail(offlineChat);
		offlineChat->display(rcvMsg->data.chatMsg);
	}
	default: break;
	}

	CSocket::OnReceive(nErrorCode);

}


void ClientSocket::UpdateUserList(Message* rcvMsg)     //更新用户信息列表
{
	//更新链表数据
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
	//更新显示内容
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