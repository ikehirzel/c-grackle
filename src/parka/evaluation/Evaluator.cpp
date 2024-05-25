#include "parka/evaluation/Evaluator.hpp"
#include "parka/enum/AssignmentType.hpp"
#include "parka/enum/JumpType.hpp"
#include "parka/evaluation/IntrinsicConversion.hpp"
#include "parka/evaluation/IntrinsicBinaryOperator.hpp"
#include "parka/ir/AssignmentStatementIr.hpp"
#include "parka/ir/BoolLiteralIr.hpp"
#include "parka/ir/CastExpressionIr.hpp"
#include "parka/ir/ConditionalExpressionIr.hpp"
#include "parka/ir/DeclarationStatementIr.hpp"
#include "parka/ir/ExpressionStatementIr.hpp"
#include "parka/ir/FloatLiteralIr.hpp"
#include "parka/ir/IfStatementIr.hpp"
#include "parka/ir/ReturnStatementIr.hpp"
#include "parka/ir/StringLiteralIr.hpp"
#include "parka/log/Log.hpp"
#include <stdexcept>

using namespace parka::ir;

namespace parka::evaluation
{
	void evaluate(const Ir& ir)
	{
		auto globalState = GlobalState();
		auto state = LocalState(globalState);
		auto *entryPoint = ir.entryPoint();

		if (!entryPoint)
			log::fatal("Unable to evaluate `$` as there is no entry point. Please implement a function named `main` in the global scope.", ir.name());

		auto& result = evaluateFunction(*entryPoint, {}, state);

		log::debug("Result of $(): $", entryPoint->symbol(), result);
	}

	Value& evaluateFunction(const FunctionIr& ir, const Array<ArgumentIr>& arguments, LocalState& state)
	{

		usize previousReturnValueIndex = state.getReturnValueIndex();
		auto& returnValue = state.pushReturnValue(ir.prototype().returnType());
		usize index = state.getScopeIndex();

		evaluatePrototype(ir.prototype(), arguments, state);

		if (!ir.hasBody())
		{
			log::fatal("$() is not implemented.", ir.symbol());
		}
		
		evaluateFunctionBody(ir.body(), state);

		// TODO: This shouldn't do this for exceptions

		state.setReturning(JumpType::None);
		state.clearScopeValues(index);
		state.setReturnValueIndex(previousReturnValueIndex);

		return returnValue;
	}

	void evaluatePrototype(const PrototypeIr& ir, const Array<ArgumentIr>& arguments, LocalState& state)
	{
		for (usize i = 0; i < arguments.length(); ++i)
		{
			auto& parameter = *ir.parameters()[i];
			auto& argument = arguments[i];
			auto& argumentValue = evaluateExpression(argument.value(), state);

			argumentValue.setNode(parameter);
		}
	}

	void evaluateFunctionBody(const ir::FunctionBodyIr& ir, LocalState& state)
	{
		if (ir.isExpression())
		{
			auto& expressionValue = evaluateExpression(ir.expression(), state);

			state.returnValue().setValue(expressionValue);

			return;
		}

		evaluateBlockStatement(ir.blockStatement(), state);
	}

	void evaluateStatement(const StatementIr& ir, LocalState& state)
	{
		switch (ir.statementType)
		{
			case StatementType::Declaration:
				return evaluateDeclarationStatement(static_cast<const DeclarationStatementIr&>(ir), state);

			case StatementType::Expression:
				return evaluateExpressionStatement(static_cast<const ExpressionStatementIr&>(ir), state);

			case StatementType::Return:
				return evaluateReturnStatement(static_cast<const ReturnStatementIr&>(ir), state);

			case StatementType::Break:
				return evaluateBreakStatement(static_cast<const BreakStatementIr&>(ir), state);

			case StatementType::Continue:
				return evaluateContinueStatement(static_cast<const ContinueStatementIr&>(ir), state);

			case StatementType::Yield:
				return evaluateYieldStatement(static_cast<const YieldStatementIr&>(ir), state);

			case StatementType::For:
				return evaluateForStatement(static_cast<const ForStatementIr&>(ir), state);

			case StatementType::Block:
				return evaluateBlockStatement(static_cast<const BlockStatementIr&>(ir), state);

			case StatementType::Assignment:
				return evaluateAssignmentStatement(static_cast<const AssignmentStatementIr&>(ir), state);

			case StatementType::If:
				return evaluateIfStatement(static_cast<const IfStatementIr&>(ir), state);

			default:
				break;
		}

		log::fatal("Unable to evaluate statement of type: $", (int)ir.statementType);
	}

