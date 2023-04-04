#include "parka/ast/expression/block.hpp"
#include "parka/ast/function/function.hpp"
#include "parka/ast/statement/statement.hpp"
#include "parka/symbol/node_bank.hpp"
#include "parka/token.hpp"
#include "parka/util/print.hpp"

Optional<ExpressionId> Block::parse(Token& token)
{
	if (token.type() != TokenType::LeftBrace)
	{
		printParseError(token, "'{' before block");

		return {};
	}

	token.increment();

	// TODO: Add initial capacity
	auto statements = Array<StatementId>();

	while (token.type() != TokenType::RightBrace)
	{
		auto statement = Statement::parse(token);

		if (!statement)
			return {};

		statements.push(statement.unwrap());
	}

	token.increment();

	auto block = Block(std::move(statements));
	auto id = NodeBank::add(std::move(block));

	return id;
}

bool Block::validate(const EntityId& functionId)
{
	auto& function = NodeBank::getFunction(functionId);
	
	function.pushBlock();

	auto success = true;

	for (auto statement : _statements)
	{
		if (!NodeBank::get(statement).validate(functionId))
			success = false;
	}

	function.popBlock();

	return success;
}

Optional<Type> Block::getType(Ref<Type>) const
{
	if (_returnType)
		return Type(*_returnType);

	return Type::voidType;
}
