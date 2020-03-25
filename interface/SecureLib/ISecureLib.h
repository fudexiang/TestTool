#pragma once
#ifndef _ISECURELIB_H_

#define _ISECURELIB_H_

#include "Common/DataType.h"
#include "Common/Config.h"
#include "core/objptr.h"
#include "stdio.h"

class ISecureLib : public x3::IObject
{
	X3DEFINE_IID(ISecureLib);

	virtual CodeRet_t GetPCPrivateInfo(uint8_t* pBuffer, uint8_t MAX_Len) = 0;
};


#endif