#include "Interfaces.h"
#include "audio_demo_test.h"

#include "stdio.h"

#define REGISTER_TOOL_HEADER_SIZE			24
#define REGISTER_TOOL_READ_REG_MSG_LEN		(REGISTER_TOOL_HEADER_SIZE+16)
#define REGISTER_TOOL_WR_REG_MSG_LEN		(REGISTER_TOOL_HEADER_SIZE+20)
#define REGISTER_TOOL_READ_CONFIG_MSG_LEN	(REGISTER_TOOL_HEADER_SIZE+8)

#define LEFT_CHANNEL_ADDR 0x34
#define RIGHT_CHANNEL_ADDR 0x35

enum
{
	HW_MSG_TYPE_GET_VERSION_REQ = 0,
	HW_MSG_TYPE_GET_VERSION_REP,
	HW_MSG_TYPE_RESET_REQ,
	HW_MSG_TYPE_SET_FREQUENCY_REQ,
	HW_MSG_TYPE_SET_ADC_REQ,
	HW_MSG_TYPE_GET_BATTERYVOL_REQ,
	HW_MSG_TYPE_GET_BATTERYVOL_REP,
	HW_MSG_TYPE_SET_I2CADDRESS_REQ,
	HW_MSG_TYPE_SET_I2CADDRESS_REP,
	HW_MSG_TYPE_REG_REQ,
	HW_MSG_TYPE_REG_REP,
	HW_MSG_TYPE_SET_OWIMODE_REQ,
	HW_MSG_TYPE_SET_OWI_PULSE_REQ,
	HW_MSG_TYPE_UPDATA_FIRMWARE_REQ,
	HW_MSG_TYPE_UPDATA_FIRMWARE_REP,
	HW_MSG_READ_CONFIG,
	HW_MSG_WRITE_CONFIG,
};

typedef enum
{
	PA_LEFT_CHANNEL = 0,
	PA_RIGHT_CHANNEL = 1,
	PA_GROUP = 2,
}PA_CHIPS_t;

typedef struct
{
	uint32_t pktSN;
	uint32_t total_len;
	uint32_t msgSN;
	uint32_t type;
	uint32_t len;
	uint32_t timeout;
	uint32_t data[0x5];
}RegisterTool_Socket_Msg_t;

RegisterTool_Socket_Msg_t read_msg = { 0,0x10,0,HW_MSG_TYPE_REG_REQ ,0x10,0,0,0x34,1,0x3 };

RegisterTool_Socket_Msg_t write_msg = { 0,0x14,0,HW_MSG_TYPE_REG_REQ ,0x14,0,0x1,0x34,0x1,0x20,0x2890 };

RegisterTool_Socket_Msg_t read_config_msg = { 0,0x8,0,	HW_MSG_READ_CONFIG ,0x8,0,PA_LEFT_CHANNEL,1 };

uint16_t register_tool_read_reg(uint16_t slave, uint8_t addr)
{
	uint8_t buffer[0x100];
	uint16_t val = 0, i;

	read_msg.data[1] = slave;
	read_msg.data[3] = addr;

	if (REGISTER_TOOL_READ_REG_MSG_LEN != sizeof(RegisterTool_Socket_Msg_t) - 4)
	{
		printf("socket len is wrong,r\n");
	}

	TCPIP_SocketClientSend((char*)(&read_msg), REGISTER_TOOL_READ_REG_MSG_LEN, &(gTest_config.SocketPluginObject));

	Sleep(10);

	TCPIP_SocketClientRece((char*)buffer, 0x100, &(gTest_config.SocketPluginObject));

	i = sizeof(RegisterTool_Socket_Msg_t) - 4;
	val = buffer[i + 1];
	val = val << 8;
	val |= buffer[i];
#if 0
	printf("rece:");
	for (i = 0; i < val; i++)
	{
		printf("0x%x ", buffer[i]);
	}
	printf("\n");
#else
	LogPrintf(log_info, "read slave[0x%x],addr[0x%x] = [0x%x]\n", slave, addr, val);
#endif
	return val;
}

void register_tool_write_reg(uint16_t slave, uint8_t addr, uint16_t val)
{
	uint8_t buffer[AUDIO_BUFFER_SIZE];

	write_msg.data[1] = slave;
	write_msg.data[3] = addr;
	write_msg.data[4] = val;

#if 1
	LogPrintf(log_info, "write slave[0x%x],addr[0x%x] = [0x%x]\n", slave, addr, val);
#endif

	if (sizeof(RegisterTool_Socket_Msg_t) != REGISTER_TOOL_WR_REG_MSG_LEN)
	{
		LogPrintf(log_info, "socket data len is wrong,w\n");
	}

	TCPIP_SocketClientSend((char*)(&write_msg), REGISTER_TOOL_WR_REG_MSG_LEN, &(gTest_config.SocketPluginObject));

}

