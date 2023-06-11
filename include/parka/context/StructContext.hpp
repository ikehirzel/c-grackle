#ifndef PARKA_CONTEXT_STRUCT_cONTEXT_HPP
#define PARKA_CONTEXT_STRUCT_cONTEXT_HPP

#include "parka/repository/EntityContext.hpp"
#include "parka/repository/EntityContextId.hpp"
#include "parka/symbol/StructSymbolTable.hpp"
#include "parka/util/Optional.hpp"

namespace parka
{
	class StructContext : public EntityContext
	{
	public:

		StructContext() = default;
		StructContext(StructContext&&) = default;
		StructContext(const StructContext&) = delete;

		static StructContext *validate(StructSymbolTable& symbolTable);
		
		EntityType type() const { return EntityType::Struct; }
	};
}

#endif