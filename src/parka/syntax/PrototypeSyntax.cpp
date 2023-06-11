#include "parka/syntax/PrototypeSyntax.hpp"
#include "parka/log/Log.hpp"
#include "parka/syntax/FunctionSyntax.hpp"
#include "parka/syntax/ParameterSyntax.hpp"
#include "parka/symbol/Identifier.hpp"
#include "parka/syntax/KeywordSyntax.hpp"
#include "parka/syntax/TypeAnnotationSyntax.hpp"
#include "parka/type/ValueType.hpp"
#include "parka/util/Array.hpp"
#include "parka/util/Print.hpp"

namespace parka
{
	PrototypeSyntax::PrototypeSyntax(KeywordSyntax&& keyword, Identifier&& identifier, Array<ParameterSyntax*>&& parameters, Optional<TypeAnnotationSyntax>&& returnType) :
	_keyword(std::move(keyword)),
	_identifier(std::move(identifier)),
	_parameters(std::move(parameters)),
	_returnType(std::move(returnType))
	{}

	Optional<Array<ParameterSyntax*>> parseParameterList(Token& token)
	{
		if (token.type() != TokenType::LeftParenthesis)
		{
			log::parseError(token, "'(' after function name");
			return {};
		}

		token.increment();

		auto parameters = Array<ParameterSyntax*>();

		if (token.type() == TokenType::RightParenthesis)
		{
			token.increment();

			return parameters;
		}
		
		while (true)
		{
			auto parameter = ParameterSyntax::parse(token);

			if (!parameter) // TODO: Attempt to fast forward to parameter
				return {};

			parameters.push(parameter);

			if (token.type() == TokenType::Comma)
			{
				token.increment();
				continue;
			}

			break;
		}

		if (token.type() != TokenType::RightParenthesis)
		{
			log::parseError(token, "')'", "Invalid tokens in parameter list");
			return {};
		}

		token.increment();

		return parameters;
	}

	Optional<TypeAnnotationSyntax> parseReturnType(Token& token)
	{
		if (token.type() != TokenType::SingleArrow)
			return {};

		token.increment();
		
		auto returnType = TypeAnnotationSyntax::parse(token);

		return returnType;
	}

	Optional<PrototypeSyntax> PrototypeSyntax::parse(Token& token)
	{
		auto keyword = KeywordSyntax::parseFunction(token);

		if (!keyword)
			return {};

		auto identifier = Identifier::parse(token);

		if (!identifier)
			return {};

		auto parameters = parseParameterList(token);
		
		if (!parameters)
			return {};

		auto returnType = parseReturnType(token);

		// TODO: Handle returnType error?

		auto prototype = PrototypeSyntax(*keyword, *identifier, *parameters, std::move(returnType));

		return prototype;
	}

	static Optional<ValueType> validateReturnType(Optional<TypeAnnotationSyntax>& syntax, SymbolTable& symbolTable)
	{
		if (!syntax)
			return ValueType::voidType;

		return syntax->validate(symbolTable);
	}

	Optional<PrototypeContext> PrototypeSyntax::validate(FunctionSymbolTable& symbolTable)
	{
		auto success = true;
		const auto parameterCount = _parameters.length();
		auto parameters = Array<ParameterContext*>(parameterCount);

		for (auto *parameterSyntax : _parameters)
		{
			auto *parameterContext = parameterSyntax->validate(symbolTable);

			if (parameterContext == nullptr)
			{
				success = false;
				continue;
			}

			parameters.push(parameterContext);
		}

		auto returnType = validateReturnType(_returnType, symbolTable);

		if (!returnType)
			success = false;

		if (!success)
			return {};

		auto context = PrototypeContext(std::move(parameters), *returnType);

		return context;
	}
}
