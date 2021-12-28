#include <warbler/ast/expression/postfix.h>

// standard headers
#include <assert.h>
#include <stdlib.h>

void postfix_expression_init(PostfixExpression *self)
{
	self->type = POSTFIX_NONE;
	self->primary = NULL;
}

void postfix_expression_free(PostfixExpression *self)
{
	if (self->type == POSTFIX_NONE)
	{
		if (self->primary)
			primary_expression_free(self->primary);

		free(self->primary);
	}
	else
	{
		if (self->postfix)
			postfix_expression_free(self->postfix);

		free(self->postfix);
	}
}

static inline Error try_postfix_expression_parse(PostfixExpression *self, TokenIterator *iter)
{
	Error error;

	switch (iter->token->type)
	{
		default:
			self->type = POSTFIX_NONE;
			self->primary = malloc(sizeof(PrimaryExpression));

			if (!self->primary)
				return ERROR_MEMORY;

			if ((error = primary_expression_parse(self->primary, iter)))
				return error;
			break;
	}

	return ERROR_NONE;
}

Error postfix_expression_parse(PostfixExpression *self, TokenIterator *iter)
{
	assert(self != NULL);
	assert(iter != NULL);

	postfix_expression_init(self);

	Error error = try_postfix_expression_parse(self, iter);

	if (error)
		postfix_expression_free(self);

	return error;
}
