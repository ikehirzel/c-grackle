#ifndef PARKA_SYNTAX_EXPRESSION_BOOLEAN_OR_SYNTAX_HPP
#define PARKA_SYNTAX_EXPRESSION_BOOLEAN_OR_SYNTAX_HPP

#include "parka/syntax/ExpressionSyntax.hpp"

namespace parka
{
	class BooleanOrExpressionSyntax : public ExpressionSyntax
	{
		ExpressionSyntax& _lhs;
		ExpressionSyntax& _rhs;

	public:

		BooleanOrExpressionSyntax(ExpressionSyntax& lhs, ExpressionSyntax& rhs) :
		_lhs(lhs),
		_rhs(rhs)
		{}
		BooleanOrExpressionSyntax(BooleanOrExpressionSyntax&&) = default;
		BooleanOrExpressionSyntax(const BooleanOrExpressionSyntax&) = delete;

		static ExpressionSyntax *parse(Token& token);

		ExpressionType expressionType() const { return ExpressionType::BooleanOr; }
		const auto& lhs() const { return _lhs; }
		const auto& rhs() const { return _rhs; }
	};
}

#endif
