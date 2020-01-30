#pragma once
#ifndef _HEXSTR_H_
#define _HEXSTR_H_

#include <module/classmacro.h>
#include <HexStr/IHexStr.h>

#define RECORD_EXTENDED_LINEAR_ADDRESS	0x04
#define RECORD_SET_EXECUTION_START		0x05

#define LINE_MAX_NUM					0x30

#define ADDRESS_UNIT_BYTES				2

typedef struct
{
	uint8_t type[2];
	uint8_t count[2];
	uint8_t addr[8];
	uint8_t data[256];
	uint8_t checksum[2];
}S19_Format_Data_t;

class CHexStr : public IHexStr
{
	X3BEGIN_CLASS_DECLARE(CHexStr, clsidHexStr)
		X3DEFINE_INTERFACE_ENTRY(IHexStr)
		X3END_CLASS_DECLARE()

protected:
	CHexStr();
	virtual ~CHexStr();
public:
	
	virtual CodeRet_t Hex2BinGenerate(void *pInfo);
	virtual CodeRet_t S19ToBinGenerate(void *pInfo);

};

#endif