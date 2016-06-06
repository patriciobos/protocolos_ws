/*
 * includes.h
 *
 *  Created on: May 4, 2012
 *      Author: pablo
 */

#ifndef INCLUDES_H_
#define INCLUDES_H_

#ifdef __USE_CMSIS
#include "LPC13xx.h"
#endif

#include <cr_section_macros.h>
#include <NXP/crp.h>

#include <stdio.h>

#define USAR_PRINTF	0

typedef uint8_t u8;
typedef int8_t s8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;
typedef int32_t s32;
typedef int64_t s64;
typedef uint64_t u64;
typedef char ascii;





#include "ssp.h"
#include "cc2520.h"
#include "cc2520-mac.h"
#include "cc2520-task.h"
#include "ledpul.h"
#include <string.h>


#endif /* INCLUDES_H_ */
