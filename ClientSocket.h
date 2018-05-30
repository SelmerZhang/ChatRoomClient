#pragma once
#include "Message.h"
#include "ChatDlg.h"


class ClientSocket : public CSocket
{
public:
	ClientSocket();
	virtual ~ClientSocket();
public:
	virtual void OnReceive(int nErrorCode);   //�ͻ��˽�����Ϣ  
	void UpdateUserList(Message* rcvMsg);
	virtual void OnAccept(int nErrorCode);
	CPtrList m_List;
	CPtrList* m_pList;
	CString m_UserName;
	ChatDlg* chatWindow;   //ÿ�����û��Ի���socket��Ӧһ�����촰��
};

#pragma pack(push,1)
typedef struct file_user_info    //�洢�û���Ϣ�Ľṹ��
{
	char username[20];
	char online;
	SOCKADDR addr;
	SOCKADDR fileaddr;
}UserInfo;

extern CPtrList UserInfoList;   //ά���û���Ϣ��ȫ������
extern CPtrList offlineDlg;     //������Ϣ�Ի���
#pragma pack(pop)