
/*Парсер протокола sbas
* [startbyte] [data1] [data2][data2]....[data16] [flags][endbyte]
* [startbyte] - стартовый байт протокола
* [data1] [data2][data2]....[data16] -данные
* [flags] - байт содержащий признаки о номере канала, о целостности данных, принака режима failsafe
* [endbyte] - конечный байт протокола
*/

#include <stdio.h>
#include <stdint.h>



#define SBUS_STARTBYTE               0x0f    //< стартовый байт протокола sbus
#define SBUS_ENDBYTE                 0x00    //< конечный байт протокола sbus

#define SBUS_NUMBER_OF_CHENNELS      18      //<  количество каналов sbus
#define SBUS_PACKAGE_SIZE_UART_RX    25      //< размер пакета sbus uint8_t

#define  SBUS_DIGITAL_CHANNEL_NO_1   0       //<сдвиг бита в регистре Flags определяющего наличие команды для первого цифрового канала
#define  SBUS_DIGITAL_CHANNEL_NO_2   1       //< сдвиг бита в регистре Flags определяющего наличие команды для второго цифрового канала

#define SBUS_SIGNAL_FAILSAFE         2       // сдвиг бита в регистре Flags определяющего наличие режима failsafe
#define SBUS_FRAME_LOST              3       // сдвиг бита в регистре Flags определяющего наличие признака целостности пакета данных (Frame lost)
#define SBUS_FLAGS_BYTE              23      // номер байта в пакете uint8_t c данными Flags

///структура описывающая блок памяти для хранения данных sbus

typedef struct {
	uint16_t  data[SBUS_NUMBER_OF_CHENNELS];  ///< Поля data протокола sbus 1-16                         
	uint8_t   sbusTrustFlag;                  ///< флаг для блокировки ресурса(при использовании FreeRTOS)
}sbus_st;


///глобальный объект для хранения последних значений цифрового канала 1
sbus_st  sbus_digital_channel_1;

 ///глобальный объект для хранения последних значений цифрового канала 2
sbus_st   sbus_digital_channel_2;

///глобальный объект для хранения преобразованных данных
sbus_st   sbus_buff;


///структура описывающая блок памяти для хранения флагов sbus
typedef struct {
	 _Bool  channelNo_1;                 ///< признак первого цифрового канала
	 _Bool  channelNo_2;                 ///< признак второго цифрового канала
	 _Bool  signalFailSafe;              ///< признак  режима failsafe
	  uint16_t  frameLostCounter;         ///< счетчик поврежденных пакетов       
}sbus_flags;

///глобальный объект для хранения флагов протокола sbus
sbus_flags sbus_flags_t;

/**
 * @brief Парсер посылок sbus
 * @details Данный обработчик позваляет принимать сообщения разной длины
 * и отправляет их в очередь.
 * @param input Указатель на массив принятых значений
 * @param length длина принятого пакета значений
 * @retval None
 */
void parceSbus(uint8_t* input, int length);

/**
 * @brief Приведение протокола sbus из uint8_t в uint16_t
 * @details Данный функция позволяет производить конвертацию 
 * 11 разрядных данных протокола sbus принятых в uint8_t 
 * в uint16_t.
 * @param input Указатель на массив принятых значений
 * @param startByteNuber номер стартового байта в пакете
 * @param nuberDigitalChannelr номер цифрового канала
 * @retval None 
 */
void sbusConvertU8ToU16(uint8_t* input, int length, uint8_t startByteNuber, uint8_t nuberDigitalChannel);


///выбор вывода отладочной ниформации для sbus
typedef enum SBUS_DBG_PORT_E{
	uart,
	xbee,
	usb,
	print
}sbus_dbg_port_t;


/**
 * @brief выводит текущую структуру sbus_st в выбранный порт
 * @param port Указатель на массив принятых значений
 * @param sbus_digital_channel указатель на структуру цифрового канала
 * @retval None
 */
void printSbusSt(sbus_dbg_port_t port, sbus_st  sbus_digital_channel);

/**
 * @brief выводит масси топа uint_8 в консоль
 * @param input указатель на массив
 * @param length длина массива
 * @retval None
 */
void printArrU8(uint8_t* input, int length);

/**
 * @brief выводит масси топа uint_16 в консоль
 * @param input указатель на массив
 * @param length длина массива
 * @retval None
 */
void printArrU16(uint16_t* input, int length);
