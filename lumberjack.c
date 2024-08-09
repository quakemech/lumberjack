/* SPDX-License-Identifier: Apache-2.0 */
/**
 * @file 		log.c
 *
 * @brief 		Code file for logging library
 *
 * @copyright   Copyright (C) 2024 Barrett Edwards. All rights reserved.
 *       
 * @date        Aug 2024
 * @author      Barrett Edwards <thequakemech@gmail.com>
 */

/* INCLUDES ==================================================================*/

/* printf()
 * fprintf()
 * vfprintf()
 * FILE
 * stdout
 * stderr
 */
#include <stdio.h>

/* calloc() 
 * free()
 */
#include <stdlib.h>

/* getpid()
 */
#include <unistd.h>

/* strdup()
 */
#include <string.h>

/* clock_gettime()
 */
#include <time.h>

/* va_start 
 * va_end
 */
#include <stdarg.h>

/* LOG_* Macros 
 */
#include <syslog.h> 

#include "lumberjack.h"

/* MACROS ====================================================================*/

/* ENUMERATIONS ==============================================================*/

/* STRUCTS ===================================================================*/

struct __attribute__((__packed__)) log_ctx
{
	char priority;
	char dst;
	char use_timestamp; 
	char rsvd;
	char *owner;
	FILE *file;
	char *filename;
	log_fn log_fn;
};

/* GLOBAL VARIABLES ==========================================================*/

/* PROTOTYPES ================================================================*/

/* FUNCTIONS =================================================================*/

/**
 * Free the memory used by struct log_ctx
 */
int log_free(struct log_ctx *ctx)
{
	if (ctx->owner != NULL)
		free(ctx->owner);

	if (ctx->filename != NULL)
		free(ctx->filename);

	if (ctx->file != 0)
		fclose(ctx->file);

	if (ctx != NULL)
		free(ctx);

	return 0;
}

unsigned int log_get_dst(struct log_ctx *ctx)
{
	return ctx->dst;
}

const char *log_get_filename(struct log_ctx *ctx)
{
	return ctx->filename;
}

const char *log_get_owner(struct log_ctx *ctx)
{
	return ctx->owner;
}

unsigned int log_get_priority(struct log_ctx *ctx)
{
	return ctx->priority;
}

unsigned int log_get_use_timestamp(struct log_ctx *ctx)
{
	return ctx->use_timestamp;
}

struct log_ctx *log_init(char *owner, unsigned int dst, unsigned int priority, unsigned int timestamp, char *filepath, log_fn fn)
{
	struct log_ctx *l;

	// Allocate memory for the log struct 
	l = calloc(1, sizeof(*l));
	if (l == NULL)
		goto end;
	
	// Set values 
	if (owner != NULL)
		l->owner = strdup(owner);
	l->priority = priority;
	l->use_timestamp = timestamp;

	log_set_dst(l, dst, filepath, fn);

end:
	return l;
}

int	log_set_dst(struct log_ctx *ctx, unsigned int dst, char *filepath, log_fn fn)
{
	int rv = 1;
	FILE *fp;

	// Free filename if it was set before 
	if (ctx->filename != NULL)
	{
		free(ctx->filename);
		ctx->filename = NULL;
	}

	// Close file if it is open 
	if (ctx->file != 0)
	{
		fclose(ctx->file);
		ctx->file = 0;
	}

	switch(dst)
	{
		case LDST_SYSLOG:
			ctx->log_fn = log_submit_to_syslog;
			ctx->dst = LDST_SYSLOG;
			break;

		case LDST_NULL:
			ctx->log_fn = log_submit_to_null;
			ctx->dst = LDST_NULL;
			break;

		case LDST_FILE:

			fp = fopen(filepath, "a");
			if (fp > 0)
			{
				ctx->log_fn = log_submit_to_file;
				ctx->dst = LDST_FILE;
				ctx->file = fp;
				ctx->filename = strdup(filepath);
			}
			else 
			{
				ctx->log_fn = log_submit_to_stderr;
				ctx->dst = LDST_STDERR;
				rv = 1;
				goto end;
			}

			break;

		case LDST_USER:
			ctx->log_fn = fn;
			ctx->dst = LDST_USER;
			break;

		case LDST_STDERR:
		default:
			ctx->log_fn = log_submit_to_stderr;
			ctx->dst = LDST_STDERR;
			break;
	}

	rv = 0;

end:

	return rv;
}

