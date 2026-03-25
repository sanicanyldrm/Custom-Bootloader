/*
 * app_metadata.c
 *
 *  Created on: Mar 24, 2026
 *      Author: can.yildirim
 */

#include "boot_image_metadata.h"
#include "app_version.h"
#include "boot_memory_map.h"

//#define APP_METADATA_PLACEHOLDER_U32   (0xFFFFFFFFUL)

const BootImageMetadata_t AppMetadata __attribute__((section(".app_metadata"))) =
{
		.magic = BOOT_METADATA_MAGIC,
		.format_version = BOOT_METADATA_FORMAT_VERSION,
		.image_version = APP_IMAGE_VERSION,
		.image_start_address = APP_START_ADDRESS,
		.image_size_bytes = 0U,
		.image_crc32 = 0U,
		.vector_table_addr = APP_START_ADDRESS,
		.reserved = 0U
};
