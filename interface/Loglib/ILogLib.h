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
};


#endif