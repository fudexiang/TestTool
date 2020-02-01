#pragma once
#ifndef _SOCKETLIB_H_
#define _SOCKETLIB_H_

#include <module/classmacro.h>
#include <SocketLib/ISocketLib.h>

class CSocketLib : public ISocketLib
{
	X3BEGIN_CLASS_DECLARE(CSocketLib, clsidSocketLib)
		X3DEFINE_INTERFACE_ENTRY(ISocketLib)
		X3END_CLASS_DECLARE()

protected:
	CSocketLib();
	virtual ~CSocketLib();
public:

	virtual CodeRet_t InitSocketServer(SOCKET* pSocket);

	virtual CodeRet_t InitSocketClient(SOCKET* pSocket);

	virtual CodeRet_t InitSocketClientConnect(const char* SERVER_ADDR, u_short port, SOCKET* pSocket);

	virtual int SocketLib_Rece(char* pbuffer, int size, SOCKET* pSocket);
	virtual int SocketLib_Send(char* pbuffer, int size, SOCKET* pSocket);

};

#endif