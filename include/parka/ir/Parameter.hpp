#ifndef PARKA_IR_PARAMETER_HPP
#define PARKA_IR_PARAMETER_HPP

#include "parka/symbol/Resolution.hpp"

namespace parka::ir
{
	class ParameterIr: public Resolution
	{
		String _symbol;
		ValueType _type;

	public:

		ParameterIr(ValueType&& type):
		Resolution(ResolvableType::Parameter),
		_type(std::move(type))
		{}
		ParameterIr(ParameterIr&&) = default;
		ParameterIr(const ParameterIr&) = delete;

		const String& symbol() const { return _symbol; }
		const ValueType *valueType() const { return &_type; }
	};
}

#endif
