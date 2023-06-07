#include "parka/symbol/PackageSymbolTable.hpp"
#include "parka/intrinsic/Primitive.hpp"
#include "parka/log/Indent.hpp"
#include "parka/log/Log.hpp"
#include "parka/symbol/FunctionSymbolTable.hpp"
#include "parka/symbol/Identifier.hpp"
#include "parka/syntax/PackageSyntax.hpp"
#include "parka/repository/EntitySyntaxId.hpp"
#include "parka/symbol/SymbolTableEntry.hpp"
#include "parka/util/Print.hpp"
#include "parka/util/String.hpp"

namespace parka
{
	PackageSymbolTable::PackageSymbolTable(const EntitySyntaxId& packageId, const SymbolTable *parent) :
	_packageId(packageId),
	_symbols(), // TODO: pre-reserve symbol count
	_parent(parent)
	{
		const auto isGlobalPackage = parent == nullptr;

		if (isGlobalPackage)
		{
			for (const auto& primitive : Primitive::primitives)
			{
				const auto& id = EntitySyntaxId::getFor(primitive);
				_symbols.insert(primitive.identifier(), SymbolTableEntry(id, *this));
			}
		}

		const auto& syntax = packageId.getPackage();

		for (const auto& mod : syntax.modules())
		{
			for (const auto& structId : mod.structIds())
				declare(structId);

			for (const auto& functionId : mod.functionIds())
				declare(functionId);
		}

		for (const auto& packageId : syntax.packageIds())
			declare(packageId);
	}

	bool PackageSymbolTable::declare(const EntitySyntaxId& entityId)
	{
		// TODO: Invalidate symbol on failure, better error
		auto entry = SymbolTableEntry(entityId, *this);
		const auto& identifier = entityId->identifier();
		auto result = _symbols.insert(identifier, std::move(entry));

		if (!result)
			log::error("Name `$` is already declared in this package.", identifier);

		return result;
	}

	const SymbolTableEntry *PackageSymbolTable::findEntry(const QualifiedIdentifier& identifier, usize index) const
	{
		auto& part = identifier[index];
		auto result = resolve(part);

		if (!result)
		{
			if (_parent)
				result = _parent->resolve(part);

			if (!result)
			{
				auto& package = *_packageId;
				// TODO: Output package symbol, entity type and reference highlight
				log::error("Unable to find `$` in package `$`.", part.text(), package.identifier());
				
				return nullptr;
			}
		}

		return nullptr;
	}

	Optional<EntitySyntaxId> PackageSymbolTable::resolve(const Identifier& identifier) const
	{
		// TODO: Confirm this makes sense. I'm not sure if resolving single identifiers should always do
		// this or if it should seek upwards at times
		const auto *result = _symbols.find(identifier.text());

		if (!result)
			return {};
			
		return result->syntaxId();
	}

	Optional<EntitySyntaxId> PackageSymbolTable::resolve(const QualifiedIdentifier& identifier) const
	{
		// TODO: Make this logic work
		const auto *entry = findEntry(identifier, 0);

		if (!entry)
			return {};

		return entry->syntaxId();
	}

	std::ostream& operator<<(std::ostream& out, const PackageSymbolTable& symbols)
	{
		// TODO: Implement printing other packages
		auto indent = Indent(out);
		const auto& identifier = symbols._packageId->identifier();

		out << (identifier.length() > 0 ? identifier : "Global Scope") << ":\n";

		for (const auto& entry : symbols._symbols)
		{
			out << indent << entry.value();
		}

		return out;
	}
}
