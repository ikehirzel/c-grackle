#ifndef PARKA_IR_U64_PRIMITIVE_IR_HPP
#define PARKA_IR_U64_PRIMITIVE_IR_HPP

#include "parka/ir/LValueIr.hpp"
#include "parka/ir/InvalidTypeIr.hpp"

namespace parka::ir
{
	class U64PrimitiveIr: public TypeIr, public LValueIr
	{
		String _symbol;

	public:

		static U64PrimitiveIr instance;

	private:

		U64PrimitiveIr();
		U64PrimitiveIr(U64PrimitiveIr&&) = delete;
		U64PrimitiveIr(const U64PrimitiveIr&) = delete;

	public:

		BinaryOperatorIr* getBinaryOperator(BinaryExpressionType, const TypeIr&) const;
		AssignmentOperatorIr* getAssignmentOperator(AssignmentType, const TypeIr&) const;

		std::ostream& printType(std::ostream& out) const;
		const String& symbol() const { return _symbol; }
		const TypeIr& type() const { return InvalidTypeIr::typeNameType; }
	};
}

#endif
