#ifndef PARKA_AST_VARIABLE_HPP
#define PARKA_AST_VARIABLE_HPP

#include "parka/ast/Expression.hpp"
#include "parka/ast/Identifier.hpp"
#include "parka/ir/Expression.hpp"
#include "parka/ir/Variable.hpp"
#include "parka/symbol/SymbolTable.hpp"
#include "parka/ast/TypeAnnotation.hpp"
#include "parka/ast/Entity.hpp"

namespace parka::ast
{
	class VariableAst : public EntityAst
	{
		Snippet _snippet;
		Identifier _identifier;
		Optional<TypeAnnotationAst> _annotation;
		ir::VariableIr *_context;
		bool _isMutable;

	public:

		VariableAst(const Snippet& snippet, Identifier&& identifier, bool isMutable, Optional<TypeAnnotationAst> annotation) :
		_snippet(snippet),
		_identifier(std::move(identifier)),
		_annotation(std::move(annotation)),
		_isMutable(isMutable)
		{}
		VariableAst(VariableAst&&) = default;
		VariableAst(const VariableAst&) = delete;

		static VariableAst *parse(Token& token);
		ir::VariableIr *validate(SymbolTable& symbolTable, ir::ExpressionIr *value);
		ir::EntityIr *context() { return _context; }
		String getSymbol() const { return _identifier.text(); }

		EntityType entityType() const { return EntityType::Variable; }
		const Snippet& snippet() const { return _snippet; }
		const String& name() const { return _identifier.text(); }
		const Identifier& identifier() const { return _identifier; }
		const auto& isExplicitlyTyped() const { return _annotation.hasValue(); }
		const auto& annotation() const { return *_annotation; }
		const auto& isMutable() const { return _isMutable; }
	};

	
}

#endif
