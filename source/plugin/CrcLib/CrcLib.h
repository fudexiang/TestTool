#pragma once
#ifndef _CRCLIB_H_
#define _CRCLIB_H_

#include <module/classmacro.h>
#include <CrcLib/ICrcLib.h>

class CCrcLib : public ICrcLib
{
	X3BEGIN_CLASS_DECLARE(CCrcLib, clsidCrcLib)
		X3DEFINE_INTERFACE_ENTRY(ICrcLib)
		X3END_CLASS_DECLARE()

protected:
	CCrcLib();
	virtual ~CCrcLib();
public:
	
};

#endif