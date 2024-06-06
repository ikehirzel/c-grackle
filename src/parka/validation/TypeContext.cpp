#include "parka/validation/TypeContext.hpp"


namespace parka::validation
{
	ir::ConversionIr* TypeContext::getConversionTo(const ir::TypeIr&) const
	{
		return nullptr;
	}

	ir::BinaryOperatorIr* TypeContext::getBinaryOperator(BinaryExpressionType, const ir::TypeIr&) const
	{
		return nullptr;
	}

	ir::AssignmentOperatorIr* TypeContext::getAssignmentOperator(AssignmentType, const ir::TypeIr&) const
	{
		return nullptr;
	}

	// static bool isCompatible(const Array<ir::ExpressionIr*>& arguments, const ir::CallOperatorIr& op, GlobalContext& globalContext)
	// {
	// 	const auto& parameters = op.prototype().parameters();
		
	// 	if (arguments.length() != parameters.length())
	// 		return false;

	// 	for (usize i = 0; i < parameters.length(); ++i)
	// 	{
	// 		auto& parameter = *parameters[i];
	// 		auto& argument = *arguments[i];
	// 		auto& typeContext = globalContext.getTypeContext(argument.type());
	// 		auto* conversion = typeContext.getConversionTo(parameter.type());

	// 		if (!conversion)
	// 			return false;
	// 	}

	// 	return true;
	// }
}
