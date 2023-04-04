#ifndef PARKA_UTIL_ENTITY_ENTITY_BANK_HPP
#define PARKA_UTIL_ENTITY_ENTITY_BANK_HPP

#include "parka/ast/expression/expression.hpp"
#include "parka/ast/statement/statement.hpp"
#include "parka/symbol/entity.hpp"
#include "parka/symbol/entity_id.hpp"
#include "parka/symbol/expression_id.hpp"
#include "parka/symbol/statement_id.hpp"
#include "parka/util/table.hpp"

class Package;
class Struct;
class Function;
class Variable;
class Parameter;
class AdditiveExpression;
class Assignment;
class BitwiseAndExpression;
class BitwiseOrExpression;
class BitwiseXorExpression;
class Block;
class BooleanAndExpression;
class BooleanOrExpression;
class CallExpression;
class ConditionalExpression;
class EqualityExpression;
class IdentifierExpression;
class IfExpression;
class IndexExpression;
class MemberAccess;
class MultiplicativeExpression;
class Prefix;
class RelationalExpression;
class BitShiftExpression;
class BoolLiteral;
class CharLiteral;
class FloatLiteral;
class IntegerLiteral;
class StringLiteral;
class Declaration;
class ExpressionStatement;
class JumpStatement;

struct NodeBank
{
	static const EntityId i32Id;
	static const EntityId voidId;

	static void initialize();

	static EntityId add(Package&& value);
	static EntityId add(Struct&& value);
	static EntityId add(Function&& value);
	static EntityId add(Variable&& value);
	static EntityId add(Parameter&& value);

	static ExpressionId add(AdditiveExpression&& value);
	static ExpressionId add(Assignment&& value);
	static ExpressionId add(BitwiseAndExpression&& value);
	static ExpressionId add(BitwiseOrExpression&& value);
	static ExpressionId add(BitwiseXorExpression&& value);
	static ExpressionId add(Block&& value);
	static ExpressionId add(BooleanAndExpression&& value);
	static ExpressionId add(BooleanOrExpression&& value);
	static ExpressionId add(CallExpression&& value);
	static ExpressionId add(ConditionalExpression&& value);
	static ExpressionId add(EqualityExpression&& value);
	static ExpressionId add(IdentifierExpression&& value);
	static ExpressionId add(IfExpression&& value);
	static ExpressionId add(IndexExpression&& value);
	static ExpressionId add(MemberAccess&& value);
	static ExpressionId add(MultiplicativeExpression&& value);
	static ExpressionId add(Prefix&& value);
	static ExpressionId add(RelationalExpression&& value);
	static ExpressionId add(BitShiftExpression&& value);

	// Literals
	static ExpressionId add(BoolLiteral&& value);
	static ExpressionId add(CharLiteral&& value);
	static ExpressionId add(FloatLiteral&& value);
	static ExpressionId add(IntegerLiteral&& value);
	static ExpressionId add(StringLiteral&& value);

	// Statements
	static StatementId add(Declaration&& value);
	static StatementId add(ExpressionStatement&& value);
	static StatementId add(JumpStatement&& value);

	static Entity& get(EntityId id);
	static Expression& get(ExpressionId id);
	static Statement& get(StatementId id);

	static Package& getPackage(EntityId id);
	static Struct& getStruct(EntityId id);
	static Function& getFunction(EntityId id);
	static Variable& getVariable(EntityId id);
	static Parameter& getParameter(EntityId id);

	static EntityId getId(Function& value);
	static EntityId getId(Package& value);
	
	static void declarePrimitives(Table<String, EntityId>& globalSymbols);
	static usize getGlobalCount();
	static usize getPackageCount();
};

#endif
