#pragma once
#ifndef _LOGLIB_H_
#define _LOGLIB_H_

#include <module/classmacro.h>
#include <LogLib/ILogLib.h>

#include "spdlog/spdlog.h"


class CLogLib: public ILogLib
{
	X3BEGIN_CLASS_DECLARE(CLogLib, clsidLogLib)
		X3DEFINE_INTERFACE_ENTRY(ILogLib)
		X3END_CLASS_DECLARE()

protected:
	CLogLib();
	virtual ~CLogLib();
public:
	std::shared_ptr<spdlog::logger> plog;
	char LogBuff[100];
	virtual CodeRet_t CreateLogFile(void);
};

#endif