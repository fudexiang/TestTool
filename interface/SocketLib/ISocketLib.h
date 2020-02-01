#pragma once
#ifndef _ISOCKETLIB_H_

#define _ISOCKETLIB_H_

#include "Common/DataType.h"
#include "Common/Config.h"
#include "core/objptr.h"
#include "stdio.h"

//#include <WINSOCK2.H>

class ISocketLib : public x3::IObject
{
	X3DEFINE_IID(ISocketLib);
#if 0
	virtual CodeRet_t InitSocketServer(SOCKET* pSocket) = 0;

	virtual CodeRet_t InitSocketClient(SOCKET* pSocket) = 0;

	virtual CodeRet_t InitSocketClientConnect(const char* SERVER_ADDR, u_short port, SOCKET* pSocket) = 0;
	virtual int SocketLib_Rece(char* pbuffer, int size, SOCKET* pSocket) = 0;
	virtual int SocketLib_Send(char* pbuffer, int size, SOCKET* pSocket) = 0;
#endif
};


#endif