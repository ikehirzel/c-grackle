#include "parka/evaluation/GlobalState.hpp"
#include "parka/enum/AssignmentType.hpp"
#include "parka/evaluation/IntrinsicAssignmentOperator.hpp"
#include "parka/evaluation/IntrinsicBinaryOperator.hpp"
#include "parka/evaluation/IntrinsicConversion.hpp"
#include "parka/ir/LValueIr.hpp"
#include "parka/log/Log.hpp"
#include "parka/symbol/BinaryOperatorKey.hpp"

namespace parka::evaluation
{
	GlobalState::GlobalState():
		_intrinsicBinaryOperators(getIntrinsicBinaryOperators()),
		_intrinsicAssignmentOperators(getIntrinsicAssignmentOperators()),
		_intrinsicConversions(getIntrinsicConversions())
	{}

	Value& GlobalState::add(ir::LValueIr *, const ir::TypeIr&)
	{
		log::notImplemented(here());
		// auto insertion = _values.insert(key, Value(type));

		// if (!insertion)
		// 	log::fatal("Failed to insert value for $.", (void*)key);

		// return *insertion;
	}

	IntrinsicBinaryOperator GlobalState::getBinaryOperator(const ir::TypeIr& left, const ir::TypeIr& right, BinaryExpressionType binaryExpressionType) const
	{
		auto key = BinaryOperatorKey(left, right, binaryExpressionType);
		auto* op = _intrinsicBinaryOperators.find(key);

		if (!op)
			log::fatal("Unable to find intrinsic operator `$ $ $`.", left, binaryExpressionType, right);

		return *op;
	}

	IntrinsicAssignmentOperator GlobalState::getAssignmentOperator(const ir::TypeIr& left, const ir::TypeIr& right, AssignmentType assignmentType) const
	{
		auto key = AssignmentOperatorKey(left, right, assignmentType);
		auto* op = _intrinsicAssignmentOperators.find(key);

		if (!op)
			log::fatal("Unable to find intrinsic operator `$ $ $`.", left, assignmentType, right);

		return *op;
	}

	IntrinsicConversion GlobalState::getConversion(const ir::TypeIr& to, const ir::TypeIr& from) const
	{
		auto key = ConversionKey(to, from);
		auto* conversion = _intrinsicConversions.find(key);

		if (!conversion)
			log::fatal("Unable to find intrinsic conversion from `$` to `$`.", to, from);

		return *conversion;
	}
}
