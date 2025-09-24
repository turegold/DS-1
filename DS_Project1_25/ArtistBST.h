#pragma once
#include "ArtistBSTNode.h"
#include "PlayList.h"
class ArtistBST
{
private:
	ArtistBSTNode *root;
	// ArtistBSTNode *parent;
	// string data;
	// ArtistBSTNode *target;

	void destroyTree(ArtistBSTNode *node);
	void printNode(ArtistBSTNode *node, ofstream &flog);
	ArtistBSTNode *deleteTitleRecursive(ArtistBSTNode *node, const string &title, bool &deleted);

public:
	ArtistBST();
	~ArtistBST();

	void printTree(ofstream &flog);
	bool isEmpty() const;
	bool searchArtist(const string &artist, ofstream &flog, bool is_print);
	bool searchSong(const string &artist, const string &title, ofstream &flog, bool is_print);
	bool searchArtistToPlayList(const string &artist, PlayList &pl, ofstream &flog);
	bool searchSongToPlayList(const string &artist, const string &title, PlayList &pl, ofstream &flog);
	bool deleteArtist(const string &artist);
	bool deleteTitle(const string &title);
	bool deleteSong(const string &artist, const string &title);
	bool insert(MusicQueueNode *node);
	void search();
	void print();
	void delete_node();
};