#pragma once
#include "afxcmn.h"
#include "FileSocket.h"

// SendFile �Ի���

class FileSocket;

class SendFile : public CDialogEx
{
	DECLARE_DYNAMIC(SendFile)

public:
	SendFile(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SendFile();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SENDFILE };
#endif

public:
	char* sendIP;
	UINT sendPort;
	int nFileSize_s;     //���ͷ��ļ���С
	int nRecv;           //�ѽ��յ��ļ�������
	int nSend;			 //�ѷ��͵��ļ�������
	int nFileSize;   //���շ��ļ���С
	bool sending;
	FILE* file;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