	void evaluateDeclarationStatement(const DeclarationStatementIr& ir, LocalState& state)
	{
		auto& variableValue = evaluateExpression(ir.value(), state);

		variableValue.setNode(ir.variable());

		log::debug("Declaring variable: $", variableValue);
	}

	void evaluateExpressionStatement(const ir::ExpressionStatementIr& ir, LocalState& state)
	{
		evaluateExpression(ir.expression(), state);
	}

	void evaluateReturnStatement(const ReturnStatementIr& ir, LocalState& state)
	{
		if (ir.hasValue())
		{
			auto& value = evaluateExpression(ir.value(), state);

			state.returnValue().setValue(value);
		}

		state.setReturning(JumpType::Return);
	}

	void evaluateBreakStatement(const ir::BreakStatementIr&, LocalState& state)
	{
		state.setReturning(JumpType::Break);
	}

	void evaluateContinueStatement(const ir::ContinueStatementIr&, LocalState& state)
	{
		state.startContinue();
	}

	void evaluateYieldStatement(const ir::YieldStatementIr&, LocalState&)
	{
		log::notImplemented(here());
	}

	void evaluateForStatement(const ir::ForStatementIr& ir, LocalState& state)
	{
		usize index = state.getScopeIndex();

		evaluateDeclarationStatement(ir.declaration(), state);

		while (true)
		{
			auto& conditionValue = evaluateExpression(ir.condition(), state);

			if (!conditionValue.getValue<bool>())
				break;

			evaluateBlockStatement(ir.body(), state);

			if (state.isBreaking())
				break;

			evaluateStatement(ir.action(), state);

			state.cancelContinue();
		}

		state.cancelBreak();

		state.clearScopeValues(index);
	}

	void evaluateBlockStatement(const BlockStatementIr& ir, LocalState& state)
	{
		usize index = state.getScopeIndex();

		for (const auto *statement : ir.statements())
		{
			evaluateStatement(*statement, state);

			if (state.isJumping())
				break;
		}

		state.clearScopeValues(index);
	}

	void evaluateAssignmentStatement(const AssignmentStatementIr& ir, LocalState& state)
	{
		auto& lhs = state.findValue(ir.identifier().value());
		auto& rhs = evaluateExpression(ir.value(), state);

		evaluateAssignmentOperator(ir.op(), lhs, rhs, state);
	}
	
	void evaluateIfStatement(const IfStatementIr& ir, LocalState& state)
	{
		auto& conditionValue = evaluateExpression(ir.condition(), state);
		
		if (conditionValue.getValue<bool>())
		{
			evaluateStatement(ir.thenCase(), state);
		}
		else if (ir.hasElseCase())
		{
			evaluateStatement(ir.elseCase(), state);
		}
	}

	Value& evaluateExpression(const ExpressionIr& ir, LocalState& state)
	{
		switch (ir.expressionType)
		{
			case ExpressionType::Binary:
				return evaluateBinaryExpression(static_cast<const BinaryExpressionIr&>(ir), state);

			case ExpressionType::Call:
				return evaluateCallExpression(static_cast<const ir::CallExpressionIr&>(ir), state);

			case ExpressionType::Conditional:
				return evaluateConditionalExpression(static_cast<const ir::ConditionalExpressionIr&>(ir), state);

			case ExpressionType::Identifier:
				return evaluateIdentifierExpression(static_cast<const IdentifierExpressionIr&>(ir), state);

			case ExpressionType::Subscript:
				break;

			case ExpressionType::MemberAccess:
				break;

			case ExpressionType::Prefix:
				break;

			case ExpressionType::Cast:
				return evaluateCastExpression(static_cast<const CastExpressionIr&>(ir), state);

			case ExpressionType::BoolLiteral:
				return evaluateBoolLiteral(static_cast<const BoolLiteralIr&>(ir), state);

			case ExpressionType::CharLiteral:
				return evaluateCharLiteral(static_cast<const CharLiteralIr&>(ir), state);

			case ExpressionType::FloatLiteral:
				return evaluateFloatLiteral(static_cast<const FloatLiteralIr&>(ir), state);

			case ExpressionType::IntegerLiteral:
				return evaluateIntegerLiteral(static_cast<const IntegerLiteralIr&>(ir), state);

			case ExpressionType::StringLiteral:
				return evaluateStringLiteral(static_cast<const StringLiteralIr&>(ir), state);
				
			default:
				break;
		}

		log::fatal("Unable to evaluate Expression with TypeIr: $", ir.expressionType);
	}

