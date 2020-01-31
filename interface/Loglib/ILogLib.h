#pragma once
#ifndef _ILGLIB_H_

#define _ILGLIB_H_

#include "Common/DataType.h"
#include "Common/Config.h"
#include "core/objptr.h"

class ILogLib : public x3::IObject
{
	X3DEFINE_IID(ILogLib);

	virtual CodeRet_t CreateLogFile(void) = 0;
	virtual CodeRet_t SaveLogFile(void) = 0;

	virtual CodeRet_t trace(const char* fmt, ...) const = 0;
	virtual CodeRet_t debug(const char* fmt, ...) const = 0;
	virtual CodeRet_t info(const char* fmt, ...) const = 0;
	virtual CodeRet_t warn(const char* fmt, ...) const = 0;
	virtual CodeRet_t error(const char* fmt, ...) const = 0;
	virtual CodeRet_t critical(const char* fmt, ...) const = 0;
};


#endif