/*
 * boot_memory_map.h
 *
 *  Created on: Mar 24, 2026
 *      Author: can.yildirim
 */

#ifndef INC_BOOT_MEMORY_MAP_H_
#define INC_BOOT_MEMORY_MAP_H_

#define FLASH_BASE_ADDRESS         	(0x08000000UL)		// Flash base address
#define FLASH_TOTAL_SIZE_BYTES     	(128UL * 1024UL)	// Total flash size

#define BOOTLOADER_SIZE_BYTES      	(32UL * 1024UL)		// Total bootloader size

#define APP_START_ADDRESS          	(FLASH_BASE_ADDRESS + BOOTLOADER_SIZE_BYTES)	// Application start address
#define APP_PARTITION_SIZE_BYTES   	(96UL * 1024UL)		// Total application size

#define APP_METADATA_SIZE_BYTES		(256UL)				// Total metadata size


#define APP_METADATA_ADDRESS		(APP_START_ADDRESS + APP_PARTITION_SIZE_BYTES - APP_METADATA_SIZE_BYTES)
#define APP_MAX_SIZE_BYTES			(APP_PARTITION_SIZE_BYTES - APP_METADATA_SIZE_BYTES)

#endif /* INC_BOOT_MEMORY_MAP_H_ */
