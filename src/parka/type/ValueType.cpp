#include "parka/log/Log.hpp"
#include "parka/ast/AdditiveExpression.hpp"
#include "parka/ast/Literal.hpp"
#include "parka/ast/Function.hpp"
#include "parka/intrinsic/Primitive.hpp"
#include "parka/ast/TypeAnnotation.hpp"
#include "parka/type/ValueType.hpp"

namespace parka
{
	const ValueType ValueType::voidType(Primitive::voidPrimitive);
	const ValueType ValueType::u8Type(Primitive::u8Primitive);
	const ValueType ValueType::u16Type(Primitive::u16Primitive);
	const ValueType ValueType::u32Type(Primitive::u32Primitive);
	const ValueType ValueType::u64Type(Primitive::u64Primitive);
	const ValueType ValueType::i8Type(Primitive::i8Primitive);
	const ValueType ValueType::i16Type(Primitive::i16Primitive);
	const ValueType ValueType::i32Type(Primitive::i32Primitive);
	const ValueType ValueType::i64Type(Primitive::i64Primitive);
	const ValueType ValueType::f32Type(Primitive::f32Primitive);
	const ValueType ValueType::f64Type(Primitive::f64Primitive);
	const ValueType ValueType::boolType(Primitive::boolPrimitive);
	const ValueType ValueType::charType(Primitive::charPrimitive);
	const ValueType ValueType::stringType(Primitive::stringPrimitive);

	ValueType::ValueType(EntityContext& entity) :
	_entity(entity)
	{}

	bool ValueType::canConvertTo(const ValueType& to) const
	{
		return &_entity == &to._entity;
	}

	std::ostream& operator<<(std::ostream& out, const ValueType& type)
	{
		// TODO: Get Symbols
		log::notImplemented(here());
		// output symbol
		// auto& entity = *type.entityId();
		// const auto& identifier = entity.identifier();

		// out << '`' << identifier << '`';

		return out;
	}
}
