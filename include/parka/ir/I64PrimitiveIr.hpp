#ifndef PARKA_IR_I64_PRIMITIVE_IR_HPP
#define PARKA_IR_I64_PRIMITIVE_IR_HPP

#include "parka/ir/LValueIr.hpp"
#include "parka/ir/TypeNameIr.hpp"

namespace parka::ir
{
	class I64PrimitiveIr: public TypeIr, public LValueIr
	{
		String _symbol;

	public:

		static I64PrimitiveIr instance;

	private:

		I64PrimitiveIr();
		I64PrimitiveIr(I64PrimitiveIr&&) = delete;
		I64PrimitiveIr(const I64PrimitiveIr&) = delete;

	public:

		std::ostream& printType(std::ostream& out) const;
		const String& symbol() const { return _symbol; }
		const TypeIr& type() const { return TypeNameIr::instance; }
	};
}

#endif
