#pragma once
#ifndef _LOGLIB_H_
#define _LOGLIB_H_

#include <module/classmacro.h>
#include <LogLib/ILogLib.h>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/sinks/file_sinks.h"

#define MSG_MAX_LEN	0x1000

class CLogLib: public ILogLib
{
	X3BEGIN_CLASS_DECLARE(CLogLib, clsidLogLib)
		X3DEFINE_INTERFACE_ENTRY(ILogLib)
		X3END_CLASS_DECLARE()

protected:
	CLogLib();
	virtual ~CLogLib();
public:
	std::shared_ptr<spdlog::logger> pFileLog;
	std::shared_ptr<spdlog::logger> pConsoleLog;

	char LogBuff[MSG_MAX_LEN];
	int day;
	int hour;
	int min;
	int sec;
	int file_count;

	virtual CodeRet_t CreateLogFile(void);
	virtual CodeRet_t SaveLogFile(void);
	virtual CodeRet_t trace(const char* fmt, ...) const;
	virtual CodeRet_t debug(const char* fmt, ...) const;
	virtual CodeRet_t info(const char* fmt, ...) const;
	virtual CodeRet_t warn(const char* fmt, ...) const;
	virtual CodeRet_t error(const char* fmt, ...) const;
	virtual CodeRet_t critical(const char* fmt, ...) const;
};

#endif