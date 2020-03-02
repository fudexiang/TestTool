#pragma once
#ifndef _IHTTPLIB_H_

#define _IHTTPLIB_H_

#include "Common/DataType.h"
#include "Common/Config.h"
#include "core/objptr.h"
#include "stdio.h"

class IHttpLib : public x3::IObject
{
	X3DEFINE_IID(IHttpLib);

	virtual CodeRet_t HttpContext(const char* pURL, void *pText, uint32_t max_len) = 0;
	virtual CodeRet_t HttpResClear(void) = 0;

};


#endif