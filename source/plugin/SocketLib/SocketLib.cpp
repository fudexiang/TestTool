#include <module/plugininc.h>		/* important */
#include "SocketLib.h"

//#pragma comment(lib,"ws2_32.lib")

CSocketLib::CSocketLib()
{

}

CSocketLib::~CSocketLib()
{

}
#if 0
CodeRet_t CSocketLib::InitSocketServer(SOCKET *pSocket)
{
	WSADATA data;
	WORD sockVersion = MAKEWORD(2, 2);

	if (WSAStartup(sockVersion, &data) != 0)
	{
		return RET_ERROR;
	}

	*pSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	return RET_OK;
}

CodeRet_t CSocketLib::InitSocketClient(SOCKET* pSocket)
{
	WSADATA data;
	WORD sockVersion = MAKEWORD(2, 2);

	if (WSAStartup(sockVersion, &data) != 0)
	{
		return RET_ERROR;
	}

	*pSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);


	return RET_OK;
}

CodeRet_t CSocketLib::InitSocketClientConnect(const char * SERVER_ADDR,u_short port,SOCKET* pSocket)
{

	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(port);

	serAddr.sin_addr.S_un.S_addr = inet_addr(SERVER_ADDR);

	if (connect(*pSocket, (sockaddr*)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		printf("connect error !");
		closesocket(*pSocket);
		return RET_ERROR;
	}
	return RET_OK;
}

int CSocketLib::SocketLib_Rece(char* pbuffer, int size, SOCKET* pSocket)
{
	return  recv(*pSocket, pbuffer, size, 0);
}

int CSocketLib::SocketLib_Send(char* pbuffer, int size, SOCKET* pSocket)
{
	send(*pSocket, pbuffer, size, 0);
	return 0;
}
#endif