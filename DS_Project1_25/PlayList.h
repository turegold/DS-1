#pragma once
#include "PlayListNode.h"
#include <string>
using namespace std;

class PlayList
{
private:
	PlayListNode *head;
	PlayListNode *cursor;
	int count;
	int time;
	// string data;

public:
	PlayList();
	~PlayList();
	void insert_node(const string &artist, const string &title, int runtime_sec);
	void delete_node();
	bool empty();
	bool full();
	bool exist();
	string print();
	int run_time();
	int size() const;
};