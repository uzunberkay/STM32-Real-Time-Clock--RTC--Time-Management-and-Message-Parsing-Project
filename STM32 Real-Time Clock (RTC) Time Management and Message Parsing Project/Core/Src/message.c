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
#include "Message.h"
// Global olarak tanımlanan mesaj değişkeni
Message globalMessage;


uint16_t SLAVE_ADDRESS = 0xFFFF;
/**
 * @brief Hexadecimal bir diziyi temsil eden bir char dizisinden uint8_t degeri cikarmak icin kullanilir.
 * @param startIndex: Başlangic indeksi
 * @param hex: Hexadecimal diziyi temsil eden char dizisi
 * @retval uint8_t turunde cikartilan deger
 */
uint8_t hexToUint8(int startIndex, char hex[]) {
    uint8_t result = 0;
    uint8_t length=2;	// Bir uint8_t icin hexadecimal değer 2 karakterlidir.

    for (uint8_t i = startIndex; i < startIndex + length; i++) {
        result = result << 4;	// 4 bit sola kaydırma işlemi yapılır.

        if (hex[i] >= '0' && hex[i] <= '9') {
            result |= hex[i] - '0';			// '0' ile '9' arasındaki karakterleri isler.
        } else if (hex[i] >= 'A' && hex[i] <= 'F') {
            result |= hex[i] - 'A' + 10;	// 'A' ile 'F' arasındaki karakterleri isler.
        } else if (hex[i] >= 'a' && hex[i] <= 'f') {
            result |= hex[i] - 'a' + 10;	// 'a' ile 'f' arasındaki karakterleri isler.
        } else {
            return -1;						// Gecersiz karakter durumunda -1 dondurulur.

        }
    }

    return result;
}

/**
 * @brief Hexadecimal bir diziyi temsil eden bir char dizisinden uint16_t degeri cikarmak icin kullanilir.
 * @param startIndex: Başlangic indeksi
 * @param hex: Hexadecimal diziyi temsil eden char dizisi
 * @retval uint16_t turunde cikartilan deger
 */
uint16_t hexToUint16(int startIndex, char hex[]) {
    uint16_t result = 0;
    uint8_t length=4;	// Bir uint16_t icin hexadecimal değer 4 karakterlidir.

    for (uint8_t i = startIndex; i < startIndex + length; i++) {
        result = result << 4;	// 4 bit sola kaydirma islemi yapılır.

        if (hex[i] >= '0' && hex[i] <= '9') {
            result |= hex[i] - '0';				// '0' ile '9' arasındaki karakterleri isler.
        } else if (hex[i] >= 'A' && hex[i] <= 'F') {
            result |= hex[i] - 'A' + 10;		// 'A' ile 'F' arasındaki karakterleri isler.
        } else if (hex[i] >= 'a' && hex[i] <= 'f') {
            result |= hex[i] - 'a' + 10;		// 'a' ile 'f' arasındaki karakterleri isler.
        } else {
            return -1;							// Gecersiz karakter durumunda -1 dondurulur.

        }
    }

    return result;
}

/**
 * @brief Hexadecimal bir diziyi temsil eden bir char dizisinden uint32_t degeri cikarmak icin kullanilir.
 * @param startIndex: Başlangic indeksi
 * @param hex: Hexadecimal diziyi temsil eden char dizisi
 * @retval uint32_t turunde cikartilan deger
 */
uint32_t hexToUint32(int startIndex, char hex[]) {
    uint32_t result = 0;
    uint8_t length=8;	// Bir uint32_t icin hexadecimal değer 8 karakterlidir.

    for (uint8_t i = startIndex; i < startIndex + length; i++) {
        result = result << 4;	// 4 bit sola kaydirma islemi yapılır.

        if (hex[i] >= '0' && hex[i] <= '9') {
            result |= hex[i] - '0';				// '0' ile '9' arasındaki karakterleri isler.
        } else if (hex[i] >= 'A' && hex[i] <= 'F') {
            result |= hex[i] - 'A' + 10;		// 'A' ile 'F' arasındaki karakterleri isler.
        } else if (hex[i] >= 'a' && hex[i] <= 'f') {
            result |= hex[i] - 'a' + 10;		// 'a' ile 'f' arasındaki karakterleri isler.
        } else {
            return -1;							// Gecersiz karakter durumunda -1 dondurulur.

        }
    }

    return result;
}

/**
  * @brief  RTC tarih ve saat bilgilerini okur ve bunları USB araciligiyla iletisim portuna gonderir.
  * @return Islemin basarili olup olmadigini gosteren hata kodu.
  *         - NO_ERROR: Islem basariyla tamamlandi.
  */
int readCommand() {
	// RTC tarih ve saat bilgilerini iceren bir dizi olustur.
	sprintf(dizi,"\nSistem Saati: %d/%d/%d %d:%d:%d",Int_RTC_Date.Date,Int_RTC_Date.Month,Int_RTC_Date.Year,Int_RTC_Time.Hours,Int_RTC_Time.Minutes,Int_RTC_Time.Seconds);

	// Olusturulan diziyi USB araciligiyla iletisim portuna gonder
	CDC_Transmit_FS((uint8_t*)dizi, strlen(dizi));

	// Islem basariyla tamamlandi
    return NO_ERROR;
}

