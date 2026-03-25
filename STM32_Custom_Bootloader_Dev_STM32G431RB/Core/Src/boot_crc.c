/*
 * boot_crc.c
 *
 *  Created on: Mar 25, 2026
 *      Author: can.yildirim
 */

#include <stdint.h>

#include "boot_crc.h"
#include "boot_config.h"

#define APP_METADATA_IMAGE_SIZE_OFF  (16UL)
#define APP_METADATA_IMAGE_CRC_OFF   (20UL)

#define CRC32_INIT_VALUE             (0xFFFFFFFFUL)
#define CRC32_FINAL_XOR_VALUE        (0xFFFFFFFFUL)
#define CRC32_POLYNOMIAL_REVERSED    (0xEDB88320UL)



static uint32_t bl_Crc32Calculate(const uint8_t * data, uint32_t length);
static uint32_t bl_ReadUint32FromFlash(uint32_t address);

static uint32_t bl_ReadUint32FromFlash(uint32_t address)
{
    return (*(const uint32_t *)address);
}

static uint32_t bl_Crc32Calculate(const uint8_t * data, uint32_t length)
{
    uint32_t crc = CRC32_INIT_VALUE;
    uint32_t byte_index;
    uint32_t bit_index;

    if (data == (const uint8_t *)0)
    {
        crc = 0UL;
    }

    for (byte_index = 0UL; byte_index < length; byte_index++)
    {
        crc ^= (uint32_t)data[byte_index];

        for (bit_index = 0UL; bit_index < 8UL; bit_index++)
        {
            if ((crc & 1UL) != 0UL)
            {
                crc = (crc >> 1U) ^ CRC32_POLYNOMIAL_REVERSED;
            }
            else
            {
                crc = (crc >> 1U);
            }
        }
    }

    crc ^= CRC32_FINAL_XOR_VALUE;

    return crc;
}

Bl_CrcStatus_t bl_CheckApplicationCrc(uint32_t * calculated_crc, uint32_t * expected_crc)
{
    const uint8_t * app_data_ptr = (const uint8_t *)APP_ADDRESS;
    uint32_t image_size;
    uint32_t stored_crc;
    uint32_t crc_value;
    uint32_t metadata_offset_limit;
    Bl_CrcStatus_t result;

    image_size = bl_ReadUint32FromFlash(APP_METADATA_ADDRESS + APP_METADATA_IMAGE_SIZE_OFF);
    stored_crc = bl_ReadUint32FromFlash(APP_METADATA_ADDRESS + APP_METADATA_IMAGE_CRC_OFF);

    if ((calculated_crc != (uint32_t *)0))
    {
        *calculated_crc = 0UL;
    }

    if ((expected_crc != (uint32_t *)0))
    {
        *expected_crc = stored_crc;
    }

    if (image_size == 0UL)
    {
    	result = BL_CRC_STATUS_SIZE_ERROR;
    }

    /* CRC bölgesi metadata alanına taşmamalı */
    metadata_offset_limit = APP_METADATA_ADDRESS - APP_ADDRESS;

    if (image_size > metadata_offset_limit)
    {
    	result = BL_CRC_STATUS_SIZE_ERROR;
    }

    crc_value = bl_Crc32Calculate(app_data_ptr, image_size);

    if ((calculated_crc != (uint32_t *)0))
    {
        *calculated_crc = crc_value;
    }

    if (crc_value == stored_crc)
    {
    	result = BL_CRC_STATUS_OK;
    }
    else
    {
    	result = BL_CRC_STATUS_MISMATCH;
    }

    return result;
}
