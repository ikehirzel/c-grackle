#include "parka/ast/BitwiseOrExpression.hpp"
#include "parka/log/Log.hpp"
#include "parka/ast/BitwiseXorExpression.hpp"

namespace parka
{
	ExpressionSyntax *BitwiseOrExpressionSyntax::parse(Token& token)
	{
		auto *lhs = BitwiseXorExpressionSyntax::parse(token);

		if (!lhs)
			return {};

		while (token.type() == TokenType::Pipe)
		{
			token.increment();

			auto rhs = BitwiseXorExpressionSyntax::parse(token);

			if (!rhs)
				return {};

			lhs = new BitwiseOrExpressionSyntax(*lhs, *rhs);
		}

		return lhs;
	}

	ExpressionContext *BitwiseOrExpressionSyntax::validate(SymbolTable&)
	{
		log::notImplemented(here());
	}
}