/**
  * @brief  Gelen bir mesaj dizisini analiz eder ve analiz sonuclarina gore RTC tarihini ve saatini ayarlar.
  * @param  messageString: Tarih ve saat bilgilerini iceren mesaj dizisinin isaretcisi.
  * @return Islemin basarili olup olmadıgini gosteren hata kodu.
  *         - NO_ERROR: Islem basariyla tamamlandi.
  *         - INVALID_DATE: Mesaj icinde gecersiz tarih veya saat parametreleri.
  */
int writeCommand(char* messageString) {
    uint8_t day = hexToUint8(DAY_INDEX, messageString);
    uint8_t month = hexToUint8(MONTH_INDEX, messageString);
    uint8_t year = hexToUint8(YEAR_INDEX, messageString);
    uint8_t hours = hexToUint8(HOURS_INDEX, messageString);
    uint8_t minutes = hexToUint8(MINUTES_INDEX, messageString);
    uint8_t seconds = hexToUint8(SECONDS_INDEX, messageString);

    // Cikarilan tarih ve saat bilesenlerini dogrula
    if (day > 31 || month > 12 || year > 99 || hours > 23 || minutes > 59 || seconds > 59) {
        return INVALID_DATE;
    }
    // RTC tarihini ve saatini ayarla
    globalMessage.payload.timestamp.day=Int_RTC_Date.Date = day;
    globalMessage.payload.timestamp.month=Int_RTC_Date.Month = month;
    globalMessage.payload.timestamp.year=Int_RTC_Date.Year = year;
	HAL_RTC_SetDate(&hrtc, &Int_RTC_Date, RTC_FORMAT_BIN);

	globalMessage.payload.timestamp.hour=Int_RTC_Time.Hours = hours;
	globalMessage.payload.timestamp.minute=Int_RTC_Time.Minutes = minutes;
	globalMessage.payload.timestamp.second=Int_RTC_Time.Seconds = seconds;
    HAL_RTC_SetTime(&hrtc, &Int_RTC_Time, RTC_FORMAT_BIN);

    // Islem basariyla tamamlandi
    return NO_ERROR;
}


/**
 * @brief Mesaj stringini analiz ederek globalMessage yapisini dolduran ve mesaji isleyen bir fonksiyon.
 *
 * Bu fonksiyon, verilen mesaj stringini analiz eder, gecerliligini kontrol eder ve
 * globalMessage yapısına uygun degerleri atar. Ayrica, mesaj turune bagli olarak
 * ozel islemleri gerceklestirir (ornegin, belirli bir komut numarasina veya adresine göre kontrol).
 *
 * @param messageString: Analiz edilecek mesaj stringi.
 * @return ErrorCode: Fonksiyonun basarılı olup olmadığını belirten hata kodu.
 */
ErrorCode parseMessage(char* messageString) {
    // Başlangıç karakterini kontrol et
    if (messageString[0] != 0xA3) // '£' karakteri kabul etmedigi icin ASCII karsiligi olan hex degerini kullandim.
    {
        return INVALID_START_CHAR;
    }
    // Bitiş karakterini kontrol et
    if (messageString[strlen(messageString)-1] != '%') {
        return INVALID_END_CHAR;
    }

    // Slave address kontrol et.
    if(SLAVE_ADDRESS!=hexToUint16(SLAVE_NUMBER_START_INDEX, messageString))
    {
    	return INVALID_SLAVE_ADDRESS;

    }
    // Emir turunu kontrol et.
    globalMessage.commandType = hexToUint8(COMMAND_TYPE_START_INDEX,messageString);

    if (globalMessage.commandType == READ_COMMAND && strlen(messageString) != READ_Message_Length) {
        return INVALID_COMMAND_TYPE;
    } else if (globalMessage.commandType == WRITE_COMMAND && strlen(messageString) != WRITE_Message_Length) {
    	return INVALID_COMMAND_TYPE;
    }
    else{
    	return INVALID_COMMAND_TYPE;
    }

    globalMessage.start=messageString[0];
    globalMessage.end=messageString[strlen(messageString)-1];
    globalMessage.slaveAddress= hexToUint16(SLAVE_NUMBER_START_INDEX, messageString);

    switch (globalMessage.commandType) {
            case READ_COMMAND:
                return readCommand();
            case WRITE_COMMAND:
                return writeCommand(messageString);
            default:
                return INVALID_COMMAND_TYPE;
        }

}
/**
 * @brief Analiz edilmis bir mesajin durumuna gore islem yapar ve gerekirse hata mesaji gonderir.
 *
 * @param buffer: Analiz edilmis mesajin string temsilcisi.
 */
void parseData(char *buffer)
{
	ErrorCode result = parseMessage(buffer);

	if (result != NO_ERROR) {
	        switch (result) {
	            case INVALID_END_CHAR:
	                sprintf(dizi, "Bitis karakteri hatali!");
	                break;
	            case INVALID_DATA_SIZE:
	                sprintf(dizi, "Gecersiz veri uzunlugu!");
	                break;
	            case INVALID_START_CHAR:
	                sprintf(dizi, "Gecersiz baslangic karakteri!");
	                break;
	            case INVALID_DATE:
	                sprintf(dizi, "Gecersiz Tarih!");
	                break;
	            case INVALID_SLAVE_ADDRESS:
	                sprintf(dizi, "Gecersiz slave addresi!");
	                break;
	            case INVALID_COMMAND_TYPE:
	            	sprintf(dizi,"Gecersiz emir turu!\n ");
	            	break;
	            default:
	                break;
	        }

	        CDC_Transmit_FS((uint8_t*)dizi, strlen(dizi));
	        errorMessageCount++;
	    } else {
	        successfulMessageCount++;
	    }
}












