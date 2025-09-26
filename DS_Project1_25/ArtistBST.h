#pragma once
#include "ArtistBSTNode.h"
#include "PlayList.h"
class ArtistBST
{
private:
	ArtistBSTNode *root;

	// Recursively delete all nodes in the tree
	void destroyTree(ArtistBSTNode *node);
	// Print all songs in in-order traversal
	void printNode(ArtistBSTNode *node, ofstream &flog);
	// Recursively delete a node by title
	ArtistBSTNode *deleteTitleRecursive(ArtistBSTNode *node, const string &title, bool &deleted);
	// Recursively delete a node by artist
	ArtistBSTNode *deleteArtistRecursive(ArtistBSTNode *node, const string &artist, bool &deleted);

public:
	// Constructor
	ArtistBST();
	// Destructor
	~ArtistBST();
	// Print all artists and songs
	void printTree(ofstream &flog);
	// Check if tree is empty
	bool isEmpty() const;
	// Search for artist and optionally print
	bool searchArtist(const string &artist, ofstream &flog, bool is_print);
	// Search for specific song
	bool searchSong(const string &artist, const string &title, ofstream &flog, bool is_print);
	// Search by artist and add it to PlayList
	bool searchArtistToPlayList(const string &artist, PlayList &pl, ofstream &flog);
	// Search specific song and add it playList
	bool searchSongToPlayList(const string &artist, const string &title, PlayList &pl, ofstream &flog);
	// Delete artist and all their songs
	bool deleteArtist(const string &artist);
	// Delete song by title
	bool deleteTitle(const string &title);
	// Delete specific song by artist and title
	bool deleteSong(const string &artist, const string &title);
	// Insert a node from MusicQueue
	bool insert(MusicQueueNode *node);
};