#include <module/plugininc.h>		/* important */
#include "LogLib.h"


CLogLib::CLogLib()
{
	plog = NULL;
}

CLogLib::~CLogLib()
{
	spdlog::drop_all();
}

CodeRet_t CLogLib::CreateLogFile(void)
{
	spdlog::set_pattern("[%Y:%D:%T] %v");

	plog = spdlog::basic_logger_mt("mylog", "log", true);

	sprintf_s(LogBuff, "critical [%d]", 100);

	plog->critical(LogBuff);

	return RET_OK;
}