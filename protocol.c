#include <stdio.h>
#include "lib/sbus.h"
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

typedef struct test{
    uint8_t input_digital_chanel_1[29]; // пакет для цифрового канала 1
	uint8_t input_digital_chanel_2[31]; // пакет для цифрового канала 2
    uint16_t output[18];
}test_t;
int main()
{

	sbus_digital_channel_1.sbusTrustFlag = 1;
	sbus_digital_channel_2.sbusTrustFlag = 1;
	sbus_buff.sbusTrustFlag = 1;


    test_t t1 = {
    .input_digital_chanel_1 = {17, 25, 15, 41,107,214,235, 44,169, 3, 33,187,239, 95, 95, 76,252, 16,236,190,212,237, 81,  6, 69, 77,  0, 40, 30}, 
	.input_digital_chanel_2 = {8, 9, 15, 15, 41,107,214,235, 44,169, 3, 33,187,239, 95, 95, 76,252, 16,236,190,212,237, 81,  6, 69, 78,  0, 14, 18, 19 },
    .output = {809, 717, 943,1174,  58,1602,1006, 767,1119,1929,  67,1910,1355, 987, 404, 552,2047,   0}
    };
    
    parceSbus(t1.input_digital_chanel_1, 29);
	parceSbus(t1.input_digital_chanel_2, 31);
    printArrU16(t1.output, 18);
    printSbusSt(print, sbus_digital_channel_1);
	printSbusSt(print, sbus_digital_channel_2);

    if (memcmp(sbus_digital_channel_1, t1.output, 10) == 0) {
        printf("TEST DONE digital_channel_1\n");
    }
    else {
        printf("TEST FAILED digital_channel_1\n");
    }

	if (memcmp(sbus_digital_channel_2, t1.output, 10) == 0) {
		printf("TEST DONE digital_channel_2\n");
	}
	else {
		printf("TEST FAILED digital_channel_2\n");
	}

}

