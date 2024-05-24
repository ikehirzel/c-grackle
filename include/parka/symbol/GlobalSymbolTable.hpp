#ifndef PARKA_SYMBOL_GLOBAL_SYMBOL_TABLE_HPP
#define PARKA_SYMBOL_GLOBAL_SYMBOL_TABLE_HPP

#include "parka/ast/FunctionAst.hpp"
#include "parka/ast/PackageAst.hpp"
#include "parka/ir/AssignmentOperatorIr.hpp"
#include "parka/ir/TypeIr.hpp"
#include "parka/symbol/AssignmentOperatorKey.hpp"
#include "parka/symbol/BinaryOperatorKey.hpp"
#include "parka/symbol/ConversionKey.hpp"
#include "parka/symbol/SymbolTable.hpp"
#include "parka/util/Array.hpp"
#include "parka/util/FlatMap.hpp"

namespace parka
{
	class GlobalSymbolTable: public SymbolTable
	{
		String _scope;
		FlatMap<String, Resolvable*> _symbols;
		FlatMap<BinaryOperatorKey, ir::BinaryOperatorIr*> _binaryOperators;
		FlatMap<ConversionKey, ir::ConversionIr*> _conversions;
		FlatMap<AssignmentOperatorKey, ir::AssignmentOperatorIr*> _assignmentOperators;
		Array<FunctionEntry> _functions;

	public:

		GlobalSymbolTable(const ast::PackageAst& globalPackage);
		GlobalSymbolTable(GlobalSymbolTable&&) = default;
		GlobalSymbolTable(const GlobalSymbolTable&) = delete;

		FunctionEntry& addFunction(FunctionEntry&& entry);

		FunctionEntry& declare(const ast::FunctionAst& ast);
		VariableEntry& declare(const ast::VariableAst& ast);
		ParameterEntry& declare(const ast::ParameterAst& ast);
		Resolvable* findSymbol(const ast::IdentifierAst& identifier);
		ir::LValueIr* resolveSymbol(const ast::QualifiedIdentifierAst& identifier);
		ir::BinaryOperatorIr* resolveBinaryOperator(BinaryExpressionType binaryExpressionType, const ir::TypeIr& left, const ir::TypeIr& right);
		Result<ir::ConversionIr*> resolveConversion(const ir::TypeIr& to, const ir::TypeIr& from);
		const String& scope() const { return _scope; }
		SymbolTable* parent() { return nullptr; }
		GlobalSymbolTable& globalSymbolTable() { return *this; }

		auto& functions() { return _functions; }
	};
}

#endif
