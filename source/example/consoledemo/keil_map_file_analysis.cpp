#include "stdio.h"
#include "Interfaces.h"

#define GENERATE_DSIM_HEX

#define IMAGE_SECTION_MAX	20

typedef struct
{
	uint32_t exec_base;
	uint32_t load_base;
	uint32_t load_size;
	uint32_t section_max_size;
}Section_info_t;

typedef struct
{
	Section_info_t section[IMAGE_SECTION_MAX];
	int section_num;
}ImageInfo_t;

static int GetWordsNextOffsetInString(char* pWords, char* pString, void* pPlugin)
{
	int ret = -1;
	ret = GetWordsOffsetInString(pWords, pString, pPlugin);
	ret += strlen(pWords);
	return ret;
}

CodeRet_t Hex2HexGenerate(FILE* f_hex, FILE* f_out, ImageInfo_t* pMap_info, void* pHexPlugin)
{
	uint32_t addr_offset = 0, load_addr, load_size, last_line_pos = 0, addr_halfword;
	int num_bytes, byte_val, i, section_count = 0, line_number=0;
	int record_size = 0;
	char line[256];
	char record_line[256], record_offset = 0;
	bool exit_flag = false;

	load_addr = pMap_info->section[section_count].load_base;

	load_size = pMap_info->section[section_count].load_size;

	fprintf(f_out, "@%x\r", pMap_info->section[section_count].exec_base);

	//: len offset  type value checksum
	//:	02	0000	04	1000	EA
	while ((fgets(line, sizeof(line), f_hex) != NULL) && (!exit_flag))
	{
		line_number++;

		/** step1:Must start with colon*/
		if (line[0] != ':')
		{
			printf("Bad format on line %d: %s\n", line_number, line);
			return RET_ERROR;
		}

		/** step2: length of data*/
		num_bytes = HexStrToHex(&line[1],2, pHexPlugin);

		/** step3: check type*/

		/*
		'00' Data Record//数据记录
		'01' End of File Record//文件结束记录
		'02' Extended Segment Address Record//扩展段地址记录
		'03' Start Segment Address Record//开始段地址记录
		'04' Extended Linear Address Record//扩展线性地址记录
		'05' Start Linear Address Record//开始线性地址记录
		*/
		if ((byte_val = HexStrToHex(&line[7], 2, pHexPlugin)) == 4)
		{
			addr_halfword = HexStrToHex(&line[9], 2, pHexPlugin);
			addr_halfword = addr_halfword << 8;
			addr_halfword |= HexStrToHex(&line[11], 2, pHexPlugin);

			addr_halfword = addr_halfword << 16;
			continue;
		}
		else if ((byte_val = HexStrToHex(&line[7], 2, pHexPlugin)) == 0)
		{
			addr_offset = HexStrToHex(&line[3], 2, pHexPlugin);
			addr_offset = addr_offset << 8;
			addr_offset |= HexStrToHex(&line[5], 2, pHexPlugin);

			if (((addr_halfword + addr_offset) > (load_addr + load_size - 1)) || \
				((addr_halfword + addr_offset + num_bytes - 1) < load_addr))
			{
				continue;
			}
		}
		else if ((byte_val = HexStrToHex(&line[7], 2, pHexPlugin)) == 1)
		{
			if (record_offset > 0)
			{
				record_line[record_offset++] = '\r';
				record_line[record_offset++] = '\0';
				record_offset = 0;
				fprintf(f_out, record_line);
			}

			printf("reach end of file\n");
			exit_flag = true;
		}
		else if ((byte_val = HexStrToHex(&line[7], 2, pHexPlugin)) == 5)
		{
			continue;
		}
		else
		{
			printf("Bad type on line %d: %s\n", line_number, line);
			return RET_ERROR;
		}

		for (i = 0; i < num_bytes; i++)
		{
			if ((addr_halfword + addr_offset + i) >= load_addr)
			{
				record_line[record_offset++] = line[i * 2 + 9];
				record_line[record_offset++] = line[i * 2 + 10];
				record_line[record_offset++] = ' ';

				record_size++;
			}

			if (48 == record_offset)
			{
				record_line[record_offset++] = '\r';
				record_line[record_offset++] = '\0';
				record_offset = 0;

				fprintf(f_out, record_line);
			}

			if (record_size >= load_size)
			{
				if (record_offset > 0)
				{
					record_line[record_offset++] = '\r';
					record_line[record_offset++] = '\0';
					record_offset = 0;
					fprintf(f_out, record_line);
				}


				if (section_count < pMap_info->section_num)
				{
					section_count++;

					LogPrintf(log_info, "section[%d]-Exec base:[0x%x],Load base:[0x%x],size[0x%x]\n", section_count-1, 
						pMap_info->section[section_count-1].exec_base,
						pMap_info->section[section_count-1].load_base,
						pMap_info->section[section_count-1].load_size);


					load_addr = pMap_info->section[section_count].load_base;

					load_size = pMap_info->section[section_count].load_size;

					fprintf(f_out, "@%x\r", pMap_info->section[section_count].exec_base);

					record_offset = 0;
					record_size = 0;
					fseek(f_hex, last_line_pos, SEEK_SET);
					break;
				}
				else
				{
					exit_flag = true;
					break;
				}
			}
		}

		last_line_pos = ftell(f_hex);
	}

	LogPrintf(log_info, "section[%d]-Exec base:[0x%x],Load base:[0x%x],size[0x%x]\n", section_count,
		pMap_info->section[section_count].exec_base,
		pMap_info->section[section_count].load_base,
		pMap_info->section[section_count].load_size);

	return RET_OK;
}

