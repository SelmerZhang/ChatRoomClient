#pragma once
#include "afxcmn.h"
#include "FileSocket.h"

// SendFile 对话框

class FileSocket;

class SendFile : public CDialogEx
{
	DECLARE_DYNAMIC(SendFile)

public:
	SendFile(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SendFile();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SENDFILE };
#endif

public:
	char* sendIP;
	UINT sendPort;
	int nFileSize_s;     //发送方文件大小
	int nRecv;           //已接收的文件块数量
	int nSend;			 //已发送的文件块数量
	int nFileSize;   //接收方文件大小
	bool sending;
	FILE* file;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString peerUser;
	FileSocket* fileSocket;
	CString filepath;
	CString rcvText;
	CString sendText;
	CProgressCtrl rcvProgress;
	CProgressCtrl sendProgress;
	afx_msg void OnBnClickedButtonSelectfile();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
