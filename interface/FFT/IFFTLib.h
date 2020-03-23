#pragma once
#ifndef _IFFTLIB_H_

#define _IFFTLIB_H_

#include "Common/DataType.h"
#include "Common/Config.h"
#include "core/objptr.h"
#include "stdio.h"

class IFFTLib : public x3::IObject
{
	X3DEFINE_IID(IFFTLib);
	virtual CodeRet_t fft(complex* x, uint32_t size_x) = 0;
};


#endif