#pragma once
#include "TitleBSTNode.h"

class TitleBST
{
private:
	TitleBSTNode *root;
	TitleBSTNode *parent;
	string data;
	TitleBSTNode *target;

	void destroyTree(TitleBSTNode *node);
	void printNode(TitleBSTNode *node, ofstream &flog);

public:
	TitleBST();
	~TitleBST();

	void printTree(ofstream &flog);
	bool isEmpty() const;
	void insert(MusicQueueNode *node);
	void search();
	void print();
	void delete_node();
};