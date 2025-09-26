#pragma once
#include <string>
#include <fstream>
class TitleBSTNode;
class PlayList;
class MusicQueueNode;

class TitleBST
{
private:
	TitleBSTNode *root;
	TitleBSTNode *parent;

	// Helper to recursively delete all nodes
	void destroyTree(TitleBSTNode *node);
	// Helper to print all nodes in-order
	void printNode(TitleBSTNode *node, ofstream &flog);
	// Helper to delete all songs by a specific artist
	TitleBSTNode *deleteArtistRecursive(TitleBSTNode *node, const string &artist, bool &deleted);

public:
	// Constructor
	TitleBST();
	// Destructor
	~TitleBST();

	// Search for a title in the BST and optionally print its data
	bool searchTitle(const string &titleName, ofstream &flog, bool is_print);
	// Search a title and add it to PlayList if found
	bool searchTitleToPlayList(const string &title, PlayList &pl, ofstream &flog);
	// Print entire tree in-order
	void printTree(ofstream &flog);
	// Delete all songs by a specific artist
	bool deleteArtist(const string &artist);
	// Delete a specific title
	bool deleteTitle(const string &title);
	// Delete a specific song
	bool deleteSong(const string &artist, const string &title);
	// Check whether the tree is empty
	bool isEmpty() const;
	// Insert a new MusicQueueNode
	bool insert(MusicQueueNode *node);
};