void log_set_owner(struct log_ctx *ctx, char *owner)
{
	if (ctx->owner != NULL)
	{
		free(ctx->owner);
		ctx->owner = NULL;
	}
	
	if (owner != NULL)
		ctx->owner = strdup(owner);
}

int log_set_priority(struct log_ctx *ctx, unsigned int priority)
{
	int rv = 0;

	// Validate inputs 
	if (priority < 0)
	{
		priority = 0;
		rv = 1;
	}
	else if (priority > LOG_DEBUG)
	{
		priority = LOG_DEBUG;
		rv = 1;
	}

	ctx->priority = priority;

	return rv;
}

void log_set_use_timestamp(struct log_ctx *ctx, unsigned int use_timestamp)
{
	if (use_timestamp)
		ctx->use_timestamp = 1;
	else 
		ctx->use_timestamp = 0;
}

const char *log_str_priority(unsigned int priority)
{
	/**
	 * String representation of logging levels 
	 */
	const char *levels[] = 
	{
		"EMERG",
		"ALERT",
		"CRIT",
		"ERR",
		"WARNING",
		"NOTICE",
		"INFO",
		"DEBUG",
	};

	if (priority < 0 || priority > LOG_DEBUG)
		return NULL;

	return levels[priority];	
}

const char *log_str_dst(unsigned int dst)
{
	static const char *log_destinations[] =
	{
		"STDIO", 
		"SYSLOG", 
		"NULL",
		"FILE", 
	};
	
	if (dst > LDST_MAX)
		return NULL;

	return log_destinations[dst];
}

void log_submit(struct log_ctx *ctx, unsigned int priority, const char *fn, int ln, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	ctx->log_fn(ctx, priority, fn, ln, format, args);
	va_end(args);
}

void log_submit_to_file(struct log_ctx *ctx, unsigned int priority, const char *fn, int ln, const char *format, va_list args)
{
	struct timespec ts;
	FILE *f = ctx->file;

	if (ctx->use_timestamp)
	{
		// Get time for entry 
		clock_gettime(CLOCK_REALTIME, &ts);

		// print the time first 
		fprintf(f, "[%10ld.%09ld] [%d] %s - ", ts.tv_sec, ts.tv_nsec, getpid(), log_str_priority(priority));
		fprintf(f, "%s: %s:%d ", ctx->owner, fn, ln);
	}

	// print the message 
	vfprintf(f, format, args);

	// flush 
	fflush(f);
}

void log_submit_to_null(struct log_ctx *ctx, unsigned int priority, const char *fn, int ln, const char *format, va_list args)
{
}

void log_submit_to_stderr(struct log_ctx *ctx, unsigned int priority, const char *fn, int ln, const char *format, va_list args)
{
	struct timespec ts;
	FILE *fp;

	if (priority >= LOG_INFO)
		fp = stdout;
	else
		fp = stderr;

	if (ctx->use_timestamp)
	{
		// Get time for entry 
		clock_gettime(CLOCK_REALTIME, &ts);

		// print the time first 
		fprintf(fp, "[%10ld.%09ld] [%d] %s - ", ts.tv_sec, ts.tv_nsec, getpid(), log_str_priority(priority));
		fprintf(fp, "%s: %s:%d ", ctx->owner, fn, ln);
	}

	vfprintf(fp, format, args);
}

void log_submit_to_syslog(struct log_ctx *ctx, unsigned int priority, const char *fn, int ln, const char *format, va_list args)
{
	vsyslog(priority, format, args);
}