	Value& evaluateBinaryExpression(const BinaryExpressionIr& ir, LocalState& state)
	{
		auto& lhs = evaluateExpression(ir.lhs(), state);
		auto& rhs = evaluateExpression(ir.rhs(), state);
		auto& result = evaluateBinaryOperator(ir.op(), lhs, rhs, state);

		return result;
	}

	Value& evaluateCallExpression(const ir::CallExpressionIr& ir, LocalState& state)
	{
		return evaluateFunction(ir.subject(), ir.arguments(), state);
	}

	Value& evaluateConditionalExpression(const ir::ConditionalExpressionIr& ir, LocalState& state)
	{
		auto& conditionValue = evaluateExpression(ir.condition(), state);
		auto& value = conditionValue.getValue<bool>()
			? evaluateExpression(ir.thenCase(), state)
			: evaluateExpression(ir.elseCase(), state);

		return value;
	}

	Value& evaluateIdentifierExpression(const IdentifierExpressionIr& ir, LocalState& state)
	{
		// TODO: This makes no sense for function calls

		auto& value = state.findValue(ir.value());

		return value;
	}

	Value& evaluateCastExpression(const ir::CastExpressionIr& ir, LocalState& state)
	{
		auto& expressionValue = evaluateExpression(ir.expression(), state);
		auto& castedValue = state.pushValue(ir.type());

		evaluateConversion(ir.conversion(), castedValue, expressionValue, state);

		return castedValue;
	}

	Value& evaluateBoolLiteral(const ir::BoolLiteralIr& ir, LocalState& state)
	{
		auto& result = state.pushValue(ir.type());

		result.setValue(ir.value());

		return result;
	}

	Value& evaluateCharLiteral(const ir::CharLiteralIr& ir, LocalState& state)
	{
		auto& result = state.pushValue(ir.type());

		result.setValue(ir.value());

		return result;
	}

	Value& evaluateFloatLiteral(const ir::FloatLiteralIr& ir, LocalState& state)
	{
		auto& result = state.pushValue(ir.type());

		result.setValue(ir.value());

		return result;
	}

	Value& evaluateIntegerLiteral(const IntegerLiteralIr& ir, LocalState& state)
	{
		auto& result = state.pushValue(ir.type());

		result.setValue(ir.value());

		return result;
	}

	Value& evaluateStringLiteral(const ir::StringLiteralIr& ir, LocalState& state)
	{
		auto& result = state.pushValue(ir.type());

		result.setValue(ir.value().c_str());

		return result;
	}

	Value& evaluateBinaryOperator(const BinaryOperatorIr& ir, Value& left, Value& right, LocalState& state)
	{
		auto op = state.getBinaryOperator(ir.leftType(), ir.rightType(), ir.binaryExpressionType());
		auto& value = op(left, right, state);

		return value;
	}

	void evaluateAssignmentOperator(const ir::AssignmentOperatorIr& ir, Value& left, const Value& right, LocalState& state)
	{
		auto op = state.getAssignmentOperator(ir.left(), ir.right(), ir.assignmentType());

		op(left, right);
	}

	Value& evaluateConversion(const ir::ConversionIr& ir, Value& to, Value& from, LocalState& state)
	{
		auto conversion = state.getConversion(ir.to(), ir.from());
		auto& value = conversion(to, from);

		return value;
	}
}
