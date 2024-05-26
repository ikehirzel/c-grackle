#ifndef PARKA_IR_TYPE_IR_HPP
#define PARKA_IR_TYPE_IR_HPP

#include "parka/enum/BinaryExpressionType.hpp"

namespace parka::ir
{
	class ConversionIr;
	class BinaryOperatorIr;

	struct TypeIr
	{
		virtual~TypeIr() {}

		virtual ConversionIr* getConversion(const TypeIr&) const { return nullptr; }
		virtual BinaryOperatorIr* getBinaryOperator(BinaryExpressionType, const TypeIr&) const { return nullptr; }
		virtual std::ostream& printType(std::ostream&) const = 0;

		friend std::ostream& operator<<(std::ostream& out, const TypeIr& type)
		{
			return type.printType(out);
		}
	};
}

#endif
