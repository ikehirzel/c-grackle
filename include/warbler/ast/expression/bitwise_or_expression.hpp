#ifndef WARBLER_AST_EXPRESSION_BITWISE_OR_HPP
#define WARBLER_AST_EXPRESSION_BITWISE_OR_HPP

// local headers
#include <warbler/ast/expression/bitwise_xor_expression.hpp>

namespace warbler::ast
{
	class BitwiseOrExpression : public Expression
	{
	private:

		Ptr<Expression> _lhs;
		Array<Ptr<Expression>> _rhs;	

	public:

		BitwiseOrExpression(Ptr<Expression>&& lhs, Array<Ptr<Expression>>&& rhs);

		static Result<Ptr<Expression>> parse(TokenIterator& iter);

		bool validate(semantics::ModuleContext& mod_ctx, semantics::FunctionContext& func_ctx);
		Type *get_type();
		const Location& location() const;
		void print_tree(u32 depth = 0) const;
	};
}

#endif
