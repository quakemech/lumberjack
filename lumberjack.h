/* SPDX-License-Identifier: Apache-2.0 */
/**
 * @file 		log.h 
 *
 * @brief 		Header file for logging library
 *
 * @copyright   Copyright (C) 2024 Barrett Edwards. All rights reserved.
 *       
 * @date        Aug 2024
 * @author      Barrett Edwards <thequakemech@gmail.com>
 *
 * Log levels for reference:
 * LOG_EMERG	0	 system is unusable 
 * LOG_ALERT	1	 action must be taken immediately
 * LOG_CRIT	    2	 critical conditions
 * LOG_ERR		3	 error conditions
 * LOG_WARNING	4	 warning conditions
 * LOG_NOTICE	5	 normal but significant condition
 * LOG_INFO	    6	 informational
 * LOG_DEBUG	7	 debug-level messages
 */

#ifndef _LOG_H
#define _LOG_H

/* INCLUDES ==================================================================*/

/* FILE *
 */
#include <stdio.h>

/* LOG_* Macros 
 */
#include <syslog.h>

/* MACROS ====================================================================*/

/* Parent object debug submit macros  */
#define dbg(ctx, arg...) 			log_dbg(ctx->log, ## arg)
#define info(ctx, arg...) 			log_info(ctx->log, ## arg)
#define notice(ctx, arg...) 		log_notice(ctx->log, ## arg)
#define warn(ctx, arg...) 			log_warn(ctx->log, ## arg)
#define err(ctx, arg...) 			log_err(ctx->log, ## arg)

/* Log object debug submit macros */
#ifdef ENABLE_LOGGING
#  define log_dbg(ctx, arg...)  	do { if ((*((char*)ctx)) >= LOG_DEBUG) 		log_submit(ctx, LOG_DEBUG,		__FUNCTION__, __LINE__, ## arg); } while(0)
#  define log_info(ctx, arg...) 	do { if ((*((char*)ctx)) >= LOG_INFO)  		log_submit(ctx, LOG_INFO, 		__FUNCTION__, __LINE__, ## arg); } while(0)
#  define log_notice(ctx, arg...)	do { if ((*((char*)ctx)) >= LOG_NOTICE) 	log_submit(ctx, LOG_NOTICE, 	__FUNCTION__, __LINE__, ## arg); } while(0) 
#  define log_warn(ctx, arg...) 	do { if ((*((char*)ctx)) >= LOG_WARNING)  	log_submit(ctx, LOG_WARNING,	__FUNCTION__, __LINE__, ## arg); } while(0)
#  define log_err(ctx, arg...)  	do { if ((*((char*)ctx)) >= LOG_ERR)   		log_submit(ctx, LOG_ERR, 		__FUNCTION__, __LINE__, ## arg); } while(0)
#else
#  define log_dbg(ctx, arg...)   	{}
#  define log_info(ctx, arg...)  	{}
#  define log_notice(ctx, arg...)   {}
#  define log_warn(ctx, arg...)  	{}
#  define log_err(ctx, arg...)   	{}
#endif

/* ENUMERATIONS ==============================================================*/

/**
 * Enumereation represeting log destinations to use 
 */
enum LDST
{
	LDST_STDERR = 0,  	//!< Log to stderr
	LDST_SYSLOG = 1, 	//!< Log to syslog 
	LDST_NULL 	= 2,	//!< Log to null 
	LDST_FILE 	= 3, 	//!< Log to a file 
	LDST_USER 	= 4, 	//!< Log to a user provided function
	LDST_MAX
};

/* STRUCTS ===================================================================*/

struct log_ctx;

/* TYPEDEFS ==================================================================*/

typedef void (*log_fn)(struct log_ctx *ctx, unsigned int priority, const char *fn, int ln, const char *format, va_list args);

/* GLOBAL VARIABLES ==========================================================*/

/* PROTOTYPES ================================================================*/

/* Library instantiation / deconstruction */
struct log_ctx *	log_init(char *owner, unsigned int dst, unsigned int priority, unsigned int timestamp, char *filepath, log_fn fn);
int 				log_free(struct log_ctx *ctx);

/* Get methods */
unsigned int 		log_get_dst(struct log_ctx *ctx);
const char * 		log_get_filename(struct log_ctx *ctx);
const char * 		log_get_owner(struct log_ctx *ctx);
unsigned int 		log_get_priority(struct log_ctx *ctx);
unsigned int 		log_get_use_timestamp(struct log_ctx *ctx);

/* Set methods */
int 				log_set_dst(struct log_ctx *ctx, unsigned int dst, char *filepath, log_fn fn);
void 				log_set_owner(struct log_ctx *ctx, char *owner);
int  				log_set_priority(struct log_ctx *ctx, unsigned int priority);
void 				log_set_use_timestamp(struct log_ctx *ctx, unsigned int use_timestamp);

/* To string methods */
const char *		log_str_priority(unsigned int priority);
const char *		log_str_dst(unsigned int dst);

/* Log submit functions */
void 				log_submit(struct log_ctx *ctx, unsigned int priority, const char *fn, int ln, const char *format, ...);
void 				log_submit_to_file(struct log_ctx *ctx, unsigned int priority, const char *fn, int ln, const char *format, va_list args);
void 				log_submit_to_null(struct log_ctx *ctx, unsigned int priority, const char *fn, int ln, const char *format, va_list args);
void 				log_submit_to_stderr(struct log_ctx *ctx, unsigned int priority, const char *fn, int ln, const char *format, va_list args);
void 				log_submit_to_syslog(struct log_ctx *ctx, unsigned int priority, const char *fn, int ln, const char *format, va_list args);

#endif //ifndef _LOG_H