int tfa_set_bf_volatile_by_regmap(uint16_t slave, const uint16_t bf, const uint16_t value)
{
	uint16_t regvalue, msk;
	/*
	* bitfield enum:
	* - 0..3  : len
	* - 4..7  : pos
	* - 8..15 : address
	*/
	uint8_t len = bf & 0x0f;
	uint8_t pos = (bf >> 4) & 0x0f;
	uint8_t address = (bf >> 8) & 0xff;

	//regvalue = snd_soc_read(codec, address);
	//regmap_read(pSia9118->regmap, address, &regvalue);
	regvalue = register_tool_read_reg(slave, address);

	msk = ((1 << (len + 1)) - 1) << pos;
	regvalue &= ~msk;
	regvalue |= value << pos;

	//snd_soc_write(codec, address, regvalue);
	//regmap_write(pSia9118->regmap, address, regvalue);
	register_tool_write_reg(slave, address, regvalue);

	return 0;
}

static int hw_configruation(uint16_t slave,
	unsigned int rate,
	int sample_width,
	int audio_in_slot_sel,
	int audio_out_slot_sel,
	int volum)
{
	unsigned int fs;
	switch (rate)
	{
	case 16000:
		fs = 0x3;
		break;
	case 32000:
		fs = 0x6;
		break;
	case 44100:
		fs = 0x7;
		break;
	case 48000:
		fs = 0x8;
		break;
	case 96000:
		fs = 0x9;
		break;
	default:
		return -EINVAL;
	}

	switch (sample_width)
	{
	case 8:
		break;
	case 16:
		break;
	case 20:
		break;
	case 24:
		break;
	case 32:
		break;
	default:
		return -EINVAL;
	}

	printf("****************************************write register start***************************\n");

	//reset
	tfa_set_bf_volatile_by_regmap(slave, TFA9874_BF_I2CR, 1);
	//enter load i2c settings
	tfa_set_bf_volatile_by_regmap(slave, TFA9874_BF_PWDN, 0);

	//-6db
	tfa_set_bf_volatile_by_regmap(slave, TFA9874_BF_INPLEV, 1);

	//rising/falling edge trigger T/R
	//tfa_set_bf_volatile(codec, TFA9874_BF_TDMCLINV,1);

	//enalbe voltage, current sense
	tfa_set_bf_volatile_by_regmap(slave, TFA9874_BF_TDMVSE, 1);
	tfa_set_bf_volatile_by_regmap(slave, TFA9874_BF_TDMCSE, 1);
	//channel 1 enable/disable
	//if(pSia9118->feature_flag)
	{
		tfa_set_bf_volatile_by_regmap(slave, TFA9874_BF_TDMDCE, 1);
	}
	//else
	{
		//tfa_set_bf_volatile_by_regmap(pSia9118, TFA9874_BF_TDMDCE, 0);
	}

	//channel 0 enable/disable
	tfa_set_bf_volatile_by_regmap(slave, TFA9874_BF_TDMSPKE, 1);

	//out mapping mode: 32bits compress mode
	tfa_set_bf_volatile_by_regmap(slave, TFA9874_BF_TDMSRCMAP, 0x3);


	//out slot for current
	tfa_set_bf_volatile_by_regmap(slave, TFA9874_BF_TDMCSS, audio_out_slot_sel);
	//out slot for voltage
	tfa_set_bf_volatile_by_regmap(slave, TFA9874_BF_TDMVSS, audio_out_slot_sel);

	//in slot for sink0
	tfa_set_bf_volatile_by_regmap(slave, TFA9874_BF_TDMSPKS, audio_in_slot_sel);

#if 1
	printf("spks,dcs slot different\n");
	tfa_set_bf_volatile_by_regmap(slave, TFA9874_BF_TDMDCS, audio_out_slot_sel);
#else
	printf("spks,dcs slot same\n");
	tfa_set_bf_volatile_by_regmap(slave, TFA9874_BF_TDMDCS, audio_in_slot_sel);
#endif

	register_tool_write_reg(slave, 0x61, volum);

	tfa_set_bf_volatile_by_regmap(slave, TFA9874_BF_AUDFS, fs);

	tfa_set_bf_volatile_by_regmap(slave, TFA9874_BF_TDMSSIZE, sample_width - 1);

	//AMPE
	tfa_set_bf_volatile_by_regmap(slave, TFA9874_BF_AMPE, 1);

	//Finish I2C configuration
	tfa_set_bf_volatile_by_regmap(slave, TFA9874_BF_MANSCONF, 1);

	printf("****************************************write register end***************************\n");

	return 0;
}

