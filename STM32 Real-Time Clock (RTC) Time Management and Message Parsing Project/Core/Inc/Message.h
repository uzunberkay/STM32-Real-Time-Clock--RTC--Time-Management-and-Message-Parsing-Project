/*______________________________________________
* Berkay Uzun									|
*												|
*												|
* 												|
* 												|
* Date          : 08.01.2024					|
* By            : Berkay UZUN					|
* e-mail        : berkyuzun@gmail.com			|
* ______________________________________________|
*/
#ifndef INC_MESSAGE_H_
#define INC_MESSAGE_H_
#include <stdint.h>
#include "usbd_cdc_if.h"


extern RTC_HandleTypeDef hrtc;
extern RTC_TimeTypeDef Int_RTC_Time;
extern RTC_DateTypeDef Int_RTC_Date;
extern char dizi[64];			// Genel amacli bir karakter dizisi
extern int successfulMessageCount; // Hatasız mesaj sayacı
extern int errorMessageCount;     // Hatalı mesaj sayacı

/* ----------------------> ENUM TANIMLAMALARI <----------------------------- */
typedef enum {
    READ_COMMAND = 0,  // Okuma emiri
    WRITE_COMMAND      // Yazma emiri
} CommandType;

typedef enum {
    NO_ERROR,				// Hata yok
    INVALID_START_CHAR,		// Gecersiz baslangic karakteri
    INVALID_END_CHAR,		// Gecersiz bitis karakteri
	INVALID_DATA_SIZE,
	INVALID_SLAVE_ADDRESS,
	INVALID_DATE,
	INVALID_COMMAND_TYPE

} ErrorCode;

/* ----------------------> MESAJ YAPISI <----------------------------- */
typedef struct {
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} DateTime;

typedef struct {
    char start;              // Başlangıç karakteri (£)
    uint32_t slaveAddress; // Emir adresi
    CommandType commandType; // Read veya Write komutu
    struct {
        DateTime timestamp;  // İç içe struct
    } payload;
    char end;                 // Bitiş karakteri (%)
} Message;

/*--------------------> Mesaj yapisi icin baslangic degerleri<-----------------------------*/

#define SLAVE_NUMBER_START_INDEX 1		// Birim numarasi icin başlangic indeksi
#define COMMAND_TYPE_START_INDEX 5		// Emir turu icin başlangic indeksi
#define DAY_INDEX 7                    // Gün indeksi
#define MONTH_INDEX 9                     // Ay indeksi
#define YEAR_INDEX 11                   // Yıl indeksi
#define HOURS_INDEX 13                  // Saat indeksi
#define MINUTES_INDEX 15                // Dakika indeksi
#define SECONDS_INDEX 17                // Saniye indeksi
#define WRITE_Message_Length 20
#define READ_Message_Length 8






/* ----------------------> FONKSIYON PROTOTIPLERI <----------------------------- */
ErrorCode parseMessage(char*);
void parseData(char*);
uint8_t hexToUint8(int,char[]);
uint16_t hexToUint16(int,char[]);
uint32_t hexToUint32(int,char[]);
int readCommand();
int writeCommand(char*);



#endif /* INC_MESSAGE_H_ */
