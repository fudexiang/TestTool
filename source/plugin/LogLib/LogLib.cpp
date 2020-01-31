#include <module/plugininc.h>		/* important */
#include "LogLib.h"

#include "time.h"

CLogLib::CLogLib()
{
	
	struct tm t;
	time_t tt;
	time(&tt);
	localtime_s(&t,&tt);

	day = t.tm_mday;
	hour = t.tm_hour;
	min = t.tm_min;
	sec = t.tm_sec;

	file_count=0;
}

CLogLib::~CLogLib()
{
	spdlog::drop_all();
}

CodeRet_t CLogLib::CreateLogFile(void)
{
	struct tm t;
	time_t tt;
	time(&tt);
	localtime_s(&t, &tt);

	if ((day != t.tm_mday) || (hour != t.tm_hour) || (min != t.tm_min) || (sec != t.tm_sec))
	{
		day = t.tm_mday;
		hour = t.tm_hour;
		min = t.tm_min;
		sec = t.tm_sec;
		file_count = 0;
	}
	else
	{
		file_count++;
	}

	sprintf_s(LogBuff, "%d%d%d_%d_%d_%d_%d",t.tm_year+ 1900,t.tm_mon+1,t.tm_mday, t.tm_hour,t.tm_min,t.tm_sec, file_count);

	spdlog::set_pattern("[%Y:%D:%T] %v");

	pFileLog = spdlog::basic_logger_mt("mylog", LogBuff, true);

	pConsoleLog = spdlog::stdout_color_mt("console");

	sprintf_s(LogBuff, "hello world [%d]", 100);

	pFileLog->info(LogBuff);
	pConsoleLog->info(LogBuff);

	return RET_OK;
}

CodeRet_t CLogLib::SaveLogFile(void)
{
	spdlog::drop_all();

	return RET_OK;
}

CodeRet_t CLogLib::trace(const char* fmt, ...) const
{
	int len;
	va_list args;
	va_start(args, fmt);
	len = vsprintf_s((char *)LogBuff, MSG_MAX_LEN, fmt, args);
	va_end(args);
	
	if (MSG_MAX_LEN <= len)
	{
		return RET_SIZE_ERROR;
	}

	pFileLog->trace(LogBuff);
	pConsoleLog->trace(LogBuff);
	return RET_OK;
}
CodeRet_t CLogLib::debug(const char* fmt, ...) const
{
	int len;
	va_list args;
	va_start(args, fmt);
	len = vsprintf_s((char *)LogBuff, MSG_MAX_LEN, fmt, args);
	va_end(args);

	if (MSG_MAX_LEN <= len)
	{
		return RET_SIZE_ERROR;
	}

	pFileLog->debug(LogBuff);
	pConsoleLog->debug(LogBuff);
	return RET_OK;
}
CodeRet_t CLogLib::info(const char* fmt, ...) const
{
	int len;
	va_list args;
	va_start(args, fmt);
	len = vsprintf_s((char *)LogBuff, MSG_MAX_LEN, fmt, args);
	va_end(args);

	if (MSG_MAX_LEN <= len)
	{
		return RET_SIZE_ERROR;
	}

	pFileLog->info(LogBuff);
	pConsoleLog->info(LogBuff);
	return RET_OK;
}

CodeRet_t CLogLib::warn(const char* fmt, ...) const
{
	int len;
	va_list args;
	va_start(args, fmt);
	len = vsprintf_s((char *)LogBuff, MSG_MAX_LEN, fmt, args);
	va_end(args);

	if (MSG_MAX_LEN <= len)
	{
		return RET_SIZE_ERROR;
	}

	pFileLog->warn(LogBuff);
	pConsoleLog->warn(LogBuff);
	return RET_OK;
}

CodeRet_t CLogLib::error(const char* fmt, ...) const
{
	int len;
	va_list args;
	va_start(args, fmt);
	len = vsprintf_s((char *)LogBuff, MSG_MAX_LEN, fmt, args);
	va_end(args);

	if (MSG_MAX_LEN <= len)
	{
		return RET_SIZE_ERROR;
	}

	pFileLog->error(LogBuff);
	pConsoleLog->error(LogBuff);
	return RET_OK;
}

CodeRet_t CLogLib::critical(const char* fmt, ...) const
{
	int len;
	va_list args;
	va_start(args, fmt);
	len = vsprintf_s((char *)LogBuff, MSG_MAX_LEN, fmt, args);
	va_end(args);

	if (MSG_MAX_LEN <= len)
	{
		return RET_SIZE_ERROR;
	}

	pFileLog->critical(LogBuff);
	pConsoleLog->critical(LogBuff);
	return RET_OK;
}