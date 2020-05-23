#include <module/plugininc.h>		/* important */
#include "HttpLib.h"

CHttpLib::CHttpLib()
{
#ifndef _WIN64

	ft_http_init();

	phttp = ft_http_new();
#endif
}

CHttpLib::~CHttpLib()
{

}

CodeRet_t CHttpLib::HttpContext(const char* pURL, void *pText,uint32_t max_len)
{
#ifndef _WIN64
	const char* body = 0;
	uint32_t val = 0;
	if (phttp)
	{
		body = ft_http_sync_request(phttp, pURL, M_GET);

		//printf("ft_http get:%s\n", body);
		//printf("----------->ft_http addr:0x%x\n", (uint32_t)body);
		//printf("----------->ft_http get len:%d\n", strlen(body));
		//memcpy_s((void*)pText, max_len, body, strlen(body) + 1);
		val = (uint32_t)body;
		*(uint32_t *)pText = val;
	}
#endif
	return RET_OK;
}

CodeRet_t CHttpLib::HttpResClear(void)
{
#ifndef _WIN64
	if (phttp) 
	{
		ft_http_destroy(phttp);
	}
	ft_http_deinit();
#endif
	return RET_OK;
}