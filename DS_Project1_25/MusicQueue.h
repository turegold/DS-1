#pragma once
#include "MusicQueueNode.h"

class MusicQueue
{
private:
	MusicQueueNode *head;
	MusicQueueNode *rear;
	int size = 0;

public:
	// Constructor
	MusicQueue();
	// Desctructor
	~MusicQueue();

	// Check if queue is empty
	bool empty();
	// Check if queue size exceeds 100
	bool isFull();
	// Check if a specific song already exists in the queue
	bool exist(const string &artist, const string &title, const string &run_time);
	// Insert a node into the queue
	void push(MusicQueueNode *node);
	// Remove and return the front node
	MusicQueueNode *pop();
	// Peek at the front node without removing it
	MusicQueueNode *front();
};