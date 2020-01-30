#include <module/plugininc.h>		/* important */
#include "HexStr.h"

CHexStr::CHexStr()
{

}

CHexStr::~CHexStr()
{

}

static int get_nybble(unsigned char *s)
{
	int value;

	if (*s >= '0' && *s <= '9') 
	{
		value = *s - '0';
	}
	else if (*s >= 'a' && *s <= 'f') 
	{
		value = *s - 'a' + 10;
	}
	else if (*s >= 'A' && *s <= 'F') 
	{
		value = *s - 'A' + 10;
	}
	else 
	{
		//printf("Invalid hex character: %c\n", *s);
		value = 0;
	}

	return value;
}

static int get_byte(unsigned char *s)
{
	/* this is right, shut up: */
	return get_nybble(s) * 16 + get_nybble(s + 1);
}

static int get_word(unsigned char *s)
{
	/* yes this too */
	return get_byte(s) * 256 + get_byte(s + 2);
}

CodeRet_t CHexStr::Hex2BinGenerate(void *pInfo)
{
	//Info_t *p = (Info_t *)pInfo;

	FILE *f=NULL;
	FILE *f_record = NULL;
	uint8_t *s;
	uint8_t checksum;

	uint32_t base_address = 0x0;
	int num_bytes;
	int address = 0x0, address_bk = 0;
	int offset=0;
	
	int i;
	int byte;
	char line[256];
	char myfile[] = "1.txt";
	int line_number = 0;

	bool new_file_flag = false;
	errno_t err;

	err = fopen_s(&f, "E:\\Fish\\Dexiang\\1.hex", "r");

	if (0 != err)
		return RET_ERROR;
	else
	{

	}
	while (fgets(line, sizeof(line), f) != NULL) 
	{
		s = (unsigned char *)line;
		line_number++;

		/** step1:Must start with colon*/
		if (*s != ':')				
		{
			printf("Bad format on line %d: %s\n", line_number, line);
			return RET_ERROR;
		}
		s++;

		checksum = 0;
		num_bytes = get_byte(s);
		if (num_bytes == 0) 
		{
			// All done.
			break;
		}

		/** step2: length of data*/
		checksum += (uint8_t)num_bytes;
		s += 2;

		/** step3: address of data*/
		address = get_word(s);
		checksum += (uint8_t)get_byte(s);
		checksum += (uint8_t)get_byte(s + 2);
		s += 4;

		/** step4: type of data*/
		if ((byte = get_byte(s)) != 0) 
		{
			s += 2;
			checksum += (uint8_t)byte;

			/*
			'00' Data Record//数据记录
			'01' End of File Record//文件结束记录
			'02' Extended Segment Address Record//扩展段地址记录
			'03' Start Segment Address Record//开始段地址记录
			'04' Extended Linear Address Record//扩展线性地址记录
			'05' Start Linear Address Record//开始线性地址记录
			*/
			if (byte == RECORD_SET_EXECUTION_START) 
			{
				checksum += (uint8_t)get_byte(s);
				checksum += (uint8_t)get_byte(s + 2);
				checksum += (uint8_t)get_byte(s + 4);
				checksum += (uint8_t)get_byte(s + 6);
				s += 8;

				new_file_flag = true;
			}
			else if (byte == RECORD_EXTENDED_LINEAR_ADDRESS) 
			{
				base_address = get_word(s) * 65536;
				checksum += (uint8_t)get_byte(s);
				checksum += (uint8_t)get_byte(s + 2);
				s += 4;

				new_file_flag = true;
			}
			else 
			{
				printf("record type other than 0 or 4\n");
				return RET_ERROR;
			}
		}
		else 
		{
			// byte doesn't affect checksum because it's 0
			s += 2;

			if (address_bk != address)
			{
				new_file_flag = true;
			}

			if (true == new_file_flag)
			{
				new_file_flag = false;
				if (NULL != f_record)
				{
					fclose(f_record);
					myfile[0] += 1;
				}
				fopen_s(&f_record, myfile, "w+");
				if (NULL == f_record)
					return RET_ERROR;

				fprintf(f_record, "0x%x\n", base_address+ address);
			}
			address_bk = address + num_bytes/ ADDRESS_UNIT_BYTES;

			for (i = 0; i < num_bytes; i++) 
			{
				byte = get_byte(s);
				s += 2;

				checksum += (uint8_t)byte;

				fprintf(f_record, "%02x", byte);

				address++;
			}

			offset += num_bytes;
			if (LINE_MAX_NUM <= offset)
			{
				fprintf(f_record, "\n");
				offset = 0;
			}
			
		}

		/** step5: Verify checksum*/
		checksum = ~checksum + 1;
		int file_checksum = get_byte(s);
		if (checksum != file_checksum) 
		{
				return RET_ERROR;
		}
	}

	if (NULL != f_record)
	{
		fclose(f_record);
	}

	fclose(f);

	return RET_OK;
}

