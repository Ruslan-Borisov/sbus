
/*������ ��������� sbas
* [startbyte] [data1] [data2][data2]....[data16] [flags][endbyte]
* [startbyte] - ��������� ���� ���������
* [data1] [data2][data2]....[data16] -������
* [flags] - ���� ���������� �������� � ������ ������, � ����������� ������, ������� ������ failsafe
* [endbyte] - �������� ���� ���������
*/

#include <stdio.h>
#include <stdint.h>



#define SBUS_STARTBYTE               0x0f    //< ��������� ���� ��������� sbus
#define SBUS_ENDBYTE                 0x00    //< �������� ���� ��������� sbus

#define SBUS_NUMBER_OF_CHENNELS      18      //<  ���������� ������� sbus
#define SBUS_PACKAGE_SIZE_UART_RX    25      //< ������ ������ sbus uint8_t

#define  SBUS_DIGITAL_CHANNEL_NO_1   0       //<����� ���� � �������� Flags ������������� ������� ������� ��� ������� ��������� ������
#define  SBUS_DIGITAL_CHANNEL_NO_2   1       //< ����� ���� � �������� Flags ������������� ������� ������� ��� ������� ��������� ������

#define SBUS_SIGNAL_FAILSAFE         2       // ����� ���� � �������� Flags ������������� ������� ������ failsafe
#define SBUS_FRAME_LOST              3       // ����� ���� � �������� Flags ������������� ������� �������� ����������� ������ ������ (Frame lost)
#define SBUS_FLAGS_BYTE              23      // ����� ����� � ������ uint8_t c ������� Flags

///��������� ����������� ���� ������ ��� �������� ������ sbus

typedef struct {
	uint16_t  data[SBUS_NUMBER_OF_CHENNELS];  ///< ���� data ��������� sbus 1-16                         
	uint8_t   sbusTrustFlag;                  ///< ���� ��� ���������� �������(��� ������������� FreeRTOS)
}sbus_st;


///���������� ������ ��� �������� ��������� �������� ��������� ������ 1
sbus_st  sbus_digital_channel_1;

 ///���������� ������ ��� �������� ��������� �������� ��������� ������ 2
sbus_st   sbus_digital_channel_2;

///���������� ������ ��� �������� ��������������� ������
sbus_st   sbus_buff;


///��������� ����������� ���� ������ ��� �������� ������ sbus
typedef struct {
	 _Bool  channelNo_1;                 ///< ������� ������� ��������� ������
	 _Bool  channelNo_2;                 ///< ������� ������� ��������� ������
	 _Bool  signalFailSafe;              ///< �������  ������ failsafe
	  uint16_t  frameLostCounter;         ///< ������� ������������ �������       
}sbus_flags;

///���������� ������ ��� �������� ������ ��������� sbus
sbus_flags sbus_flags_t;

/**
 * @brief ������ ������� sbus
 * @details ������ ���������� ��������� ��������� ��������� ������ �����
 * � ���������� �� � �������.
 * @param input ��������� �� ������ �������� ��������
 * @param length ����� ��������� ������ ��������
 * @retval None
 */
void parceSbus(uint8_t* input, int length);

/**
 * @brief ���������� ��������� sbus �� uint8_t � uint16_t
 * @details ������ ������� ��������� ����������� ����������� 
 * 11 ��������� ������ ��������� sbus �������� � uint8_t 
 * � uint16_t.
 * @param input ��������� �� ������ �������� ��������
 * @param startByteNuber ����� ���������� ����� � ������
 * @param nuberDigitalChannelr ����� ��������� ������
 * @retval None 
 */
void sbusConvertU8ToU16(uint8_t* input, int length, uint8_t startByteNuber, uint8_t nuberDigitalChannel);


///����� ������ ���������� ���������� ��� sbus
typedef enum SBUS_DBG_PORT_E{
	uart,
	xbee,
	usb,
	print
}sbus_dbg_port_t;


/**
 * @brief ������� ������� ��������� sbus_st � ��������� ����
 * @param port ��������� �� ������ �������� ��������
 * @param sbus_digital_channel ��������� �� ��������� ��������� ������
 * @retval None
 */
void printSbusSt(sbus_dbg_port_t port, sbus_st  sbus_digital_channel);

/**
 * @brief ������� ����� ���� uint_8 � �������
 * @param input ��������� �� ������
 * @param length ����� �������
 * @retval None
 */
void printArrU8(uint8_t* input, int length);

/**
 * @brief ������� ����� ���� uint_16 � �������
 * @param input ��������� �� ������
 * @param length ����� �������
 * @retval None
 */
void printArrU16(uint16_t* input, int length);
