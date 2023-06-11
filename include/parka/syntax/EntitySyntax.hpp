#ifndef PARKA_SYNTAX_ENTITY_SYNTAX_HPP
#define PARKA_SYNTAX_ENTITY_SYNTAX_HPP

#include "parka/repository/EntityContext.hpp"
#include "parka/repository/EntitySyntaxId.hpp"
#include "parka/symbol/SymbolTable.hpp"
#include "parka/util/String.hpp"

namespace parka
{
	struct EntitySyntax
	{
		virtual ~EntitySyntax() {}
		virtual const String& identifier() const = 0;
		virtual EntityType entityType() const = 0;

		static EntitySyntax& get(const EntitySyntaxId& id);
	};
}

#endif
