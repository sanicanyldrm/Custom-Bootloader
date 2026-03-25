/*
 * boot_image_metadata.h
 *
 *  Created on: Mar 24, 2026
 *      Author: can.yildirim
 */

#ifndef INC_BOOT_IMAGE_METADATA_H_
#define INC_BOOT_IMAGE_METADATA_H_

#include <stdint.h>

#define BOOT_METADATA_MAGIC					(0x424F4F54UL)		// 'BOOT'
#define BOOT_METADATA_FORMAT_VERSION		(1U)

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

#endif /* INC_BOOT_IMAGE_METADATA_H_ */
