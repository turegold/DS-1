#pragma once
#include "TitleBST.h"
#include "MusicQueueNode.h"
#include <vector>

class TitleBST;

class TitleBSTNode
{
	// Allow TitleBST class to access private members of TitleBSTNode
	friend class TitleBST;

private:
	string title;
	vector<string> artist;
	vector<string> run_time;
	int count;
	TitleBSTNode *left;
	TitleBSTNode *right;

public:
	// Default constructor
	TitleBSTNode() = default;

	// Constructor with initial song data
	TitleBSTNode(const string &title, const string &artist, const string &run_time)
		: title(title), artist{artist}, run_time{run_time}, count(1), left(nullptr), right(nullptr) {}

	// Default destructor
	~TitleBSTNode() = default;
};