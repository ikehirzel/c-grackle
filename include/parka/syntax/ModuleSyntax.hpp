#ifndef PARKA_SYNTAX_MODULE_SYNTAX_HPP
#define PARKA_SYNTAX_MODULE_SYNTAX_HPP

#include "parka/syntax/EntitySyntax.hpp"
#include "parka/syntax/FunctionSyntax.hpp"
#include "parka/syntax/StructSyntax.hpp"
#include "parka/util/Array.hpp"
#include "parka/file/File.hpp"
#include "parka/util/Optional.hpp"
#include "parka/util/Table.hpp"

namespace parka
{
	class ModuleSyntax
	{
		String _filepath;
		Array<const FunctionSyntax*> _functions;
		Array<const StructSyntax*> _structs;

		ModuleSyntax(String&& filepath, Array<const FunctionSyntax*>&& functions, Array<const StructSyntax*>&& structs) :
		_filepath(std::move(filepath)),
		_functions(std::move(functions)),
		_structs(std::move(structs))
		{}

	public:

		ModuleSyntax(ModuleSyntax&&) = default;
		ModuleSyntax(const ModuleSyntax&) = delete;

		static ModuleSyntax parse(const File& file);

		const auto& filename() const { return _filepath; }
		const auto& functions() const { return _functions; }
		const auto& structs() const { return _structs; }
	};
}

#endif