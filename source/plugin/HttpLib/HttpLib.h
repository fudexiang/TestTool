#pragma once
#ifndef _HTTPLIB_H_
#define _HTTPLIB_H_

#include <module/classmacro.h>
#include <HttpLib/IHttpLib.h>

#include "http.h"

class CHttpLib : public IHttpLib
{
	X3BEGIN_CLASS_DECLARE(CHttpLib, clsidHttpLib)
		X3DEFINE_INTERFACE_ENTRY(IHttpLib)
		X3END_CLASS_DECLARE()

protected:
	CHttpLib();
	virtual ~CHttpLib();
public:
	ft_http_client_t* phttp;
	virtual CodeRet_t HttpContext(const char* pURL, void *pText, uint32_t max_len);
	virtual CodeRet_t HttpResClear(void);
};

#endif