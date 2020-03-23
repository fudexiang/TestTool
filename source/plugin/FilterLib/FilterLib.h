#pragma once
#ifndef _FILTERLIB_H_
#define _FILTERLIB_H_

#include <module/classmacro.h>
#include <FilterLib/IFilterLib.h>

class CFilterLib : public IFilterLib
{
	X3BEGIN_CLASS_DECLARE(CFilterLib, clsidFilterLib)
		X3DEFINE_INTERFACE_ENTRY(IFilterLib)
		X3END_CLASS_DECLARE()

protected:
	CFilterLib();
	virtual ~CFilterLib();
public:

	FILTERINFO pre_filter_Vo;
	virtual void Init_Filter(FILTER_TYPE_t type, double ts, double freq_center, double q);
	virtual double Filter(double in);
};

#endif