void Audio_Register_Tool_Test(void)
{
	uint32_t rate, i, ret;
	char ServerAddr[] = "192.168.1.5";
	int total_count, current_count = 0;
	uint32_t audio_data_end_pos, audio_data_start_pos;
	int total_size = 0, readlen = 0, sendlen = 0, count = 0, file_read_size = 0;

	gTest_config.Tunl_Config.pBuffer = ServerAddr;
	gTest_config.Tunl_Config.port = 2828;
	gTest_config.Tunl_Config.pSocket = &gTest_config.SocketPluginObject.Socket;

	gTest_config.SocketPluginObject.pAudioSocketPluginObject = CreatePlugin(PLUGIN_SOCKET, clsidSocketLib);

	if (RET_OK == TCPIP_SocketClientConnect(&(gTest_config.Tunl_Config), &(gTest_config.SocketPluginObject)))
	{
		while (1)
		{
			printf("please input command:\n");

			scanf("%s", gBuffer);

			switch (gBuffer[0])
			{
			case 'c':
			case 'C':
				if ('0' == gBuffer[1])
					tfa_set_bf_volatile_by_regmap(LEFT_CHANNEL_ADDR, TFA9874_BF_I2CR, 1);
				else if ('1' == gBuffer[1])
					tfa_set_bf_volatile_by_regmap(RIGHT_CHANNEL_ADDR, TFA9874_BF_I2CR, 1);
				else
				{
					printf("channel num error[%c]\n", gBuffer[1]);
				}

				break;
			case 'l':
			case 'L':
				if ('0' == gBuffer[1])
				{
					read_config_msg.data[0] = PA_LEFT_CHANNEL;
				}
				else if ('1' == gBuffer[1])
				{
					read_config_msg.data[0] = PA_RIGHT_CHANNEL;
				}
				else
				{
					printf("channel num error[%c]\n", gBuffer[1]);
					break;
				}

				TCPIP_SocketClientSend((char*)(&read_config_msg), REGISTER_TOOL_READ_CONFIG_MSG_LEN, &(gTest_config.SocketPluginObject));
				break;
			case 'o':
			case 'O':
				if ('0' == gBuffer[1])
				{
					hw_configruation(LEFT_CHANNEL_ADDR, 44100, 16, 0, 1, 0x3fc);
					register_tool_read_reg(LEFT_CHANNEL_ADDR, 0x02);
					register_tool_read_reg(LEFT_CHANNEL_ADDR, 0x22);
					register_tool_read_reg(LEFT_CHANNEL_ADDR, 0x23);
					register_tool_read_reg(LEFT_CHANNEL_ADDR, 0x26);
					register_tool_read_reg(LEFT_CHANNEL_ADDR, 0x27);
					register_tool_read_reg(LEFT_CHANNEL_ADDR, 0x61);
					register_tool_read_reg(LEFT_CHANNEL_ADDR, 0x68);

				}
				else if ('1' == gBuffer[1])
				{
					hw_configruation(RIGHT_CHANNEL_ADDR, 44100, 16, 1, 0, 0x3fc);
					register_tool_read_reg(RIGHT_CHANNEL_ADDR, 0x02);
					register_tool_read_reg(RIGHT_CHANNEL_ADDR, 0x22);
					register_tool_read_reg(RIGHT_CHANNEL_ADDR, 0x23);
					register_tool_read_reg(RIGHT_CHANNEL_ADDR, 0x26);
					register_tool_read_reg(RIGHT_CHANNEL_ADDR, 0x27);
					register_tool_read_reg(RIGHT_CHANNEL_ADDR, 0x61);
					register_tool_read_reg(RIGHT_CHANNEL_ADDR, 0x68);
				}
				else
				{
					printf("channel num error[%c]\n", gBuffer[1]);
				}
				break;
			case 'r':
			case 'R':
				//get_capture_data_flag = 1;
				break;

			}

		}
	}
	else
	{
		LogPrintf(log_critical, "Socket client connect failed");
	}
}