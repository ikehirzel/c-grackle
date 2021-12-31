#ifndef WARBLER_ERROR_H
#define WARBLER_ERROR_H

// local headers
#include <warbler/token.h>

// standard headers
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

typedef enum Error
{
	ERROR_NONE = 0,
	ERROR_MEMORY,
	ERROR_FILE,
	ERROR_ARGUMENT,
	ERROR_PARSE,
	ERROR_NOT_FOUND,
	ERROR_INVALID_PTR,
	ERROR_NOT_IMPLEMENTED
} Error;

extern bool is_color_enabled;
extern bool is_printing_enabled;

void print_error(const char *msg);
void print_errorf(const char *fmt, ...);
void print_token_error(const char *msg, const Token *token);
void set_color_enabled(bool enabled);
void set_printing_enabled(bool enabled);

static inline Error _not_implemented_error(const char *func_name)
{
	print_errorf("%s is not implemented yet", func_name);
	return ERROR_NOT_IMPLEMENTED;
}

#define _try(expr) { Error error = expr; if (error) return error; }
#define not_implemented_error() _not_implemented_error(__func__);

#endif
