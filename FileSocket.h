#pragma once
#include "SendFile.h"

class SendFile;

// FileSocket ÃüÁîÄ¿±ê

class FileSocket : public CSocket
{
public:
	FileSocket();
	virtual ~FileSocket();

public:
	virtual void OnReceive(int nErrorCode);
	SendFile* sendFileDlg;
};
