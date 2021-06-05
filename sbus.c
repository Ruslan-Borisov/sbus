#include "sbus.h"



void parceSbus(uint8_t* input, int length) {
	
		for (int i= 0; i < length; i++)
		{
			if (input[i] == SBUS_STARTBYTE ) 
			{
				if (i + SBUS_PACKAGE_SIZE_UART_RX-1 < length)
				{
					if (input[i+SBUS_PACKAGE_SIZE_UART_RX-1] == SBUS_ENDBYTE) 
					{
					     sbus_flags_t.frameLostCounter += (input[i + SBUS_FLAGS_BYTE] & (1 << SBUS_FRAME_LOST) != 0) ? 1 : 0; // считаем количесво пакетов пришедших со статусом FRAME LOST 
						 sbus_flags_t.channelNo_1 = (input[i + SBUS_FLAGS_BYTE] & (1 << SBUS_DIGITAL_CHANNEL_NO_1) != 0) ? 1 : 0; // определяем является ли пакет первого канала
						 sbus_flags_t.channelNo_2 = (input[i + SBUS_FLAGS_BYTE] & (1 << SBUS_DIGITAL_CHANNEL_NO_2) != 0) ? 1 : 0; // определяем является ли пакет второго канала

						if (sbus_flags_t.channelNo_1 == 1 && sbus_buff.sbusTrustFlag==1) 
						    {
								sbus_buff.sbusTrustFlag = 0;
								sbusConvertU8ToU16(input, length, i, SBUS_DIGITAL_CHANNEL_NO_1); 
							} 
					    if (sbus_flags_t.channelNo_2 == 1 && sbus_buff.sbusTrustFlag ==1) 
							{
								sbus_buff.sbusTrustFlag = 0;
								sbusConvertU8ToU16(input, length, i, SBUS_DIGITAL_CHANNEL_NO_2); 
							}
							else 
							{
								return; 
							}
					}
				}
			}
	}
}
	
void sbusConvertU8ToU16(uint8_t* input, int length, uint8_t startByteNuber, uint8_t nuberDigitalChannel)
{
	 // склеиваем байты из 8 бит в 11 и 16 бит
	 sbus_buff.data[0]  = ((input[startByteNuber +  1]      | input[startByteNuber +  2] << 8) & 0x07FF);
	 sbus_buff.data[1]  = ((input[startByteNuber +  2] >> 3 | input[startByteNuber +  3] << 5) & 0x07FF);
	 sbus_buff.data[2]  = ((input[startByteNuber +  3] >> 6 | input[startByteNuber +  4] << 2
	                                                        | input[startByteNuber+5] << 10) & 0x07FF);
	 sbus_buff.data[3]  = ((input[startByteNuber +  5] >> 1 | input[startByteNuber +  6] << 7) & 0x07FF);
	 sbus_buff.data[4]  = ((input[startByteNuber +  6] >> 4 | input[startByteNuber +  7] << 4) & 0x07FF);
	 sbus_buff.data[5]  = ((input[startByteNuber +  7] >> 7 | input[startByteNuber +  8] << 1
						                                    | input[startByteNuber+9] << 9) & 0x07FF);
	 sbus_buff.data[6]  = ((input[startByteNuber +  9] >> 2 | input[startByteNuber + 10] << 6) & 0x07FF);
	 sbus_buff.data[7]  = ((input[startByteNuber + 10] >> 5 | input[startByteNuber + 11] << 3) & 0x07FF);
	 sbus_buff.data[8]  = ((input[startByteNuber + 12]      | input[startByteNuber + 13] << 8) & 0x07FF);
	 sbus_buff.data[9]  = ((input[startByteNuber + 13] >> 3 | input[startByteNuber + 14] << 5) & 0x07FF);
	 sbus_buff.data[10] = ((input[startByteNuber + 14] >> 6 | input[startByteNuber + 15] << 2
						                                    | input[startByteNuber+16] << 10) & 0x07FF);
	 sbus_buff.data[11] = ((input[startByteNuber + 16] >> 1 | input[startByteNuber + 17] << 7) & 0x07FF);
	 sbus_buff.data[12] = ((input[startByteNuber + 17] >> 4 | input[startByteNuber + 18] << 4) & 0x07FF);
	 sbus_buff.data[13] = ((input[startByteNuber + 18] >> 7 | input[startByteNuber + 19] << 1 | input[startByteNuber+20] << 9) & 0x07FF);
	 sbus_buff.data[14] = ((input[startByteNuber + 20] >> 2 | input[startByteNuber + 21] << 6) & 0x07FF);
     sbus_buff.data[15] = ((input[startByteNuber + 21] >> 5 | input[startByteNuber + 22] << 3) & 0x07FF);

	 sbus_buff.data[16] =  (nuberDigitalChannel == SBUS_DIGITAL_CHANNEL_NO_1) ? 2047 : 0; 
	 sbus_buff.data[17] =  (nuberDigitalChannel == SBUS_DIGITAL_CHANNEL_NO_2) ? 2047 : 0; 

	 if (nuberDigitalChannel == SBUS_DIGITAL_CHANNEL_NO_1 && sbus_digital_channel_1.sbusTrustFlag == 1) 
	 {
		 sbus_digital_channel_1.sbusTrustFlag = 0; 
		 for (uint8_t i = 0; i < SBUS_FLAGS_BYTE-1; i++) 
		 {
			 sbus_digital_channel_1.data[i] = sbus_buff.data[i]; 
		 }
		 sbus_digital_channel_1.sbusTrustFlag = 1; 
		 sbus_buff.sbusTrustFlag = 1;
		 return; 
	 }
	 else if (nuberDigitalChannel == SBUS_DIGITAL_CHANNEL_NO_2 && sbus_digital_channel_2.sbusTrustFlag == 1) 
	 {
		 sbus_digital_channel_2.sbusTrustFlag = 0;
		 for (uint8_t i = 0; i < SBUS_FLAGS_BYTE-1; i++)
		 {
			 sbus_digital_channel_2.data[i] = sbus_buff.data[i];
		 }
		 sbus_digital_channel_2.sbusTrustFlag = 1;
		 sbus_buff.sbusTrustFlag = 1;
		 return;
	 }
	 else 
	 {
		 return; 
	 }

}






void printSbusSt(sbus_dbg_port_t port, sbus_st  sbus_digital_channel) {
	switch (port) {
	case print:
	{
		for (int i = 0; i < SBUS_NUMBER_OF_CHENNELS; i++) {
			printf("%4u", sbus_digital_channel.data[i]);
			if (i < SBUS_NUMBER_OF_CHENNELS - 1) {
				printf(",");
			}
		}
		printf("\n");
		break;
	}
	default:
		return;
	}

}

void printArrU8(uint8_t* input, int length){
	for (int i = 0; i < length; i++) {
		printf("%3u", input[i]);
		if (i < length - 1) {
			printf(",");
		}
	}
	printf("\n");
}

void printArrU16(uint16_t* input, int length) {
	for (int i = 0; i < length; i++) {
		printf("%5u", input[i]);
		if (i < length - 1) {
			printf(",");
		}
	}
	printf("\n");
}

