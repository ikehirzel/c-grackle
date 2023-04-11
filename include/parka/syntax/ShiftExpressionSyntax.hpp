#ifndef PARKA_SYNTAX_EXPRESSION_SHIFT_SYNTAX_HPP
#define PARKA_SYNTAX_EXPRESSION_SHIFT_SYNTAX_HPP

#include "parka/enum/ShiftType.hpp"
#include "parka/syntax/ExpressionSyntax.hpp"

namespace parka
{
	class ShiftExpressionSyntax : public ExpressionSyntax
	{
		ExpressionSyntaxId _lhs;
		ExpressionSyntaxId _rhs;
		ShiftType _type;

		ShiftExpressionSyntax(ExpressionSyntaxId&& lhs, ExpressionSyntaxId&& rhs, ShiftType type) :
		_lhs(std::move(lhs)),
		_rhs(std::move(rhs)),
		_type(type)
		{}

	public:

		ShiftExpressionSyntax(ShiftExpressionSyntax&&) = default;
		ShiftExpressionSyntax(const ShiftExpressionSyntax&) = delete;
		~ShiftExpressionSyntax() = default;

		static Optional<ExpressionSyntaxId> parse(Token& token);

		bool validate(const EntitySyntaxId& functionId);
		Optional<Type> getType() const;

		const auto& lhs() const { return _lhs; }
		const auto& rhs() const { return _rhs; }
	};
}

#endif
