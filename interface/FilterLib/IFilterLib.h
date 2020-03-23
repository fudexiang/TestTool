#pragma once
#ifndef _IFILTERLIB_H_

#define _IFILTERLIB_H_

#include "Common/DataType.h"
#include "Common/Config.h"
#include "core/objptr.h"
#include "stdio.h"

class IFilterLib : public x3::IObject
{
	X3DEFINE_IID(IFilterLib);
	virtual void Init_Filter(FILTER_TYPE_t type, double ts, double freq_center, double q) = 0;
	virtual double Filter(double in) = 0;
};


#endif