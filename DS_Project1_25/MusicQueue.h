#pragma once
#include "MusicQueueNode.h"

class MusicQueue
{
private:
	MusicQueueNode *head;
	MusicQueueNode *rear;
	int size = 0;

public:
	MusicQueue();
	~MusicQueue();

	bool empty();
	bool isFull();
	bool exist(const string &artist, const string &title, const string &run_time);
	void push(MusicQueueNode *node);
	MusicQueueNode *pop();
	MusicQueueNode *front();
};