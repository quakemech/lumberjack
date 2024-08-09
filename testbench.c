/* SPDX-License-Identifier: Apache-2.0 */
/**       
 * @file        testbench.c
 *
 * @brief       testbench code for lumberjack logging library
 *   
 * @copyright   Copyright (C) 2024 Barrett Edwards. All rights reserved.
 *       
 * @date        Aug 2024
 * @author      Barrett Edwards <thequakemech@gmail.com>
 *          
 */

/* INCLUDES ==================================================================*/

/* printf()
 */
#include <stdio.h>

#include <unistd.h>
#include <fcntl.h> 

/* atoi()
 */
#include <stdlib.h>

/* strcmp()
 */
#include <string.h>

/* LOG_* Macros 
 */
#include <syslog.h>

#ifndef ENABLE_LOGGING
#define ENABLE_LOGGING
#endif
#include "lumberjack.h"

/* MACROS ====================================================================*/

/* ENUMERATIONS ==============================================================*/

/* STRUCTS ===================================================================*/

/* GLOBAL VARIABLES ==========================================================*/

/* PROTOTYPES ================================================================*/

/* FUNCTIONS =================================================================*/

void stdio_debug_timestamp()
{
	struct log_ctx *ctx; 
	
	printf("%s\n", __FUNCTION__);

	ctx = log_init("testbench", LDST_STDERR, LOG_DEBUG, 1, NULL, NULL);

	log_info(ctx, "Debug Level: Info message: %d\n", 5);
	log_dbg(ctx,  "Debug Level: Debug message: %d\n", 6);
	log_err(ctx,  "Debug Level: Error message: %d\n", 7);

	log_free(ctx);
}

void stdio_info_timestamp()
{
	struct log_ctx *ctx; 

	printf("%s\n", __FUNCTION__);

	ctx = log_init("testbench", LDST_STDERR, LOG_INFO, 1, NULL, NULL);

	log_info(ctx, "Info Level: Info message: %d\n", 5);
	log_dbg(ctx,  "Info Level: Debug message: %d\n", 6);
	log_err(ctx,  "Info Level: Error message: %d\n", 7);

	log_free(ctx);
}

void stdio_err_timestamp()
{
	struct log_ctx *ctx; 

	printf("%s\n", __FUNCTION__);

	ctx = log_init("testbench", LDST_STDERR, LOG_ERR, 1, NULL, NULL);

	log_info(ctx, "Err Level: Info message: %d\n", 5);
	log_dbg(ctx,  "Err Level: Debug message: %d\n", 6);
	log_err(ctx,  "Err Level: Error message: %d\n", 7);

	log_free(ctx);
}

void file_debug_timestamp()
{
	struct log_ctx *ctx; 

	printf("%s\n", __FUNCTION__);

	ctx = log_init("testbench", LDST_STDERR, LOG_DEBUG, 1, "test.log", NULL);

	log_info(ctx, "Debug Level: Info message: %d\n", 5);
	log_dbg(ctx,  "Debug Level: Debug message: %d\n", 6);
	log_err(ctx,  "Debug Level: Error message: %d\n", 7);

	log_free(ctx);
}


void syslog_debug_timestamp()
{
	struct log_ctx *ctx; 

	printf("%s\n", __FUNCTION__);

	ctx = log_init("testbench", LDST_SYSLOG, LOG_DEBUG, 0, NULL, NULL);

	log_info(ctx, "Debug Level: Info message: %d\n", 5);
	log_dbg(ctx,  "Debug Level: Debug message: %d\n", 6);
	log_err(ctx,  "Debug Level: Error message: %d\n", 7);

	log_free(ctx);
}

void null_debug_timestamp()
{
	struct log_ctx *ctx; 

	printf("%s\n", __FUNCTION__);

	ctx = log_init("testbench", LDST_NULL, LOG_DEBUG, 0, NULL, NULL);

	log_info(ctx, "Debug Level: Info message: %d\n", 5);
	log_dbg(ctx,  "Debug Level: Debug message: %d\n", 6);
	log_err(ctx,  "Debug Level: Error message: %d\n", 7);

	log_free(ctx);
}

void file_debug()
{
	struct log_ctx *ctx; 

	printf("%s\n", __FUNCTION__);

	ctx = log_init("testbench", LDST_FILE, LOG_DEBUG, 1, "test.log", NULL);

	log_info(ctx, "Debug Level: Info message: %d\n", 5);
	log_dbg(ctx,  "Debug Level: Debug message: %d\n", 6);
	log_err(ctx,  "Debug Level: Error message: %d\n", 7);

	log_free(ctx);
}

int main()
{
	stdio_debug_timestamp();
	stdio_info_timestamp();
	stdio_err_timestamp();
	syslog_debug_timestamp();
	file_debug_timestamp();
	null_debug_timestamp();
	file_debug();
	return 0;
}
