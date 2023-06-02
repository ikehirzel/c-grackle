#include "parka/syntax/KeywordSyntax.hpp"
#include "parka/log/Log.hpp"
#include "parka/util/Optional.hpp"
#include "parka/util/Print.hpp"
#include "parka/util/String.hpp"
#include "parka/util/Table.hpp"

namespace parka
{
	Table<String, KeywordType> keywords(25);

	void KeywordSyntax::initialize()
	{
		if (keywords.count() > 0)
			return;
			
		keywords.insert("break", KeywordType::Break);
		keywords.insert("case", KeywordType::Case);
		keywords.insert("continue", KeywordType::Continue);
		keywords.insert("yield", KeywordType::Yield);
		keywords.insert("else", KeywordType::Else);
		keywords.insert("enum", KeywordType::Enum);
		keywords.insert("import", KeywordType::Import);
		keywords.insert("export", KeywordType::Export);
		keywords.insert("false", KeywordType::False);
		keywords.insert("for", KeywordType::For);
		keywords.insert("function", KeywordType::Function);
		keywords.insert("operator", KeywordType::Operator);
		keywords.insert("if", KeywordType::If);
		keywords.insert("loop", KeywordType::Loop);
		keywords.insert("match", KeywordType::Match);
		keywords.insert("mut", KeywordType::Mut);
		keywords.insert("return", KeywordType::Return);
		keywords.insert("struct", KeywordType::StructSyntax);
		keywords.insert("then", KeywordType::Then);
		keywords.insert("true", KeywordType::True);
		keywords.insert("type", KeywordType::Type);
		keywords.insert("private", KeywordType::Private);
		keywords.insert("public", KeywordType::Public);
		keywords.insert("var", KeywordType::Var);
		keywords.insert("while", KeywordType::While);
	}

	KeywordType KeywordSyntax::getKeywordType(const Token& token)
	{
		if (token.type() != TokenType::Identifier)
			return {};

		auto text = token.text();
		const auto *keywordType = keywords.find(text);

		if (!keywordType)
			return KeywordType::None;

		return *keywordType;
	}

	Optional<KeywordSyntax> KeywordSyntax::parseBool(Token& token)
	{
		auto type = getKeywordType(token);

		if (type != KeywordType::True && type != KeywordType::False)
		{
			Log::parseError(token, "`true` or `false`");
			return {};
		}

		auto keyword = KeywordSyntax(token, type);

		token.increment();
		
		return keyword;
	}

	Optional<KeywordSyntax> KeywordSyntax::parseStruct(Token& token)
	{
		auto type = getKeywordType(token);

		if (type != KeywordType::StructSyntax)
		{
			Log::parseError(token, "`struct` keyword");
			return {};
		}

		auto keyword = KeywordSyntax(token, type);

		token.increment();

		return keyword;
	}

	Optional<KeywordSyntax> KeywordSyntax::parseVar(Token& token)
	{
		auto type = getKeywordType(token);

		if (type != KeywordType::Var)
		{
			Log::parseError(token, "`var` keyword");
			return {};
		}

		auto keyword = KeywordSyntax(token, type);

		token.increment();

		return keyword;
	}

	Optional<KeywordSyntax> KeywordSyntax::parseFunction(Token &token)
	{
		auto type = getKeywordType(token);

		if (type != KeywordType::Function)
		{
			Log::parseError(token, "`function`");
			return {};
		}

		auto keyword = KeywordSyntax(token, type);

		token.increment();
		
		return keyword;
	}

	Optional<KeywordSyntax> KeywordSyntax::parseOperator(Token &token)
	{
		auto type = getKeywordType(token);

		if (type != KeywordType::Operator)
		{
			Log::parseError(token, "`operator`");
			return {};
		}

		auto keyword = KeywordSyntax(token, type);

		token.increment();

		return keyword;
	}

	Optional<KeywordSyntax> KeywordSyntax::parseMut(Token &token)
	{
		auto type = getKeywordType(token);

		if (type != KeywordType::Mut)
			return {};

		auto keyword = KeywordSyntax(token, type);

		token.increment();

		return keyword;
	}
}
