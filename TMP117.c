#include <TMP117.h>
#include <!2C.h>
#include <BOARD.h>
#include <xc.h>
#include <stdio.h>

// #define TMP116_TEST

/*********************************************************
 * PRIVATE #DEFINES									     *
**********************************************************/

#define TMP117_I2C_ADDRESS 0x48			// 
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
 } TMP117_REGISTERS