// FindPasswordDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChatRoomClient.h"
#include "FindPasswordDlg.h"
#include "afxdialogex.h"
#include "RegisterDlg.h"

// FindPasswordDlg �Ի���

IMPLEMENT_DYNAMIC(FindPasswordDlg, CDialogEx)

FindPasswordDlg::FindPasswordDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FINDPASSWORD_DLG, pParent)
	, userName(_T(""))
	, pswAnswer(_T(""))
{

}

FindPasswordDlg::~FindPasswordDlg()
{
}

void FindPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FINDPSW_USERNAME, userName);
	DDV_MaxChars(pDX, userName, 20);
	DDX_Text(pDX, IDC_FINDPSW_PSWANSWER, pswAnswer);
	DDV_MaxChars(pDX, pswAnswer, 20);
}


BEGIN_MESSAGE_MAP(FindPasswordDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &FindPasswordDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &FindPasswordDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// FindPasswordDlg ��Ϣ�������


void FindPasswordDlg::OnBnClickedOk()
{
	UpdateData();
	Message findpswMsg;
	memset(&findpswMsg, 0, sizeof(Message));
	findpswMsg.type = MSG_FINDPSW;
	strcpy_s(findpswMsg.data.findpswMsg.userName, 20, userName);
	//���ܱ��𰸽���Md5����
	CONST BYTE* tempanswer = (CONST BYTE*)pswAnswer.GetBuffer(40);
	CString md5Answer;
	DWORD err;
	GetMd5Hash(tempanswer, 40, md5Answer, err);
	strcpy_s(findpswMsg.data.findpswMsg.answer, 40, md5Answer);
	theApp.chatSocket->Send(&findpswMsg, sizeof(Message));
	CDialogEx::OnOK();
}


void FindPasswordDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}
