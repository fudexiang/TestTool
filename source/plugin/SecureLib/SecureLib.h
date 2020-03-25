#pragma once
#ifndef _SOCKETLIB_H_
#define _SOCKETLIB_H_

#include <module/classmacro.h>
#include <SecureLib/ISecureLib.h>

class CSecureLib : public ISecureLib
{
	X3BEGIN_CLASS_DECLARE(CSecureLib, clsidSecureLib)
		X3DEFINE_INTERFACE_ENTRY(ISecureLib)
		X3END_CLASS_DECLARE()

protected:
	CSecureLib();
	virtual ~CSecureLib();
public:
	virtual CodeRet_t GetPCPrivateInfo(uint8_t* pBuffer, uint8_t MAX_Len);
};

#endif