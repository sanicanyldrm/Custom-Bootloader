/*
 * boot_crc.h
 *
 *  Created on: Mar 25, 2026
 *      Author: can.yildirim
 */

#ifndef INC_BOOT_CRC_H_
#define INC_BOOT_CRC_H_

typedef enum
{
    BL_CRC_STATUS_OK = 0,
    BL_CRC_STATUS_INVALID_PARAM,
    BL_CRC_STATUS_SIZE_ERROR,
    BL_CRC_STATUS_MISMATCH,
	BL_CRC_STATUS_MAX
} Bl_CrcStatus_t;


Bl_CrcStatus_t bl_CheckApplicationCrc(uint32_t * calculated_crc, uint32_t * expected_crc);

#endif /* INC_BOOT_CRC_H_ */
