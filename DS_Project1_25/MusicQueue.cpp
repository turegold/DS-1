#include "MusicQueue.h"
#include <iostream>
MusicQueue::MusicQueue()
{
    head = nullptr;
    rear = nullptr;
    size = 0;
}

MusicQueue::~MusicQueue()
{
    while (!empty())
    {
        MusicQueueNode *delNode = pop();
        delete delNode;
    }
}

bool MusicQueue::empty()
{
    if (size == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool MusicQueue::isFull()
{
    if (size >= 100)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool MusicQueue::exist(const string &artist, const string &title, const string &run_time)
{
    MusicQueueNode *cur = head;

    while (cur)
    {
        if (cur->artist == artist && cur->title == title && cur->run_time == run_time)
        {
            return true;
        }
        cur = cur->next;
    }
    return false;
}

void MusicQueue::push(MusicQueueNode *node)
{

    // 큐가 비어있다면
    if (empty())
    {
        head = node;
        rear = node;
    }
    else
    {
        rear->next = node;
        node->prev = rear;
        rear = node;
    }
    size++;
}

MusicQueueNode *MusicQueue::pop()
{
    if (empty())
    {
        std::cerr << "Queue가 비어있기 때문에 프로그램을 종료합니다.\n";
        exit(1);
    }

    MusicQueueNode *cur_node = head;

    if (head == rear)
    { // 노드가 1개뿐일 때
        head = nullptr;
        rear = nullptr;
    }
    else
    { // head의 다음 노드를 head로 설정
        head = head->next;
        head->prev = nullptr;
    }
    // pop될 노드의 연결 해제
    cur_node->next = nullptr;
    cur_node->prev = nullptr;
    size--;
    return cur_node;
}

MusicQueueNode *MusicQueue::front()
{
    // 제일 앞에 있는 노드 리턴
    return head;
}
