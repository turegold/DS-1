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
	bool exist();
	void push(MusicQueueNode *node);
	MusicQueueNode *pop();
	MusicQueueNode *front();

	void printAll(); // 큐에 들어있는 음악 전체 출력용 -> 나중에 삭제 예정
};