/*
 * boot_image.c
 *
 *  Created on: Mar 24, 2026
 *      Author: can.yildirim
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "boot_image.h"
#include "boot_config.h"
#include "std_def.h"
#include "boot_crc.h"


typedef struct
{
	uint32_t magic;
	uint32_t format_version;
	uint32_t image_version;
	uint32_t image_start_address;
	uint32_t image_size_bytes;
	uint32_t image_crc32;
	uint32_t vector_table_addr;
	uint32_t reserved;
} BootImageMetadata_t;

const BootImageMetadata_t *pMeta = (const BootImageMetadata_t*)APP_METADATA_ADDRESS;


bool bi_IsMetaDataValid()
{
	uint8_t result = TRUE;

	if(pMeta->magic != BOOT_METADATA_MAGIC)
	{
		printf("Boot: Metadata magic number is not valid!\r\n");
		result = FALSE;
	}
	else
	{
		printf("Boot: Metadata magic number is checked and valid\r\n");
	}

	if(pMeta->format_version != BOOT_METADATA_FORMAT_VERSION)
	{
		printf("Boot: Metadata format version is not valid!\r\n");
		result = FALSE;
	}
	else
	{
		printf("Boot: Metadata format version is checked and valid\r\n");
	}

	if(pMeta->image_start_address != APP_ADDRESS)
	{
		printf("Boot: Metadata application address is not valid!\r\n");
		result = FALSE;
	}
	else
	{
		printf("Boot: Metadata application address is checked and valid\r\n");
	}

	if(pMeta->vector_table_addr != APP_ADDRESS)
	{
		printf("Boot: Metadata app vector table address is not valid!\r\n");
		result = FALSE;
	}
	else
	{
		printf("Boot: Metadata app vector table is checked and valid\r\n");
	}

	if((pMeta->image_size_bytes == 0U) || (pMeta->image_size_bytes > APP_MAX_SIZE_BYTES) ||
	   ((pMeta->image_start_address + pMeta->image_size_bytes) > APP_METADATA_ADDRESS))
	{
		printf("Boot: Metadata size is not valid!\r\n");
		result = FALSE;
	}
	else
	{
		printf("Boot: Metadata size is check and valid!\r\n");
	}

	if((pMeta->image_crc32 == 0U) || (pMeta->image_crc32 == 0xFFFFFFFFU))
	{
		printf("Boot: Metadata image crc32 field is invalid!\r\n");
		result = FALSE;
	}
	else
	{
		printf("Boot: Metadata image crc32 field is valid\r\n");
	}

	uint32_t calc_crc = 0UL;
	uint32_t ref_crc = 0U;
	Bl_CrcStatus_t crc_status;

	ref_crc = pMeta->image_crc32;
	crc_status = bl_CheckApplicationCrc(&calc_crc, &ref_crc);

	switch(crc_status)
	{

		case BL_CRC_STATUS_OK:
			printf("Boot: crc32 check is valid. Return code: BL_CRC_STATUS_OK\r\n");
		break;

		case BL_CRC_STATUS_INVALID_PARAM:
			printf("Boot: crc32 check is not valid. Return code: BL_CRC_STATUS_INVALID_PARAM");
			result = FALSE;
		break;

		case BL_CRC_STATUS_SIZE_ERROR:
			printf("Boot: crc32 check is not valid. Return code: BL_CRC_STATUS_SIZE_ERROR");
			result = FALSE;
		break;

		case BL_CRC_STATUS_MISMATCH:
			printf("Boot: crc32 check is not valid. Return code: BL_CRC_STATUS_MISMATCH");
			result = FALSE;
		break;

		default:
			// Do not enter here
			result = FALSE;
		break;
	}

	return result;
}
