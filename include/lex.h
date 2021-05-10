#ifndef LEX_H
#define LEX_H

// standard library
#include <stddef.h>

// local headers
#include <grackle/defs.h> 

typedef struct Token
{
	StringView str;
	char type;
}
Token;




enum token_type {
	ERROR,
	ASSIGN
};

// Lexer functions
void lex_init();
TokenList *lex(const char *src);

// Token functions
Token token_create(const char *str, unsigned short len, char type);

#endif
