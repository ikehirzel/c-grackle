#include "parka/syntax/StatementSyntax.hpp"
#include "parka/syntax/KeywordSyntax.hpp"
#include "parka/syntax/DeclarationStatementSyntax.hpp"
#include "parka/syntax/ExpressionStatementSyntax.hpp"
#include "parka/syntax/JumpStatementSyntax.hpp"
#include "parka/util/Print.hpp"

namespace parka
{
	Optional<StatementSyntaxId> StatementSyntax::parse(Token& token)
	{
		auto keywordType = KeywordSyntax::getKeywordType(token);

		switch (keywordType)
		{
			case KeywordType::Var:
				return DeclarationStatementSyntax::parse(token);

			case KeywordType::Return:
			case KeywordType::Break:
			case KeywordType::Continue:
			case KeywordType::Yield:
				return JumpStatementSyntax::parse(token);

			default:
				break;
		}

		return ExpressionStatementSyntax::parse(token);
	}
}
