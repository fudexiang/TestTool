#include "Interfaces.h"

#include "stdio.h"
#include "time.h"

/***********feature on/off****************/
//#define OPEN_DEBUG_INFO
/*****************************************/

#define BUFFER_NUM	20
#define HTTP_DATA_LEN	2048
#define HTTP_MAX_LEN	0x100
#define MAX_COUNT	20


uint8_t HttpURL[HTTP_DATA_LEN];

FILE* Config_File_fp;

SocktPlugin_t SocketPluginObject;

struct MyCollect_info
{
	char url_info[MAX_COUNT][HTTP_DATA_LEN];
	char start[MAX_COUNT][100];
	char highest[MAX_COUNT][100];
	char lowest[MAX_COUNT][100];
	char end[MAX_COUNT][100];
	char per[MAX_COUNT][100];
	char date[MAX_COUNT][100];
	int count;
} Myinfo;

int URLTextHandle(uint8_t *temp, char * pHttpData, void * pHexPlugin)
{
	pHttpData[0] = '%';
	HexToStr(temp[0], &pHttpData[1], CAPTICAL_CASE, pHexPlugin);
	pHttpData[3] = '%';
	HexToStr(temp[1], &pHttpData[4], CAPTICAL_CASE, pHexPlugin);
	pHttpData[6] = '%';
	HexToStr(temp[2], &pHttpData[7], CAPTICAL_CASE, pHexPlugin);
	return 9;
}

void save_result(void)
{
	int i;
	char filename[BUFFER_NUM];

	FILE* fp;


	for (i = 0; i < Myinfo.count; i++)
	{
		sprintf_s(filename, BUFFER_NUM, "%d.data", i);

		fp = fopen(filename, "a+");
		if (fp)
		{
			fprintf(fp, "%s \t%s \t%s \t%s\r\n", (char*)&Myinfo.start[i][0], \
				(char*)&Myinfo.end[i][0], (char*)&Myinfo.highest[i][0], (char*)&Myinfo.lowest[i][0]);
			fclose(fp);
		}
	}
	

}

void AnalyData(char* file_name)
{
#define	HEADER1	"                    <td>"
#define HEADER2 "                        <div class="
#define ENDER1	"</"

	int i,text_start=0,text_end=0, value_start=0, value_end=0;
	char* pText;
	uint8_t temp[HTTP_MAX_LEN];
	uint8_t test1[] = HEADER1;
	uint8_t test2[] = HEADER2;
	uint8_t oepn[] = { 0xE4,  0xBB,  0x8A,  0xE5,  0xBC,  0x80 };
	uint8_t hightest[] = { 0xE6,  0x9C,  0x80,  0xE9,  0xAB,  0x98 };
	uint8_t per[] = { 0xE5,  0xB8,  0x82,  0xE7,  0x9B,  0x88,  0xE7,  0x8E,  0x87 };
	uint8_t close_with[] = { 0xE6,  0x98,  0xA8,  0xE6,  0x94,  0xB6 };
	uint8_t lowest[] = { 0xE6,  0x9C,  0x80,  0xE4,  0xBD,  0x8E };

	FILE* fp = fopen(file_name, "r");
	temp[HTTP_MAX_LEN-1] = '\0';

	if (fp)
	{
		while (fgets((char *)temp, HTTP_MAX_LEN-1, fp) != NULL)
		{
			if (0 == memcmp(temp, HEADER1, strlen(HEADER1)))
			{
				text_start = strlen(HEADER1);

				i = 0;
				while (i < HTTP_MAX_LEN - strlen(HEADER1))
				{
					if (':' == temp[text_start + i])
					{
						text_end = text_start + i - 1;
						value_start = text_start + i + 1;

						while (i < HTTP_MAX_LEN - strlen(HEADER1))
						{
							i++;
							if (0 == memcmp(&temp[text_start + i], ENDER1, strlen(ENDER1)))
							{
								value_end = text_start + i - 1;
								break;
							}

						}
#if 0
						printf("[%d][%d]tag name[%d][%d]\r\n", text_start, text_end, value_start, value_end);
						for (i = text_start; i <= text_end; i++)
						{
							printf("0x%x\r\n", temp[i]);
						}

						for (i = value_start; i <= value_end; i++)
						{
							printf("%c\r\n", temp[i]);
						}
#endif
#if 0
						printf("%d,%d,%d,%d\r\n", text_start, text_end, value_start, value_end);
#endif
						i = value_end - value_start + 1;

						pText = NULL;
						if (0 == memcmp(&temp[text_start], oepn, sizeof(oepn)))
						{
							pText = &Myinfo.start[Myinfo.count][0];
						}
						else if (0 == memcmp(&temp[text_start], hightest, sizeof(hightest)))
						{
							pText = &Myinfo.highest[Myinfo.count][0];
						}
						else if (0 == memcmp(&temp[text_start], per, sizeof(per)))
						{
							pText = &Myinfo.per[Myinfo.count][0];
						}
						else if (0 == memcmp(&temp[text_start], close_with, sizeof(close_with)))
						{
							pText = &Myinfo.end[Myinfo.count][0];
						}
						else if (0 == memcmp(&temp[text_start], lowest, sizeof(lowest)))
						{
							pText = &Myinfo.lowest[Myinfo.count][0];
						
						}

						if (NULL != pText)
						{
							memcpy(pText, &temp[value_start], i);
							pText[i + 1] = '\0';
							printf("find [%s]\r\n", pText);
						}

						break;
					}
					i++;
				}
			}
		}
#if 0
		while (1)
		{
			i=fread(temp, 1, 1, fp);
			fgets(temp, , fp);

			if (i == 0)
			{
				break;
			}
			else
			{
				/*
				<td>今开:1.79</td>
				<td>最高:1.99</td>
				<td>市盈率:-14.97</td>
				<div class="mh-time">2020-02-28 15:31:03 （北京时间）</div>
				<td>昨收:1.83</td>
				<td>最低:1.77</td>
				<td>总市值:100.93亿</td>
				*/
				if ('<' == temp[0])
				{
					if (('<' == pData[i]) && ('t' == pData[i + 1]) && ('d' == pData[i + 2]) && ('>' == pData[i + 3]))
					{
						printf("find td\n");
						memcpy(temp, &pData[i], 0x1f);
						printf("find td[%s]\n", temp);
					}

					if (!memcmp("<td>今开:", &pData[i], sizeof("<td>今开:")))
					{
						printf("a \n");
					}
					else if (!memcmp("<td>最高:", &pData[i], sizeof("<td>最高:")))
					{
						printf("b \n");
					}
					else if (!memcmp("<td>市盈率:", &pData[i], sizeof("<td>市盈率:")))
					{
						printf("c \n");
					}
					else if (!memcmp("<td>昨收:", &pData[i], sizeof("<td>昨收:")))
					{
						printf("d \n");
					}
					else if (!memcmp("<td>最低:", &pData[i], sizeof("<td>最低:")))
					{
						printf("d \n");
						break;
					}
					else if (!memcmp("<div class=", &pData[i], sizeof("<div class=")))
					{
						printf("find time\n");
					}
					else
					{
						i++;
						continue;
					}
				}

			}
		}
#endif
		fclose(fp);
	}

}

