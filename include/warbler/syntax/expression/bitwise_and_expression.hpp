#ifndef WARBLER_SYNTAX_EXPRESSION_BITWISE_AND_HPP
#define WARBLER_SYNTAX_EXPRESSION_BITWISE_AND_HPP

// local headers
#include <warbler/syntax//expression/equality_expression.hpp>

namespace warbler::syntax
{
	class BitwiseAndExpression : public Expression
	{
	private:

		Ptr<Expression> _lhs;
		Array<Ptr<Expression>> _rhs;

	public:

		BitwiseAndExpression(Ptr<Expression>&& lhs, Array<Ptr<Expression>>&& rhs);

		static Result<Ptr<Expression>> parse(lexicon::TokenIterator& iter);

		bool validate(semantics::ModuleContext& mod_ctx, semantics::FunctionContext& func_ctx);
		void print_tree(u32 depth = 0) const;
		Type *get_type();
		const source::Location& location() const;
	};
}
#endif
