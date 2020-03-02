#pragma once
#ifndef _IHEXSTR_H_

#define _IHEXSTR_H_

#include "Common/DataType.h"
#include "Common/Config.h"
#include "core/objptr.h"
#include "stdio.h"

const uint32_t DISPLAY_BOOT_START_ADDR = 0xFA00;
const uint32_t  DISPLAY_BOOT_END_ADDR = 0xFFFF;
const uint32_t DISPLAY_MAX_ADDRESS = 0x1000000;

const uint32_t DISPLAY_ERASE_BLOCK  = 512;
const uint32_t DISPLAY_WR_BLOCK = 64;


typedef struct
{
	uint32_t max_code_address;

	uint32_t boot_start_address;
	uint32_t boot_end_address;

	uint32_t program_start_address;
	uint32_t program_end_address;

	uint32_t sector_start_addr[100];
	uint32_t sector_end_addr[100];
	uint32_t sector_data_valid[100];
	uint32_t sector_num;

	char *pImageFilePath;
	uint8_t *pDataBuff;
	uint32_t ImageTotalSize;
}Info_t;

class IHexStr : public x3::IObject
{
	X3DEFINE_IID(IHexStr);

	virtual CodeRet_t Hex2BinGenerate(void *pInfo) = 0;

	virtual CodeRet_t S19ToBinGenerate(void *pInfo) = 0;

	virtual int UnicodeToUTF8_Values(char *pText, Unicode_Size_t uint_size,char *pBuffer,int buffer_len) = 0;

	virtual int HexToStr(uint8_t val, char* pBuffer, Case_type_t type) = 0;
};


#endif