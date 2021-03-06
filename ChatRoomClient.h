
// ChatRoomClient.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "ClientSocket.h"
#include "ChatRoomClientDlg.h"
#include "FileSocket.h"


// CChatRoomClientApp:
// 有关此类的实现，请参阅 ChatRoomClient.cpp
//

class CChatRoomClientApp : public CWinApp
{
public:
	CChatRoomClientApp();

// 重写
public:
	virtual BOOL InitInstance();
// 实现
	DECLARE_MESSAGE_MAP()
public:
	ClientSocket* chatSocket;
	ClientSocket* listenSocket;
	FileSocket* fileSocket;
	SOCKADDR* listenAddr;
	SOCKADDR* fileAddr;
	CString IPAddr;
	UINT port;
	UINT fileport;
	CString m_userName;
public:
	ClientSocket* GetChatSocket();
	ClientSocket* GetListenSocket();

public:
	virtual int ExitInstance();
};

extern CChatRoomClientApp theApp;
