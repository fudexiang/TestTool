#pragma once
#ifndef _SOCKETLIB_H_
#define _SOCKETLIB_H_

#include <module/classmacro.h>
#include <SocketLib/ISocketLib.h>

class CSocketLib : public ISocketLib
{
	X3BEGIN_CLASS_DECLARE(CSocketLib, clsidSocketLib)
		X3DEFINE_INTERFACE_ENTRY(ISocketLib)
		X3END_CLASS_DECLARE()

protected:
	CSocketLib();
	virtual ~CSocketLib();
public:
	
};

#endif