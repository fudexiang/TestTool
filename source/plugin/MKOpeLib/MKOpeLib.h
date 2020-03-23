#pragma once
#ifndef _MKOPELIB_H_
#define _MKOPELIB_H_

#include <module/classmacro.h>
#include <MKOpeLib/IMKOpeLib.h>

class CMKOpeLib : public IMKOpeLib
{
	X3BEGIN_CLASS_DECLARE(CMKOpeLib, clsidMKOpeLib)
		X3DEFINE_INTERFACE_ENTRY(IMKOpeLib)
		X3END_CLASS_DECLARE()

protected:
	CMKOpeLib();
	virtual ~CMKOpeLib();
public:
	virtual void LeftClick(void);
	virtual void RightClick(void);
	virtual void MoveMouse(int x, int y);
	virtual void EnterKey(char ButtonValue);
};

#endif