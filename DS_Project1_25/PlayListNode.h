#pragma once
#include <string>
#include "PlayList.h"
using namespace std;

class PlayList;

class PlayListNode
{
private:
	string artist;
	string title;
	int runtime_sec;

	PlayListNode *prev;
	PlayListNode *next;

	friend class PlayList;

public:
	PlayListNode() : artist(""), title(""), runtime_sec(0), prev(nullptr), next(nullptr) {}
	PlayListNode(const string &artist, const string &title, int runtime_sec)
		: artist(artist), title(title), runtime_sec(runtime_sec), prev(nullptr), next(nullptr) {}
	~PlayListNode() {}

	string getArtist() const
	{
		return artist;
	}

	string getTitle() const
	{
		return title;
	}

	int getRunTimeSec() const
	{
		return runtime_sec;
	}

	PlayListNode *getPrev() const
	{
		return prev;
	}

	PlayListNode *getNext() const
	{
		return next;
	}

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

	// void set()
	// {
	// }
};