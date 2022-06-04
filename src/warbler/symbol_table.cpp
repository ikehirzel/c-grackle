#include <warbler/symbol_table.hpp>
#include <warbler/context.hpp>
#include <warbler/util/array.hpp>
#include <warbler/syntax.hpp>
#include <warbler/util/print.hpp>

namespace warbler
{
	// SymbolData::SymbolData(SymbolType type) :
	// _type(type)
	// {
	// 	switch (_type)
	// 	{
	// 		case SymbolType::Function:
	// 			new (&_function) Box<FunctionContext>();
	// 			break;

	// 		case SymbolType::Parameter:
	// 			new (&_parameter) Box<ParameterContext>();
	// 			break;

	// 		case SymbolType::Variable:
	// 			new (&_variable) Box<VariableContext>();
	// 			break;

	// 		case SymbolType::TypeDefinition:
	// 			new (&_type_definition) Box<TypeDefinitionContext>();				
	// 			break;
	// 	}
	// }

	// SymbolData::SymbolData(SymbolData&& other) :
	// _type(other._type)
	// {
	// 	switch (_type)
	// 	{
	// 		case SymbolType::Function:
	// 			new (&_function) auto(std::move(other._function));
	// 			break;

	// 		case SymbolType::Parameter:
	// 			new (&_parameter) auto(std::move(other._parameter));
	// 			break;

	// 		case SymbolType::Variable:
	// 			new (&_variable) auto(std::move(other._variable));
	// 			break;

	// 		case SymbolType::TypeDefinition:
	// 			new (&_type_definition) auto(std::move(other._type_definition));
	// 			break;
	// 	}
	// }

	// SymbolData::~SymbolData()
	// {
	// 	switch (_type)
	// 	{
	// 		case SymbolType::Function:
	// 			_function.~Box();
	// 			break;

	// 		case SymbolType::Parameter:
	// 			_parameter.~Box();
	// 			break;

	// 		case SymbolType::Variable:
	// 			_variable.~Box();
	// 			break;

	// 		case SymbolType::TypeDefinition:
	// 			_type_definition.~Box();
	// 			break;
	// 	}
	// }

	static SymbolData *get_symbol(Table<SymbolData>& symbols, const String& symbol)
	{
		auto iter = symbols.find(symbol);

		return iter == symbols.end()
			? nullptr
			: &iter->second;
	}

	String get_symbol_type_name(SymbolType type)
	{
		switch (type)
		{
			case SymbolType::Package:
				return "package";
			case SymbolType::Struct:
				return "type definition";
			case SymbolType::Primitive:
				return "primitive";
			case SymbolType::Function:
				return "function";
			case SymbolType::Parameter:
				return "parameter";
			case SymbolType::Variable:
				return "variable";
		}

		return "symbol";
	}

	static bool add_type_symbol(Table<SymbolData>& symbols, const StructSyntax& syntax, const String& current_scope)
	{
		auto symbol = current_scope + "::" + syntax.name().text();
		auto previous_declaration = get_symbol(symbols, symbol);

		if (previous_declaration != nullptr)
		{
			// TODO: improve type name specification in error, and show location of previous declaration
			auto message = "Symbol '" + symbol + "' is already defined";
			print_error(syntax.name(), message);
			return false;
		}

		symbols.emplace(symbol, SymbolData(syntax));

		return true;
	}

	static bool add_package_symbols(Table<SymbolData>& symbols, const PackageSyntax& syntax)
	{
		symbols.emplace(syntax.name(), SymbolData(syntax));

		auto success = true;

		for (const auto& struct_def : syntax.structs())
		{
			if (!add_type_symbol(symbols, struct_def, syntax.name()))
				success = false;
		}

		return success;
	}

	bool SymbolTable::add_symbols(const ProgramSyntax& syntax)
	{
		bool success = true;

		for (const auto& package : syntax.packages())
		{
			if (!add_package_symbols(_symbols, package))
				success = false;
		}

		return success;
	}

	SymbolTable::SymbolTable(Array<StructContext>& structs, Array<PrimitiveContext>& primitives):
	_symbols(),
	_current_scope(),
	_structs(structs),
	_primitives(primitives)
	{
		_primitives = Array<PrimitiveContext>({
			PrimitiveContext("u8", 1),
			PrimitiveContext("u16", 2),
			PrimitiveContext("u32", 4),
			PrimitiveContext("u64", 8),
		});

		auto index = 0;

		for (const auto& primitive : _primitives)
		{
			_symbols.emplace(primitive.symbol(), SymbolData(index, SymbolType::Primitive));
			index += 1;
		}
	
		index = 0;

		for (const auto& type : _structs)
		{
			_symbols.emplace(type.symbol(), SymbolData(index, SymbolType::Struct));
			index += 1;
		}
	}

	void SymbolTable::add_symbol(const String& symbol, const SymbolData& data)
	{
		assert(_symbols.find(symbol) == _symbols.end());

		_symbols.emplace(symbol, data);
	}

	static Array<String> get_packages_from_scope(const String& scope)
	{
		// TODO: optimize this and make it safe as this is very breakable if there is not two colons
		String temp;

		temp.reserve(256);

		Array<String> packages;

		for (usize i = 0; i < scope.size(); ++i)
		{
			if (scope[i] == ':')
			{
				i += 1;

				if (!temp.empty())
				{
					packages.push_back(temp);
					temp.clear();
				}

				continue;
			}
			
			temp += scope[i];
		}

		return packages;
	}

	static String create_symbol(const Array<String>& packages, usize count, const String& identifier)
	{
		String symbol;

		symbol.reserve(128);

		for (usize i = 0; i < count; ++i)
		{
			symbol += packages[i];
			symbol += "::";
		}

		symbol += identifier;

		return symbol;
	}

	void SymbolTable::set_scope_from_symbol(const String& symbol)
	{
		assert(!symbol.empty());

		_current_scope.clear();

		String tmp;

		tmp.reserve(64);

		// split symbol by '::'
		for (usize i = 0; i < symbol.size(); ++i)
		{
			if (symbol[i] == ':')
			{
				assert(symbol[i + 1] == ':');
				_current_scope.push_back(tmp);
				tmp.clear();
				i += 1;
				continue;
			}

			tmp += symbol[i];
		}
	}

	// TODO: make this safe
	String SymbolTable::get_symbol(const String& identifier)
	{
		return create_symbol(_current_scope, _current_scope.size(), identifier);
	}

	Result<SymbolResolution> SymbolTable::resolve(const String& identifier)
	{
		for (i32 i = (i32)_current_scope.size(); i >= 0; --i)
		{
			auto symbol = create_symbol(_current_scope, i, identifier);
			auto iter = _symbols.find(symbol);

			if (iter != _symbols.end())
				return SymbolResolution(iter->second, symbol);
		}

		return {};
	}

	// usize SymbolTable::validate_function(FunctionContext&& function)
	// {
	// 	auto index = _context.functions.size();

	// 	_context.functions.emplace_back(std::move(function));

	// 	return index;
	// }

	usize SymbolTable::add_validated_type(StructContext&& type)
	{
		auto index = _structs.size();
		
		_structs.emplace_back(std::move(type));

		return index;
	}

	// usize SymbolTable::validate_parameter(ParameterContext&& parameter)
	// {
	// }

	// usize SymbolTable::validate_variable(VariableContext&& variable)
	// {

	// }
}
