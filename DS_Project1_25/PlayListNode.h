#pragma once
#include <string>
using namespace std;

// Forward declaration of PlayList class to allow PlayListNode to reference it
class PlayList;

class PlayListNode
{
private:
	string artist;
	string title;
	int runtime_sec;

	PlayListNode *prev;
	PlayListNode *next;

	// Declare PlayList as a friend class to allow to private members
	friend class PlayList;

public:
	// Default constructor
	PlayListNode() : artist(""), title(""), runtime_sec(0), prev(nullptr), next(nullptr) {}
	// Constructor initializing node with artist, title, and runtime
	PlayListNode(const string &artist, const string &title, int runtime_sec)
		: artist(artist), title(title), runtime_sec(runtime_sec), prev(nullptr), next(nullptr) {}
	// Destructor
	~PlayListNode() {}

	// Returns the artist name
	string getArtist() const
	{
		return artist;
	}

	// Returns the song title
	string getTitle() const
	{
		return title;
	}

	// Returns the runtime
	int getRunTimeSec() const
	{
		return runtime_sec;
	}

	// Returns the previous node pointer
	PlayListNode *getPrev() const
	{
		return prev;
	}

	// Returns the next node pointer
	PlayListNode *getNext() const
	{
		return next;
	}

	// Returns true if both artist and title match
	bool equals(const string &cur_artist, const string &cur_title) const
	{
		if (cur_artist == artist && cur_title == title)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};