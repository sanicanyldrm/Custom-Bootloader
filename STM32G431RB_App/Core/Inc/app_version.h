/*
 * app_version.h
 *
 *  Created on: Mar 24, 2026
 *      Author: can.yildirim
 */

#ifndef INC_APP_VERSION_H_
#define INC_APP_VERSION_H_

#define APP_VERSION_MAJOR   (0U)		// Major 1.x.x
#define APP_VERSION_MINOR   (1U)		// Minor x.1.x
#define APP_VERSION_PATCH   (1U)		// Patch x.x.1


#define APP_IMAGE_VERSION	\
	((APP_VERSION_MAJOR << 16) | \
	 (APP_VERSION_MINOR << 8)  | \
	 (APP_VERSION_PATCH))

#endif /* INC_APP_VERSION_H_ */
