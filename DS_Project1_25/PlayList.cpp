#include "PlayList.h"
#include "PlayListNode.h"
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

PlayList::PlayList()
{
    head = nullptr;
    cursor = nullptr;
    count = 0;
    time = 0;
}

PlayList::~PlayList()
{
    // 모든 노드 삭제
    PlayListNode *cur = head;
    while (cur)
    {
        PlayListNode *temp = cur;
        cur = cur->getNext();
        delete temp;
    }
}

void PlayList::insert_node(const string &artist, const string &title, int runtime_sec)
{
    // 중복 검사
    PlayListNode *cur = head;
    while (cur)
    {
        if (cur->equals(artist, title))
        {
            return;
        }
        cur = cur->getNext();
    }

    PlayListNode *newNode = new PlayListNode(artist, title, runtime_sec);

    if (!head)
    {
        head = newNode;
        cursor = newNode;
    }
    else
    {
        PlayListNode *tail = head;
        while (tail->next)
        {
            tail = tail->next;
        }
        tail->next = newNode;
        newNode->prev = tail;
    }

    count++;
    time += runtime_sec;
}

void PlayList::delete_node()
{
    // 구현 예정
}

bool PlayList::empty()
{
    if (head == nullptr)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool PlayList::full()
{
    if (count >= 10)
    {
        return false;
    }
}

bool PlayList::exist()
{
    if (cursor != nullptr)
    {
        return true;
    }
    else
    {
        return false;
    }
}

string PlayList::print()
{
    stringstream ss;
    PlayListNode *cur = head;

    while (cur)
    {
        int rt = cur->getRunTimeSec();
        int mm = rt / 60;
        int ss_sec = rt % 60;

        ss << cur->getArtist() << "|" << cur->getTitle() << "|"
           << setfill('0') << setw(2) << mm << ":"
           << setfill('0') << setw(2) << ss_sec << "\n";

        cur = cur->getNext();
    }

    // 노래 개수 출력
    ss << "노래 개수 : " << count << "\n";

    // 총 재생 시간 출력
    int total_min = time / 60;
    int total_sec = time % 60;
    ss << "총 플레이타임 : "
       << setfill('0') << setw(2) << total_min << ":"
       << setfill('0') << setw(2) << total_sec << "\n";

    return ss.str();
}

int PlayList::run_time()
{
    // 나중에 구현
    return time;
}

int PlayList::size() const
{
    return count;
}