#include <TMP117.h>
#include <!2C.h>
#include <BOARD.h>
#include <xc.h>
#include <stdio.h>

// #define TMP116_TEST

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
 	uint16_t w:16;
 } __TMP117CONbits;

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                           *
 ******************************************************************************/

 char TMP117_Init(void){
 	unsigned int intReturn;
 	unsigned char byteReturn;

 	intReturn = I2C_Init(I2C_DEFAULT_RATE);
 	if(intReturn != I2C_DEFAULT_RATE)
 		return FALSE;

 	I2C1CONbits.PEN =  1;
 	while(I2C1CONbits.PEN == 1);

 	byteReturn = I2C_ReadRegister(TMP117_I2C_ADDRESS, DEVICE_ID);
 	if(byteReturn != TMP117_DEVICE_ID){
 		printf("TMP117 did not respond to Who Am I\n");
 		return FALSE;
 	}


 	return TRUE;
 }