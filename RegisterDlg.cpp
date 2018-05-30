// RegisterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ChatRoomClient.h"
#include "RegisterDlg.h"
#include "afxdialogex.h"
#include <wincrypt.h>

// RegisterDlg 对话框

IMPLEMENT_DYNAMIC(RegisterDlg, CDialogEx)

RegisterDlg::RegisterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_REGISTER, pParent)
	, userName(_T(""))
	, password(_T(""))
	, answer(_T(""))
{

}

RegisterDlg::~RegisterDlg()
{
}

void RegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_REG_USERNAME, userName);
	DDV_MaxChars(pDX, userName, 20);
	DDX_Text(pDX, IDC_REG_PASSWORD, password);
	DDV_MaxChars(pDX, password, 20);
	DDX_Text(pDX, IDC_REG_ANSWER, answer);
	DDV_MaxChars(pDX, answer, 20);
}


BEGIN_MESSAGE_MAP(RegisterDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &RegisterDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// RegisterDlg 消息处理程序


void RegisterDlg::OnBnClickedOk()
{
	UpdateData();
	Message regMsg;
	memset(&regMsg, 0, sizeof(Message));
	regMsg.type = MSG_REGISTER;
	strcpy_s(regMsg.data.regMsg.userName, 20, userName);
	//对密码进行Md5加密
	CONST BYTE* temppsw = (CONST BYTE*)password.GetBuffer(40);
	CString md5Password;
	DWORD err;
	GetMd5Hash(temppsw, 40, md5Password, err);
	strcpy_s(regMsg.data.loginMsg.password, 40, md5Password);
	//对密保答案进行Md5加密
	CONST BYTE* tempanswer = (CONST BYTE*)answer.GetBuffer(40);
	CString md5Answer;
	GetMd5Hash(tempanswer, 40, md5Answer, err);
	strcpy_s(regMsg.data.regMsg.answer, 40, md5Answer);
	theApp.chatSocket->Send(&regMsg, sizeof(Message));
	CDialogEx::OnOK();
}


// Md5加密函数
BOOL GetMd5Hash(CONST BYTE *pbData, DWORD dwDataLen, CString &strHash, DWORD &err)
{
	HCRYPTPROV hProv;
	if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
	{
		err = GetLastError();
		return FALSE;
	}

	HCRYPTHASH hHash;
	if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
	{
		err = GetLastError();
		CryptReleaseContext(hProv, 0);
		return FALSE;
	}

	if (!CryptHashData(hHash, pbData, dwDataLen, 0))
	{
		err = GetLastError();
		CryptDestroyHash(hHash);
		CryptReleaseContext(hProv, 0);
		return FALSE;
	}

	DWORD dwSize;
	DWORD dwLen = sizeof(dwSize);
	CryptGetHashParam(hHash, HP_HASHSIZE, (BYTE*)(&dwSize), &dwLen, 0);

	BYTE* pHash = new BYTE[dwSize];
	dwLen = dwSize;
	CryptGetHashParam(hHash, HP_HASHVAL, pHash, &dwLen, 0);

	strHash = _T("");
	for (DWORD i = 0; i<dwLen; i++)
		strHash.AppendFormat(_T("%02X"), pHash[i]);
	delete[] pHash;

	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);

	return TRUE;
}