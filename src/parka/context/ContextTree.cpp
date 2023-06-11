#include "parka/log/Log.hpp"
#include "parka/syntax/SyntaxTree.hpp"

namespace parka
{
	ContextTree::ContextTree(PackageContext& globalPackage) :
	_globalPackage(globalPackage)
	{}

	Optional<ContextTree> SyntaxTree::validate()
	{
		log::notImplemented(here());
	}
}
