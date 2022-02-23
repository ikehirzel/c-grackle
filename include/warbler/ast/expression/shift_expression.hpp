#ifndef WARBLER_AST_EXPRESSION_BITWISE_SHIFT_HPP
#define WARBLER_AST_EXPRESSION_BITWISE_SHIFT_HPP

// local headers
#include <warbler/ast/expression/additive_expression.hpp>

namespace warbler::ast
{
	enum ShiftType
	{
		SHIFT_LEFT,
		SHIFT_RIGHT
	};

	struct ShiftRhs
	{
		Ptr<Expression> expr;
		ShiftType type;
	};

	class ShiftExpression : public Expression
	{
	private:

		Ptr<Expression> _lhs;
		std::vector<ShiftRhs> _rhs;

	public:

		ShiftExpression(Ptr<Expression>&& lhs, std::vector<ShiftRhs>&& rhs);

		static Result<Ptr<Expression>> parse(TokenIterator& iter);

		bool validate(semantics::ModuleContext& mod_ctx, semantics::FunctionContext& func_ctx);
		void print_tree(u32 depth = 0) const;
		Type *get_type();
		const Location& location() const;
	};
}

#endif
