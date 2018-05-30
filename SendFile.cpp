// SendFile.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChatRoomClient.h"
#include "SendFile.h"
#include "afxdialogex.h"


// SendFile �Ի���

IMPLEMENT_DYNAMIC(SendFile, CDialogEx)

SendFile::SendFile(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SENDFILE, pParent)
	, filepath(_T(""))
	, rcvText(_T("���ս��ȣ�"))
	, sendText(_T("���ͽ��ȣ�"))
	, fileSocket(NULL)
{

	peerUser = _T("");
}

SendFile::~SendFile()
{
}

void SendFile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FILEPATH, filepath);
	DDX_Text(pDX, IDC_TEXT_RCVPROGRESS2, rcvText);
	DDX_Text(pDX, IDC_TEXT_SENDPROGRESS, sendText);
	DDX_Control(pDX, IDC_RCV_PROGRESS, rcvProgress);
	DDX_Control(pDX, IDC_SEND_PROGRESS, sendProgress);
}


BEGIN_MESSAGE_MAP(SendFile, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SELECTFILE, &SendFile::OnBnClickedButtonSelectfile)
	ON_BN_CLICKED(IDOK, &SendFile::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &SendFile::OnBnClickedCancel)
END_MESSAGE_MAP()


// SendFile ��Ϣ�������


void SendFile::OnBnClickedButtonSelectfile()
{
	CFileDialog openFile(true);
	if (IDOK == openFile.DoModal())
	{
		filepath = openFile.GetPathName();
		UpdateData(false);
	}
}


void SendFile::OnBnClickedOk()
{

	if (sending)
	{
		MessageBox("���ݷ����У����Ժ����ԡ�");
		return;
	}
	UpdateData();
	if (filepath == "")
	{
		MessageBox("������Ҫ���͵��ļ�·����");
		return;
	}
	WIN32_FIND_DATA FindFileData;
	if (INVALID_HANDLE_VALUE == FindFirstFile(filepath, &FindFileData))
	{
		MessageBox("�ļ�·��������ļ�������!\n������ָ���ļ�·����");
		return;
	}
	FileBlock* sendBlock = new FileBlock;
	memset(sendBlock, 0, sizeof(FileBlock));
	sendBlock->type = MSG_SENDREQUEST;
	strcpy_s(sendBlock->file, sizeof(FindFileData.cFileName), FindFileData.cFileName);
	CString tempFilename = FindFileData.cFileName;
	sendBlock->filesize = tempFilename.GetLength() + 1;
	_itoa(FindFileData.nFileSizeLow, &sendBlock->file[sendBlock->filesize+5], 10);   //�洢�ļ��ܴ�С
	this->fileSocket->SendTo(sendBlock, sizeof(FileBlock), sendPort, sendIP);

	if (!(file = fopen(filepath, "rb")))
	{
		MessageBox("��ȡ�ļ�ʧ�ܣ�");
	}
	nSend = 0;
	nFileSize_s = FindFileData.nFileSizeLow;
	sendProgress.SetRange(0, nFileSize_s / FILESIZE + 1);
	sending = true;
}


void SendFile::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}
