// SendFile.cpp : 实现文件
//

#include "stdafx.h"
#include "ChatRoomClient.h"
#include "SendFile.h"
#include "afxdialogex.h"


// SendFile 对话框

IMPLEMENT_DYNAMIC(SendFile, CDialogEx)

SendFile::SendFile(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SENDFILE, pParent)
	, filepath(_T(""))
	, rcvText(_T("接收进度："))
	, sendText(_T("发送进度："))
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


// SendFile 消息处理程序


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
		MessageBox("数据发送中，请稍候再试。");
		return;
	}
	UpdateData();
	if (filepath == "")
	{
		MessageBox("请输入要发送的文件路径！");
		return;
	}
	WIN32_FIND_DATA FindFileData;
	if (INVALID_HANDLE_VALUE == FindFirstFile(filepath, &FindFileData))
	{
		MessageBox("文件路径错误或文件不存在!\n请重新指定文件路径。");
		return;
	}
	FileBlock* sendBlock = new FileBlock;
	memset(sendBlock, 0, sizeof(FileBlock));
	sendBlock->type = MSG_SENDREQUEST;
	strcpy_s(sendBlock->file, sizeof(FindFileData.cFileName), FindFileData.cFileName);
	CString tempFilename = FindFileData.cFileName;
	sendBlock->filesize = tempFilename.GetLength() + 1;
	_itoa(FindFileData.nFileSizeLow, &sendBlock->file[sendBlock->filesize+5], 10);   //存储文件总大小
	this->fileSocket->SendTo(sendBlock, sizeof(FileBlock), sendPort, sendIP);

	if (!(file = fopen(filepath, "rb")))
	{
		MessageBox("读取文件失败！");
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
