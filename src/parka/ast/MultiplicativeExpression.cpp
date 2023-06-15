#include "parka/ast/MultiplicativeExpression.hpp"
#include "parka/log/Log.hpp"
#include "parka/ast/PrefixExpression.hpp"

namespace parka
{
	static Optional<MultiplicativeType> getMultiplicativeType(Token& token)
	{
		switch (token.type())
		{
			case TokenType::Modulus:
				return MultiplicativeType::Modulus;

			case TokenType::Asterisk:
				return MultiplicativeType::Multiply;

			case TokenType::Slash:
				return MultiplicativeType::Divide;

			default:
				return {};
		}
	}

	ExpressionSyntax *MultiplicativeExpressionSyntax::parse(Token& token)
	{
		auto *lhs = PrefixExpressionSyntax::parse(token);

		if (!lhs)
			return {};
		
		auto type = getMultiplicativeType(token);

		while (type)
		{
			token.increment();
			
			auto rhs = PrefixExpressionSyntax::parse(token);

			if (!rhs)
				return {};

			lhs = new MultiplicativeExpressionSyntax(*lhs, *rhs, *type);
			type = getMultiplicativeType(token);
		}

		return lhs;
	}

	ExpressionContext *MultiplicativeExpressionSyntax::validate(SymbolTable&)
	{
		log::notImplemented(here());
	}
}