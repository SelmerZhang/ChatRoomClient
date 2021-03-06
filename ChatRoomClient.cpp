
// ChatRoomClient.cpp: 定义应用程序的类行为。
//

#include "stdafx.h"
#include "ChatRoomClient.h"
#include "ChatRoomClientDlg.h"
#include "ClientSocket.h"
#include "LoginDlg.h"
#include "UserListDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChatRoomClientApp

BEGIN_MESSAGE_MAP(CChatRoomClientApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CChatRoomClientApp 构造

CChatRoomClientApp::CChatRoomClientApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CChatRoomClientApp 对象

CChatRoomClientApp theApp;

//记录用户信息的链表
CPtrList UserInfoList;

//离线消息对话框的链表
CPtrList offlineDlg;   

// CChatRoomClientApp 初始化

BOOL CChatRoomClientApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("ChatRoom"));

	//初始化winSock库,成功则返回非0否则返回0
	WSAData wsData;
	if (!AfxSocketInit(&wsData))  //AfxSocketInit()调用WSAStartup( )
	{
		AfxMessageBox(_T("Socket 库初始化出错！"));
		return false;
	}

	//初始化客户端聊天Socket
	chatSocket = new ClientSocket();
	if (!chatSocket)
	{
		AfxMessageBox(_T("内存不足！"));
		return false;
	}
	if (!chatSocket->Create())
	{
		AfxMessageBox(_T("创建套接字失败！"));
		return false;
	}


	//创建客户器端监听Socket
	listenSocket = new ClientSocket();
	if (!listenSocket)
	{
		AfxMessageBox(_T("动态创建服务器监听套接字出错！"));
		return false;
	}
	if (!listenSocket->Create())
	{
		AfxMessageBox(_T("创建套接字错误！"));
		listenSocket->Close();
		return false;
	}
	if (!listenSocket->Listen())
	{
		AfxMessageBox(_T("监听失败！"));
		listenSocket->Close();
		return false;
	}
	listenAddr = new SOCKADDR;
	memset(listenAddr, 0, sizeof(SOCKADDR));
	if (!listenSocket->GetSockName(IPAddr, port))
	{
		AfxMessageBox(_T("获取本机地址失败！"));
	}
	char buff[100];
	char *b, *bu;
	HOSTENT * host;
	gethostname(buff, 100);
	host = gethostbyname(buff);
	b = host->h_addr_list[0];				 //选择不同网卡
	bu = inet_ntoa(*(struct in_addr *)b);
	IPAddr = bu;
	SOCKADDR_IN* tempAddr = new SOCKADDR_IN;
	memset(tempAddr, 0, sizeof(SOCKADDR_IN));
	tempAddr->sin_family = AF_INET;
	tempAddr->sin_port = port;
	tempAddr->sin_addr.S_un.S_addr = inet_addr(bu);
	delete listenAddr;
	listenAddr = (SOCKADDR*)tempAddr;
	CString fileIP = bu;


	//创建文件传输Socket，基于UDP
	fileSocket = new FileSocket;
	if (!fileSocket)
	{
		AfxMessageBox(_T("内存不足！"));
		return false;
	}
	if (!fileSocket->Create(0U, SOCK_DGRAM, 0))
	{
		AfxMessageBox(_T("创建套接字失败！"));
		return false;
	}
	fileAddr = new SOCKADDR;
	memset(fileAddr, 0, sizeof(SOCKADDR));
	if (!fileSocket->GetSockName(IPAddr, fileport))
	{
		AfxMessageBox(_T("获取本机地址失败！"));
	}
	tempAddr = new SOCKADDR_IN;
	memset(tempAddr, 0, sizeof(SOCKADDR_IN));
	tempAddr->sin_family = AF_INET;
	tempAddr->sin_port = fileport;
	tempAddr->sin_addr.S_un.S_addr = inet_addr(fileIP);
	delete fileAddr;
	fileAddr = (SOCKADDR*)tempAddr;


	CChatRoomClientDlg* pdlg_connect;        //打开连接服务器对话框
	pdlg_connect = new CChatRoomClientDlg;
	INT_PTR nResponse = pdlg_connect->DoModal();
	if (nResponse == IDOK)
	{
		LoginDlg* pdlg_login;
		pdlg_login = new  LoginDlg;
		m_pMainWnd = pdlg_login;
		nResponse = pdlg_login->DoModal();
		if (nResponse == IDOK)
		{
			//登录成功
		}
		else if (nResponse == IDCANCEL)
		{
			//退出程序
			delete pdlg_login;
			chatSocket->Close();
			listenSocket->Close();
			return FALSE;
		}
	}
	else if (nResponse == IDCANCEL)
	{
		delete pdlg_connect;
		chatSocket->Close();
		listenSocket->Close();
		return FALSE;
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}


	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

ClientSocket* CChatRoomClientApp::GetChatSocket()
{
	return chatSocket;
}

ClientSocket* CChatRoomClientApp::GetListenSocket()
{
	return listenSocket;
}

int CChatRoomClientApp::ExitInstance()
{
	if (chatSocket)
	{
		delete chatSocket;
		chatSocket = NULL;
	}

	return CWinApp::ExitInstance();
}