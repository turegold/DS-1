#pragma once
#include <string>
#include <cstring>
#include <sstream>

using namespace std;

// Node class for MusicQueue
class MusicQueueNode
{
private:
	string artist;
	string title;
	string run_time;

	MusicQueueNode *next;
	MusicQueueNode *prev;

public:
	// Default constructor
	MusicQueueNode()
	{
		next = nullptr;
		prev = nullptr;
	}
	// Parameterized constructor
	MusicQueueNode(string artist, string title, string run_time) : artist(artist), title(title), run_time(run_time), next(nullptr), prev(nullptr)
	{
	}
	// Destructor
	~MusicQueueNode() {}

	// Getters for song data
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

	// Getter for next node
	MusicQueueNode *getNext() const
	{
		return next;
	}
	// Grant access to MusicQueue class
	friend class MusicQueue;
};