/*
S-record format：
	type	+	count	+	address		+	data	+	checksum
	S1			23			5000			8B899EFE05F6AF019EFF05888A81A7FCC650854C95E701C650844CF732508620	F0
type	:	2 bytes	(S0，S1，S2，S3，S5，S7，S8，S9)
			S0 -
			S1 - 2 bytes address
			S2 - 3 bytes address
			S3 - 4 bytes address
			S5 - 
			S7,S8,S9 - end 
count	:	2 bytes
address	:	4-6-8 bytes
data	:
checksum:	2 bytes
*/
CodeRet_t CHexStr::S19ToBinGenerate(void *pInfo)
{
	Info_t *p = (Info_t *)pInfo;
	FILE *f = NULL;
	FILE *f_record = NULL;
	char MyLogFile[] = "Log.txt";
	char line[256];
	int line_number;
	uint32_t num_bytes,i,flag;

	uint8_t checksum,alen,terminate;
	uint32_t address=0, address_bk = 0;
	uint8_t *s;

	errno_t err;

	err = fopen_s(&f, p->pImageFilePath, "r");
	if (0 != err)
		return RET_ERROR;

	fopen_s(&f_record, MyLogFile, "w+");
	if (NULL == f_record)
		return RET_ERROR;

	line_number = 0;
	p->ImageTotalSize = 0;

	p->program_start_address = 0;
	p->program_end_address = 0;
	p->sector_num = 0xffffffff;

	flag = 0;

	while (fgets(line, sizeof(line), f) != NULL)
	{
		s = (unsigned char *)line;
		line_number++;

		/** step1:Must start with 'S'*/
		if (*s != 'S')
		{
			printf("Bad format on line %d: %s\n", line_number, line);
			return RET_ERROR;
		}

		s++;

		/** step2: data type*/
		switch (*s)
		{
			case '0':
				continue;
			case '1':
				alen = 4;
				break;
			case '2':
				alen = 6;
				break;
			case '3':
				alen = 8;
				break;
			case '9':
			case '8':
			case '7':
				terminate = 1;
				continue;
			default:
				continue;
		}
		s++;

		/** step2: length of data*/
		num_bytes = (uint32_t)(get_byte(s)) & 0xff;
		checksum = (uint8_t)num_bytes;

		s += 2;

		/** step3: address of data*/
		address = get_word(s);
		checksum += (uint32_t)(get_byte(s)) & 0xff;
		checksum += (uint32_t)(get_byte(s + 2)) & 0xff;
		s += 4;

		if (6 == alen)
		{
			address = (address << 8);
			address += (uint32_t)(get_byte(s)) & 0xff;
			checksum += (uint8_t)get_byte(s);
			s += 2;
		}
		else if (8 == alen)
		{
			address = (address << 16);
			address += (uint32_t)(get_word(s)) & 0xffff;
			checksum += (uint8_t)get_byte(s);
			s += 2;
			checksum += (uint8_t)get_byte(s);
			s += 2;
		}

		/** step4: type of data*/
		i = 0;
		while (i < num_bytes - 1 - alen/2)
		{
			
			/** for log start*/
			if (address_bk + 1 != address+i)
			{
				fprintf(f_record, "\n**last:%x,current:%x**\n", address_bk,address);
				flag++;

				p->sector_num++;
				p->sector_start_addr[p->sector_num] = address + i;
				
			}
			else
			{
				p->sector_end_addr[p->sector_num] = address + i;
			}

			address_bk = address+i;
			/** for log end*/

			if (((address + i) >= p->boot_start_address) && ((address + i) <= p->boot_end_address))
			{
				//if boot area, do nothing
				
			}
			else
			{
				p->pDataBuff[address + i] = (uint8_t)get_byte(s);
				fprintf(f_record, "%02x", (uint8_t)get_byte(s));

				p->ImageTotalSize++;

				if (p->boot_start_address == 0)
				{
					p->boot_start_address = address;
				}
				p->program_end_address = address + i;
			}


			checksum += (uint8_t)get_byte(s);
			
			s += 2;

			i++;
						
		}

		/** step5: Verify checksum*/
		checksum += (uint8_t)get_byte(s);
		if ((uint8_t)0xff != checksum)
		{
			return RET_ERROR;
		}
	}

	if (NULL != f_record)
	{
		fclose(f_record);
	}

	fclose(f);

	//calculate the program sectors info

	return RET_OK;
}