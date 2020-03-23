#pragma once
#ifndef _IMKOPELIB_H_

#define _IMKOPELIB_H_

#include "Common/DataType.h"
#include "Common/Config.h"
#include "core/objptr.h"
#include "stdio.h"

class IMKOpeLib : public x3::IObject
{
	X3DEFINE_IID(IMKOpeLib);
	virtual void LeftClick(void) = 0;
	virtual void RightClick(void) = 0;
	virtual void MoveMouse(int x, int y) = 0;
	virtual void EnterKey(char ButtonValue) = 0;

};


#endif