void download_info(int num,char *pURL,char *file_name,void* pHttpPlugin)
{
	FILE* fp;
	const char* pStr;
	uint32_t addr = 0;

	Http_GetContext(pURL, &addr, HTTP_MAX_LEN, pHttpPlugin);

	//printf("val = 0x%x\n", addr);

	fp = fopen(file_name, "a+");
	if (fp)
	{
#if 1
		if (addr != 0)
		{
			pStr = (const char*)addr;
			fputs(pStr, fp);
		}
		else
		{
			fputs("addr = 0\n", fp);

		}
#endif

		fclose(fp);
	}
	else
	{
		printf("create file failed[%s]\n", file_name);
	}

}

void MyWebtest(void)
{
	int i, ret,offset;
	uint8_t temp[3];
	char HttpData[HTTP_MAX_LEN];

	time_t timep;
	struct tm* p;

	void* pHttpPlugin = CreatePlugin(PLUGIN_HTTP, clsidHttpLib);
	void* pHexPlugin = CreatePlugin(PLUGIN_HEX, clsidHexStr);
	int year, month, day;
	time(&timep);
	p = gmtime(&timep);

	year = 1900 + p->tm_year;
	month = 1+ p->tm_mon;
	day = p->tm_mday;

	Myinfo.count = 0;
	offset = 0;
	Config_File_fp = fopen("config.txt", "r");
	if (Config_File_fp)
	{
		while (1)
		{
			ret = fread(temp, 1, 1, Config_File_fp);
			if (ret == 0)
			{
				break;
			}
			else
			{

				if (0xe0 == (0xf0 & temp[0]))
				{
						ret = fread(&temp[1], 1, 2, Config_File_fp);
						if (ret != 2)
						{
							break;
						}
						offset += URLTextHandle((uint8_t*)temp, &HttpData[offset], pHexPlugin);
						//printf("offset2 = %d\r\n", offset);
					
				}
				else if (0xa == temp[0])
				{
					if(offset > 0)
					{
						HttpData[offset] = '\0';
						
						sprintf_s((char*) & (Myinfo.url_info[Myinfo.count][0]), HTTP_DATA_LEN, "%s%s", URL_PART1, HttpData);
						sprintf_s(HttpData, HTTP_DATA_LEN, "%4d%02d%02d_%02d", year, month,day, Myinfo.count);

						printf("downloading raw data\r\n");
						download_info(Myinfo.count ,&(Myinfo.url_info[Myinfo.count][0]), HttpData, pHttpPlugin);

						printf("analyzing raw data\r\n");
						AnalyData(HttpData);
						//AnalyData("fdx.txt");

						Myinfo.count++;

					}

					offset = 0;
					
				}
			}
		}
		fclose(Config_File_fp);
	}

	Http_ResContext(pHttpPlugin);

	printf("saving results\r\n");
	save_result();

	printf("Finished\r\n");
	//LogPrintf(log_info, "Finished\r\n");
}