#pragma once
#include "Message.h"
#include "ChatDlg.h"


class ClientSocket : public CSocket
{
public:
	ClientSocket();
	virtual ~ClientSocket();
public:
	virtual void OnReceive(int nErrorCode);   //客户端接收消息  
	void UpdateUserList(Message* rcvMsg);
	virtual void OnAccept(int nErrorCode);
	CPtrList m_List;
	CPtrList* m_pList;
	CString m_UserName;
	ChatDlg* chatWindow;   //每个与用户对话的socket对应一个聊天窗口
};

#pragma pack(push,1)
typedef struct file_user_info    //存储用户信息的结构体
{
	char username[20];
	char online;
	SOCKADDR addr;
	SOCKADDR fileaddr;
}UserInfo;

extern CPtrList UserInfoList;   //维护用户信息的全局链表
extern CPtrList offlineDlg;     //离线消息对话框
#pragma pack(pop)