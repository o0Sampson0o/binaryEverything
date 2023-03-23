#pragma once
class SyntaxTree
{
	static SyntaxTree& get();
	SyntaxTree(const SyntaxTree&) = delete;
	void operator=(const SyntaxTree&) = delete;
private:
	SyntaxTree();
};