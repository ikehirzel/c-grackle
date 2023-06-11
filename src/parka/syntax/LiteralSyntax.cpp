#include "parka/syntax/LiteralSyntax.hpp"
#include "parka/log/Log.hpp"
#include "parka/syntax/BoolLiteralSyntax.hpp"
#include "parka/syntax/CharLiteralSyntax.hpp"
#include "parka/syntax/FloatLiteralSyntax.hpp"
#include "parka/syntax/IntegerLiteralSyntax.hpp"
#include "parka/syntax/StringLiteralSyntax.hpp"
#include "parka/util/Print.hpp"

namespace parka::LiteralSyntax
{
	ExpressionSyntax *parse(Token& token)
	{
		switch (token.type())
		{
			case TokenType::CharacterLiteral:
				return CharLiteralSyntax::parse(token);

			case TokenType::StringLiteralSyntax:
				return StringLiteralSyntax::parse(token);

			case TokenType::IntegerLiteralSyntax:
				return IntegerLiteralSyntax::parse(token);

			case TokenType::FloatLiteralSyntax:
				return FloatLiteralSyntax::parse(token);

			case TokenType::Identifier:
				return BoolLiteralSyntax::parse(token);

			default:
				break;
		}
		
		log::parseError(token, "literal");
		
		return {};
	}
}
