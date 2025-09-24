#pragma once
// #include "MusicQueue.h"
#include <string>
#include <cstring>
#include <sstream>

using namespace std;

class MusicQueueNode
{
private:
	string artist;
	string title;
	string run_time;

	MusicQueueNode *next;
	MusicQueueNode *prev;

public:
	MusicQueueNode()
	{
		next = nullptr;
		prev = nullptr;
	}
	MusicQueueNode(string artist, string title, string run_time) : artist(artist), title(title), run_time(run_time), next(nullptr), prev(nullptr)
	{
	}
	~MusicQueueNode() {}

	string getArtist() const
	{
		return artist;
	}

	string getTitle() const
	{
		return title;
	}

	string getRunTime() const
	{
		return run_time;
	}

	MusicQueueNode *getNext() const
	{
		return next;
	}

	friend class MusicQueue;
};