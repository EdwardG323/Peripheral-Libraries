/**
 * File:	TMP117.h
 * Auhtor: Edward Garcia
 * 
 * Module to comunicate with the TMP over I2C
 * Provides access to raw temperature reading
 */

#pragma once
#ifndef TMP117_H
#define TMP117_H

/**
 * @Function TMP117_Init(void)
 * @Param	None
 * @Brief	Initializes the TMP117 for usage
 * @Author	Edward Garcia
*/
char TMP117_Init(void);

/**
 * @Function	TMP117_ReadTemp(void)
 * @Param	None
 * @Brief	Reads sensor temperature reading
 * @Return 	Returns raw temperature reading
 * @Author	Edward Garcia 
 */
int TMP117_ReadTemp(void);

/**
 * @Function    TMP117_ReadTempC(void)
 * @Param   None
 * @Brief   Reads sensor temperature reading
 * @Return  Returns temperature reading in celsius
 * @Author  Edward Garcia 
 */
int TMP117_ReadTempC(void);

/**
 * @Function    TMP117_ReadTempF(void)
 * @Param   None
 * @Brief   Reads sensor temperature reading
 * @Return  Returns temperature reading in farenheight
 * @Author  Edward Garcia 
 */
int TMP117_ReadTempF(void);

#endif