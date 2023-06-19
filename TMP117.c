/**
 * File:    TMP117.c
 * Auhtor: Edward Garcia
 * 
 * Module to comunicate with the TMP over I2C
 * Provides access to raw temperature reading
 */
#include <TMP117.h>
#include <I2C.h>
#include <BOARD.h>      // UNO32 board header file
#include <xc.h>         // Microchip library
#include <stdio.h>


#define TMP117_TEST

/*********************************************************
 * PRIVATE #DEFINES									     *
**********************************************************/

#define TMP117_I2C_ADDRESS 0x48			// Default address
#define TMP117_DEVICE_ID 0x0117			// Found in page23 of datasheet
 
#define TMP117_RESOLUTION 0.0078125f	// Found on page1 of datasheet

#define TMP117_CONTINUOUS_MODE 0b00
#define TMP117_ONE_SHOT_MODE 0b11
#define TMP117_SHUTDOWN_MODE 0b01 

/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/

 static enum {
 	TEMP_RESULT = 0X00,		// Temperature result register
 	CONFIGURATION = 0X01,	// Confiuguration register
 	THIGH_LIMIT = 0X02,		// Tmeperature high limit register
 	TLOW_LIMIT = 0X03,		// Temperature low limit register
 	EEPROM_UL = 0X04,		// EEPROM unlock register
 	EEPROM1 = 0X05,			// EEPROM1 register
 	EEPROM2 = 0X06,			// EEPROM2 register
 	TEMP_OFFSET = 0X07,		// Temperature offset register
 	EEPROM3 = 0X08,			// EEPROM3 register
 	DEVICE_ID = 0X0F		// Device ID register
 } TMP117_REGISTERS;


typedef union{
 	struct{
 		uint16_t EMPTY:1;		// Empty bit not used
 		uint16_t SOFT_RESET:1;	// Software reset bit
 		uint16_t DR_ALERT:1;	// ALERT pin select bit
 		uint16_t POL:1;			// ALERT pin polarity bit
 		uint16_t T_NA:1;		// Therm/alert mode select
 		uint16_t AVG:2;			// Conversion averaging modes
 		uint16_t CONV:3;		// Conversion cycle bit
 		uint16_t MOD:2;			// Set conversion bit
 		uint16_t EEPROM_BUSY:1;	// EEPROM busy flag
 		uint16_t DATA_READY:1;	// Data ready flag
 		uint16_t LOW_ALERT:1;	// Low alert flag
 		uint16_t HIGH_ALERT:1;	// High alert flag
 	};
    struct{
        uint16_t w:16;
    };
 	
 } __TMP117CONbits;

volatile __TMP117CONbits TMP117CONbits;

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                           *
 ******************************************************************************/


/**
 * @Function TMP117_Init(void)
 * @Param   None
 * @Brief   Initializes the TMP117 for usage
 * @Author  Edward Garcia
*/
 char TMP117_Init(void){
 	unsigned int intReturn;
 	int IDReturn;

 	intReturn = I2C_Init(I2C_DEFAULT_RATE);
 	if(intReturn != I2C_DEFAULT_RATE)
 		return FALSE;

 	I2C1CONbits.PEN =  1;
 	while(I2C1CONbits.PEN == 1);

 	IDReturn = I2C_ReadInt(TMP117_I2C_ADDRESS, DEVICE_ID, TRUE);
 	if(IDReturn != TMP117_DEVICE_ID){
 		printf("TMP117 did not respond to Who Am I\n\r");
        printf("Expected ID value: %x\n\rActual return value: %x\n\r", TMP117_DEVICE_ID
            , IDReturn);
 		return FALSE;
 	}

 	return TRUE;
}
 
/**
 * @Function	TMP117_ReadTemp(void)
 * @Param	None
 * @Brief	Reads sensor temperature reading
 * @Return 	Returns raw temperature reading
 * @Author	Edward Garcia 
 */
int TMP117_ReadTemp(void){
    return I2C_ReadInt(TMP117_I2C_ADDRESS,  TEMP_RESULT, TRUE);
}

/**
 * @Function    TMP117_ReadTempC(void)
 * @Param   None
 * @Brief   Reads sensor temperature reading
 * @Return  Returns temperature reading in celsius
 * @Author  Edward Garcia 
 */
int TMP117_ReadTempC(void){
    return I2C_ReadInt(TMP117_I2C_ADDRESS, TEMP_RESULT, TRUE) * TMP117_RESOLUTION;
}

/**
 * @Function    TMP117_ReadTempF(void)
 * @Param   None
 * @Brief   Reads sensor temperature reading
 * @Return  Returns temperature reading in farenheight
 * @Author  Edward Garcia 
 */
int TMP117_ReadTempF(void){
    return (TMP117_ReadTempC() * 1.8) + 32;
}

#ifdef TMP117_TEST

#include "serial.h"
#include <Oled.h>
#include <string.h>
#include <stdlib.h>

 int main(int argc, char** argv){
 
    char string2[OLED_CHARS_PER_LINE];
    char initResult = FALSE;
    BOARD_Init();
    OledInit();
    SERIAL_Init();      // BOARD_Init should already call SERIAL_Init
    

    if (IsTransmitEmpty()){
        printf("Welcome to the TMP117 test compiled at " __DATE__" " __TIME__ ". Sensor" 
            " will be brought up and values displayed\r\n");
    }
    

    initResult = TMP117_Init();
    if(initResult != TRUE){
        printf("Initialization of TMP117 failed, stopping here.\r\n");
    }
    else{
        printf("Initialization succeeded\r\n");
        while(1){
            int config = I2C_ReadInt(TMP117_I2C_ADDRESS, CONFIGURATION, TRUE);
            printf("Default configuration register: %x\r\n", config);

            //config ^= 0x0c00;

            // Read Raw temp data
            int rawTemp = TMP117_ReadTemp();
            // Convert to celsius
            int TempC = rawTemp * TMP117_RESOLUTION;
            // Display on OLED
            sprintf(string2, "Temperature: %d%sc", TempC, DEGREE_SYMBOL);
            OledClear(OLED_COLOR_BLACK);
            OledDrawString(string2);
            OledUpdate();
        }
    }
    
    

    while(1);
    
    return (EXIT_SUCCESS);
 }
 
#endif