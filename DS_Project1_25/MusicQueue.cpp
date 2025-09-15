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

bool MusicQueue::exist()
{
    // 나중에 구현 예정
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
        return nullptr;
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

// 큐에 담겨있는 노래를 출력하는 메서드 -> 나중에 삭제할 예정
void MusicQueue::printAll()
{
    MusicQueueNode *cur = head;
    while (cur)
    {
        cout << cur->artist << " | " << cur->title << " | " << cur->run_time << endl;
        cur = cur->next;
    }
}