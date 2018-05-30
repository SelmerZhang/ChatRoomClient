// FileSocket.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChatRoomClient.h"
#include "FileSocket.h"


// FileSocket

FileSocket::FileSocket()
	:sendFileDlg(NULL)
{
}

FileSocket::~FileSocket()
{
}


// FileSocket ��Ա����
void FileSocket::OnReceive(int nErrorCode)
{
	int ret;
	CString fromIP;
	UINT fromPort;
	FileBlock* rcvBlock = new FileBlock;
	memset(rcvBlock, 0, sizeof(FileBlock));
	ret = this->ReceiveFrom(rcvBlock, sizeof(FileBlock), fromIP, fromPort);


	switch (rcvBlock->type)   //�ļ����Ͳ���ͣ��Э�飬�����յ��Ĳ�ͬ��Ϣ��������
	{
	case MSG_REFUSE:          //�ܾ������ļ�
	{
		AfxMessageBox(_T("�Է��ܾ������㷢�͵��ļ���"));
		if (!sendFileDlg)
		{
			AfxMessageBox("δ�򿪶Ի���");
			return;
		}
		fclose(sendFileDlg->file);
		sendFileDlg->sending = false;
		break;
	}
	case MSG_SENDREQUEST:
	{
		char* fileName = new char[rcvBlock->filesize];
		strcpy_s(fileName, rcvBlock->filesize, rcvBlock->file);    //��ȡ�ļ���
		CString receivemsg;
		int filesize = atoi(&rcvBlock->file[rcvBlock->filesize + 5]);  //��ȡ�ļ��ܴ�С
		//��ȡ��Դ���û���
		CString fromUser;
		SOCKADDR_IN tempaddr;
		memset(&tempaddr, 0, sizeof(SOCKADDR_IN));
		tempaddr.sin_family = AF_INET;
		tempaddr.sin_port = fromPort;
		tempaddr.sin_addr.S_un.S_addr = inet_addr(fromIP);
		POSITION ps = UserInfoList.GetHeadPosition();
		while (ps != NULL)
		{
			UserInfo* pTemp = (UserInfo*)UserInfoList.GetNext(ps);
			if (((SOCKADDR_IN*)&pTemp->fileaddr)->sin_addr.S_un.S_addr == tempaddr.sin_addr.S_un.S_addr)          //�Ѵ��ڸ��û���������Ϣ�Ի���
			{
				if (((SOCKADDR_IN*)&pTemp->fileaddr)->sin_port == fromPort)
				{
					fromUser = pTemp->username;
				}
			}
		}
		receivemsg.Format("�յ������� %s ���ļ���%s\n�ļ���С��%i�ֽ�\n�Ƿ���գ�", fromUser.GetBuffer(), fileName, filesize);
		if (IDOK == AfxMessageBox(receivemsg, MB_OKCANCEL))
		{
			CFileDialog saveDlg(false, NULL, fileName);
			if (IDOK == saveDlg.DoModal())
			{
				sendFileDlg = new SendFile;
				sendFileDlg->nFileSize = filesize;
				sendFileDlg->Create(IDD_SENDFILE);
				sendFileDlg->ShowWindow(SW_SHOWNORMAL);
				sendFileDlg->rcvProgress.SetRange(0, filesize / FILESIZE + 1);
				sendFileDlg->nRecv = 0;
				if (!(sendFileDlg->file = fopen(saveDlg.GetPathName(), "wb")))
				{
					AfxMessageBox("���������ļ�ʧ�ܣ�");
					break;
				}
				char request[5] = { MSG_AGREE,0,0,0,0 };
				SendTo(request, 5, fromPort, fromIP);
				sendFileDlg->nRecv++;
				sendFileDlg->rcvText.Format("���ս��ȣ�(%.1f%%)", (float)sendFileDlg->nRecv / (sendFileDlg->nFileSize / FILESIZE + 1) * 100);
				sendFileDlg->UpdateData(false);
				break;
			}
			else
			{
				char request[5] = { MSG_REFUSE,0,0,0,0 };
				SendTo(request, 5, fromPort, fromIP);
				break;
			}
		}
		else
		{
			char request[5] = { MSG_REFUSE,0,0,0,0 };
			SendTo(request, 5, fromPort, fromIP);
			break;
		}
	}
	case MSG_AGREE:
	{
		int nRead = fread(rcvBlock->file, 1, FILESIZE, sendFileDlg->file);
		if (nRead < FILESIZE)
		{
			rcvBlock->type = MSG_FILETAIL;
			rcvBlock->filesize = nRead;
			SendTo(rcvBlock, sizeof(FileBlock), fromPort, fromIP);
			fclose(sendFileDlg->file);
			sendFileDlg->sendProgress.SetPos(sendFileDlg->nFileSize_s / FILESIZE + 1);
			sendFileDlg->sending = false;
			sendFileDlg->sendText = "���ͽ��ȣ�(100%)";
			sendFileDlg->UpdateData(false);
			AfxMessageBox("������ϣ�");
			sendFileDlg->sendProgress.SetPos(0);
			sendFileDlg->sendText = "���ͽ��ȣ�";
			sendFileDlg->UpdateData(false);
		}
		else
		{
			rcvBlock->type = MSG_FILE;
			rcvBlock->filesize = nRead;
			SendTo(rcvBlock, sizeof(FileBlock), fromPort, fromIP);
			sendFileDlg->nSend++;
			sendFileDlg->sendProgress.SetPos(sendFileDlg->nSend);
			sendFileDlg->sendText.Format("���ͽ��ȣ�(%.1f%%)", (float)sendFileDlg->nSend / (sendFileDlg->nFileSize_s / FILESIZE + 1) * 100);
			sendFileDlg->UpdateData(false);
		}
		break;
	}
	case MSG_FILE:
	{
		fwrite(rcvBlock->file, 1, rcvBlock->filesize, sendFileDlg->file);
		char request[5] = { MSG_AGREE,0,0,0,0 };
		SendTo(request, 5, fromPort, fromIP);
		sendFileDlg->nRecv++;
		sendFileDlg->rcvProgress.SetPos(sendFileDlg->nRecv);
		sendFileDlg->rcvText.Format("���ս��ȣ�(%.1f%%)", (float)sendFileDlg->nRecv / (sendFileDlg->nFileSize / FILESIZE + 1) * 100);
		sendFileDlg->UpdateData(false);
		break;
	}
	case MSG_FILETAIL:
	{
		fwrite(rcvBlock->file, 1, rcvBlock->filesize, sendFileDlg->file);
		fclose(sendFileDlg->file);
		sendFileDlg->rcvProgress.SetPos(sendFileDlg->nFileSize / FILESIZE + 1);
		sendFileDlg->rcvText = "���ս��ȣ�(100%)";
		sendFileDlg->UpdateData(false);
		AfxMessageBox("������ϣ�");
		sendFileDlg->rcvText = "���ս��ȣ�";
		sendFileDlg->rcvProgress.SetPos(0);
		sendFileDlg->UpdateData(false);
		break;
	}
	default:break;
	}

	CSocket::OnReceive(nErrorCode);
}
