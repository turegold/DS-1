#pragma once
#include "MusicQueueNode.h"
#include <string>
#include <vector>

using namespace std;

// Forward declaration of ArtistBST to allow friendship
class ArtistBST;

class ArtistBSTNode
{
	// ArtistBST can access private members
	friend class ArtistBST;

private:
	string artist;
	vector<string> title;
	vector<string> run_time;
	int count = 0;
	ArtistBSTNode *left = nullptr;
	ArtistBSTNode *right = nullptr;

public:
	// Default constructor
	ArtistBSTNode() = default;

	// Constructor to create a node with initial song
	ArtistBSTNode(const string &artist, const string &title, const string &run_time)
		: artist(artist), title{title}, run_time{run_time}, count(1), left(nullptr), right(nullptr) {}
	// Default destructor
	~ArtistBSTNode() = default;

	// Getters
	const string &getArtist() const
	{
		return artist;
	}

	const vector<string> &getTitles() const
	{
		return title;
	}

	const vector<string> &getRunTimes() const
	{
		return run_time;
	}

	int getCount() const
	{
		return count;
	}

	ArtistBSTNode *getLeft() const
	{
		return left;
	}

	ArtistBSTNode *getRight() const
	{
		return right;
	}
};