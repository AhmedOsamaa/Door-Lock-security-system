/*
 * external_eeprom.h
 *
 *  Created on: 24 Oct 2021
 *      Author: ahmed
 */

#ifndef EXTERNAL_EEPROM_H_
#define EXTERNAL_EEPROM_H_

#include "std_types.h"

/*Pre-processor Macros*/
#define ERROR 0
#define SUCCESS 1

/*Function Prototypes*/
uint8 EEPROM_writeByte(uint16 u16addr, uint8 u8data);
uint8 EEPROM_readByte(uint16 u16addr, uint8 *u8data);

#endif /* EXTERNAL_EEPROM_H_ */
