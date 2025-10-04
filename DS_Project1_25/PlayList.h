#pragma once
#include <string>
#include <fstream>
using namespace std;

class PlayListNode;

class PlayList
{
private:
	PlayListNode *head;
	PlayListNode *cursor;
	int count;
	int time;

public:
	// Constructor
	PlayList();
	// Descructor
	~PlayList();
	// Inserts a new node into the playlist
	void insert_node(const string &artist, const string &title, int runtime_sec);
	// Deletes all songs by the given artist
	bool deleteArtist(const string &artist);
	// Deletes all songs with the given title
	bool deleteTitle(const string &title);
	// Delete a specific song from the playlist
	bool deleteFromList(const string &artist, const string &title);
	// Checks if any song with the given artist exists
	bool is_existArtist(const string &artist);
	// Checks if any song with the given title exists
	bool is_existTitle(const string &title);
	// Returns true if the playlist is empty
	bool empty();
	// Returns true if the playlist is full
	bool full();
	// Check existence of any node
	bool exist(const string &artist, const string &title);
	// Returns the formatted string of the playlist for logging
	string print();
	// Returns total runtime of the playlist
	int run_time();
	// Returns number of songs in the playlist
	int size() const;
};