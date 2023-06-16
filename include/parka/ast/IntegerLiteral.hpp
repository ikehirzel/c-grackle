#ifndef PARKA_AST_EXPRESSION_LITERAL_INTEGER_HPP
#define PARKA_AST_EXPRESSION_LITERAL_INTEGER_HPP

#include "parka/ast/Expression.hpp"
#include "parka/util/Common.hpp"

namespace parka::ast
{
	class IntegerLiteralAst : public ExpressionAst
	{
		Snippet _snippet;

	public:

		IntegerLiteralAst(const Snippet& snippet) :
		_snippet(snippet)
		{}
		IntegerLiteralAst(IntegerLiteralAst&&) = default;
		IntegerLiteralAst(const IntegerLiteralAst&) = delete;

		static ExpressionAst *parse(Token& token);
		ir::ExpressionIr *validate(SymbolTable& symbolTable);

		ExpressionType expressionType() const { return ExpressionType::IntegerLiteral; }
		const Snippet& snippet() const { return _snippet; }
	};
}

#endif
