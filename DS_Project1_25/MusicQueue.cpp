#include "MusicQueue.h"
#include <iostream>

// Constructor
MusicQueue::MusicQueue()
{
    head = nullptr;
    rear = nullptr;
    size = 0;
}

// Destructor
MusicQueue::~MusicQueue()
{
    while (!empty())
    {
        MusicQueueNode *delNode = pop();
        delete delNode;
    }
}

// Check if the queue is empty
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

// Check if the queue has reached the maximum size of 100
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

// Check if a specific song already exists in the queue
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

// Insert a node into the queue
void MusicQueue::push(MusicQueueNode *node)
{

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

// Remove and return the front node
MusicQueueNode *MusicQueue::pop()
{
    if (empty())
    {
        std::cerr << "Queue is empty. Terminating program.\n";
        exit(1);
    }

    MusicQueueNode *cur_node = head;

    // If only one node exists
    if (head == rear)
    {
        head = nullptr;
        rear = nullptr;
    }
    else
    {
        head = head->next;
        head->prev = nullptr;
    }

    // Detach the node to be returned
    cur_node->next = nullptr;
    cur_node->prev = nullptr;
    size--;
    return cur_node;
}

// Return the front node without removing it
MusicQueueNode *MusicQueue::front()
{
    return head;
}
