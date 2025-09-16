#pragma once
#include "ArtistBSTNode.h"

class ArtistBST
{
private:
	ArtistBSTNode *root;
	// ArtistBSTNode *parent;
	// string data;
	// ArtistBSTNode *target;

	void destroyTree(ArtistBSTNode *node);
	void printNode(ArtistBSTNode *node, ofstream &flog);

public:
	ArtistBST();
	~ArtistBST();

	void printTree(ofstream &flog);
	bool isEmpty() const;
	void insert(MusicQueueNode *node);
	void search();
	void print();
	void delete_node();
};