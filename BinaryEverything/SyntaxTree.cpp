#include "SyntaxTree.h"

SyntaxTree& SyntaxTree::get()
{
	static SyntaxTree syntaxTree;
	return syntaxTree;
}

SyntaxTree::SyntaxTree()
{
}