void KeilMapFileOperation(Threads_Control_t* p)
{
	ImageInfo_t image_info;
	char map_file[] = "1d_dtof.map";
	char hex_file[] = "1d_dtof.hex";

	char dsim_hex_out[] = "1d_dtof_dsim.hex";
	FILE* f_map = NULL;
	FILE* f_hex = NULL;
	FILE* f_out = NULL;

	errno_t err;

	char line[1024];
	bool exit_flag = false;

	int line_number, offset,i;

	void* pHexPlugin;

	image_info.section_num = 0;

	pHexPlugin = CreatePlugin(PLUGIN_HEX, clsidHexStr);

	if (!pHexPlugin)
	{
		printf("create PLUGIN_HEX failed\r\n");
		return;
	}

	err = fopen_s(&f_map, map_file, "r");

	if (0 != err)
	{
		printf("open %s error\r\n", map_file);

		return ;
	}

	line_number = 0;
	while ((fgets(line, sizeof(line), f_map) != NULL) && (!exit_flag))
	{
		line_number++;

		offset = GetWordsOffsetInString((char*)"Execution Region ", line, pHexPlugin);
		if (offset >= 0)
		{
			offset = GetWordsNextOffsetInString((char*)"Exec base: ", line, pHexPlugin);
			image_info.section[image_info.section_num].exec_base = HexStrToHex(&line[offset],8, pHexPlugin);

			offset = GetWordsNextOffsetInString((char*)"Load base: ", line, pHexPlugin);
			image_info.section[image_info.section_num].load_base = HexStrToHex(&line[offset],8, pHexPlugin);

			offset = GetWordsNextOffsetInString((char*)", Size: ", line, pHexPlugin);
			image_info.section[image_info.section_num].load_size = HexStrToHex(&line[offset],8, pHexPlugin);


			image_info.section_num++;
		}
	}

	for (i = 0; i < image_info.section_num; i++)
	{
		LogPrintf(log_info, "section[%d]-Exec base:[0x%x],Load base:[0x%x],size[0x%x]\n",i, image_info.section[i].exec_base,
			image_info.section[i].load_base,
			image_info.section[i].load_size);
	}

	if (f_map)
	{
		fclose(f_map);
	}

#ifdef GENERATE_DSIM_HEX
	err = fopen_s(&f_hex, hex_file, "r");
	if (0 != err)
	{
		printf("open %s error\r\n", hex_file);
		return ;
	}

	fopen_s(&f_out, dsim_hex_out, "w+");
	if (NULL == f_out)
	{
		printf("create %s error\r\n", dsim_hex_out);

		return ;
	}

	LogPrintf(log_info, "Now, start to get search hex values from hex file");
	Hex2HexGenerate(f_hex, f_out, &image_info, pHexPlugin);

	if (f_hex)
	{
		fclose(f_hex);
	}

	if (f_out)
	{
		fclose(f_out);
	}
#endif
}