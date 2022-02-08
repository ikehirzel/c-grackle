#ifndef WARBLER_AST_TYPE_DEFINITION_HPP
#define WARBLER_AST_TYPE_DEFINITION_HPP

// local headers
#include <warbler/ast/identifier.hpp>
#include <warbler/ast/type/struct.hpp>
#include <warbler/ast/type/enum.hpp>
#include <warbler/ast/type/type_definition_body.hpp>

namespace warbler::semantics
{
	class Context;
}

namespace warbler::ast
{
	enum TypeDefinitionType
	{
		DEFINITION_STRUCT,
		DEFINITION_ENUM
	};

	class TypeDefinition
	{
	private:

		Identifier _name;
		String _symbol;
		TypeDefinitionBody *_body;

	public:

		TypeDefinition(Identifier&& name, TypeDefinitionBody *body);
		TypeDefinition(TypeDefinition&&);
		TypeDefinition(const TypeDefinition&) = delete;
		~TypeDefinition();

		static Result<TypeDefinition> parse(TokenIterator& iter);

		bool validate(semantics::Context& context);
		const String& generate_symbol(semantics::Context& contetxt);
		void print_tree(u32 depth = 0) const;

		const Identifier& name() const { return _name; }
		const String& symbol() const { return _symbol; }

		TypeDefinition& operator=(TypeDefinition&& other);
		TypeDefinition& operator=(const TypeDefinition& other) = delete;
	};
}

#endif
