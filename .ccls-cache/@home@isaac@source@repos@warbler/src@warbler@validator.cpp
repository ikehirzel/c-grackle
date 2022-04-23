#include <stdexcept>
#include <warbler/validator.hpp>

#include <warbler/util/print.hpp>

namespace warbler
{
	// Result<ParameterContext> validate_parameter(const ParameterSyntax& syntax)
	// {
	// 	auto *previous_declaration = func_ctx.get_parameter(_name.text());

	// 	if (previous_declaration)
	// 	{
	// 		print_error(_name.token(), "parameter '" + _name.text() + "' is previously declared in function '" + func_ctx.name + "'");
	// 		print_note(previous_declaration->name().token(), "previous declaration is here");
	// 		return false;
	// 	}

	// 	func_ctx.parameters[_name.text()] = this;
	// 	_type.validate(mod_ctx);

	// 	return true;
	// }

	// Result<VariableContext> validate_variable(const VariableSyntax& syntax)
	// {
	// 	auto *previous_declaration = func_ctx.current_block().get_variable(_name.text());

	// 	if (previous_declaration)
	// 	{
	// 		print_error(_name.token(), "'" + _name.text() + "' is previously declared as a variable in scope");
	// 		print_note(previous_declaration->name().token(), "previous declaration here");
	// 		return false;
	// 	}

	// 	auto is_base_scope = func_ctx.blocks.size() == 1;

	// 	if (is_base_scope)
	// 	{
	// 		previous_declaration = func_ctx.get_parameter(_name.text());

	// 		if (previous_declaration)
	// 		{
	// 			print_error(_name.token(), "'" + _name.text() + "' is previously declared as a parameter in function '" + func_ctx.name + "'");
	// 			print_note(previous_declaration->name().token(), "previous declaration here");
	// 			return false;
	// 		}
	// 	}

	// 	func_ctx.current_block().variables[_name.text()] = this;
	// 	_type.validate(mod_ctx);

	// 	return true;
	// }

	Result<TypeDefinitionContext> validate_type_definition(const TypeDefinitionSyntax& syntax, const SymbolTable& symbol_table)
	{
		throw not_implemented();
	}

	Result<ModuleContext> validate_module(const ModuleSyntax& syntax)
	{
		#pragma message "TODO: implement module names"

		ModuleContext context;

		context.package = "test";

		SymbolTable symbol_table(context.symbols, context.package);
		u32 i = 0;

		// inserting type symbols
		for (const auto& type : syntax.types())
		{
			auto name = type.name().token().text();

			context.symbols.emplace(name, SymbolContext(SymbolType::TypeDefinition, i, false));

			i += 1;
		}

		// validate types
		for (const auto& type : syntax.types())
		{
			auto res = validate_type_definition(type, symbol_table);

			if (!res)
				return {};			

			auto name = type.name().token().text();
			auto &symbol = context.symbols.at(name);
			symbol.is_validated = true;
		}

		return context;
	}

	Result<AstContext> validate(const AstSyntax& syntax)
	{

		auto res = validate_module(syntax.module());

		if (!res)
			return {};

		return AstContext { res.unwrap() };
	}
}
