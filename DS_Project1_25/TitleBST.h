#pragma once
#include "TitleBSTNode.h"
#include "PlayList.h"
class TitleBST
{
private:
	TitleBSTNode *root;
	TitleBSTNode *parent;
	string data;
	TitleBSTNode *target;

	void destroyTree(TitleBSTNode *node);
	void printNode(TitleBSTNode *node, ofstream &flog);
	TitleBSTNode *deleteArtistRecursive(TitleBSTNode *node, const string &artist, bool &deleted);

public:
	TitleBST();
	~TitleBST();

	bool searchTitle(const string &titleName, ofstream &flog, bool is_print);
	bool searchTitleToPlayList(const string &title, PlayList &pl, ofstream &flog);
	void printTree(ofstream &flog);
	bool deleteArtist(const string &artist);
	bool deleteTitle(const string &title);
	bool deleteSong(const string &artist, const string &title);
	bool isEmpty() const;
	bool insert(MusicQueueNode *node);
	void search();
	void print();
	void delete_node();
};