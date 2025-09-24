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
    if (!head)
    {
        return;
    }

    // 모든 노드 삭제
    PlayListNode *cur = head->next;
    while (cur != head)
    {
        PlayListNode *temp = cur;
        cur = cur->next;
        delete temp;
    }
    delete head;
    head = nullptr;
    cursor = nullptr;
}

void PlayList::insert_node(const string &artist, const string &title, int runtime_sec)
{
    // 중복 검사
    if (head)
    {
        PlayListNode *cur = head;
        do
        {
            if (cur->equals(artist, title))
            {
                return; // 중복되는 노래 존재
            }
            cur = cur->next;
        } while (cur != head);
    }

    // 새 노드 생성
    PlayListNode *newNode = new PlayListNode(artist, title, runtime_sec);

    // 처음 삽입할 경우
    if (!head)
    {
        head = newNode;
        cursor = newNode;
        newNode->next = newNode;
        newNode->prev = newNode;
    }
    else
    {
        PlayListNode *tail = head->prev;

        tail->next = newNode;
        newNode->prev = tail;

        newNode->next = head;
        head->prev = newNode;
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
        return true;
    }
    else
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

    if (!head)
    {
        return "";
    }
    PlayListNode *cur = head;

    do
    {
        int rt = cur->getRunTimeSec();
        int mm = rt / 60;
        int ss_sec = rt % 60;

        ss << cur->getArtist() << "/" << cur->getTitle() << "/";
        ss << setfill('0') << setw(2) << mm << ":"
           << setfill('0') << setw(2) << ss_sec << "\n";

        cur = cur->next;
    } while (cur != head);

    return ss.str();
}

int PlayList::run_time()
{
    return time;
}

int PlayList::size() const
{
    return count;
}

bool PlayList::deleteArtist(const string &artist)
{
    if (!head)
    {
        return false;
    }

    PlayListNode *cur = head;
    bool deleted = false;

    do
    {
        PlayListNode *nextNode = cur->next; // 순회 유지용

        if (cur->artist == artist)
        {
            // 노드가 1개 뿐인 경우
            if (cur->next == cur && cur->prev == cur)
            {
                delete cur;
                head = nullptr;
                cursor = nullptr;
                count = 0;
                time = 0;
                return true;
            }

            // 연결 끊기
            cur->prev->next = cur->next;
            cur->next->prev = cur->prev;

            // head나 cursor가 삭제 대상이면 옮겨주기
            if (cur == head)
            {
                head = cur->next;
            }
            if (cur == cursor)
            {
                cursor = cur->next;
            }

            // 시간/카운트 갱신
            time -= cur->runtime_sec;
            count--;

            delete cur;
            deleted = true;
        }
        cur = nextNode;
    } while (cur != head);

    // 삭제 후 head가 마지막 노드였다가 삭제된 경우
    if (count == 0)
    {
        head = nullptr;
        cursor = nullptr;
    }

    return deleted;
}

bool PlayList::deleteTitle(const string &title)
{
    if (!head)
    {
        return false;
    }

    PlayListNode *cur = head;
    bool deleted = false;

    do
    {
        PlayListNode *nextNode = cur->next; // 순회 유지

        if (cur->title == title)
        {
            // 노드가 하나뿐인 경우
            if (cur->next == cur && cur->prev == cur)
            {
                delete cur;
                head = nullptr;
                cursor = nullptr;
                count = 0;
                time = 0;
                return true;
            }

            // 연결 해제
            cur->prev->next = cur->next;
            cur->next->prev = cur->prev;

            // head나 cursor가 삭제 대상일 경우
            if (cur == head)
            {
                head = cur->next;
            }
            if (cur == cursor)
            {
                cursor = cur->next;
            }

            time -= cur->runtime_sec;
            count--;

            delete cur;
            deleted = true;
        }
        cur = nextNode;

    } while (cur != head);

    // 마지막 노드가 삭제되어 리스트가 비어있는지 체크
    if (count == 0)
    {
        head = nullptr;
        cursor = nullptr;
    }

    return deleted;
}

bool PlayList::deleteFromList(const string &artist, const string &title)
{
    if (!head)
    {
        return false;
    }

    PlayListNode *cur = head;
    do
    {
        PlayListNode *nextNode = cur->next;
        // 아티스트, 제목 둘 다 일치하는 노드를 찾았을 경우
        if (cur->artist == artist && cur->title == title)
        {
            // 노드가 1개뿐인 경우
            if (cur->next == cur && cur->prev == cur)
            {
                time -= cur->runtime_sec;
                count--;
                delete cur;
                head = nullptr;
                cursor = nullptr;
                return true;
            }

            // 노드 연결 해제
            cur->prev->next = cur->next;
            cur->next->prev = cur->prev;

            // head나 cursor가 삭제 대상이면 이동
            if (cur == head)
            {
                head = cur->next;
            }
            if (cur == cursor)
            {
                cursor = cur->next;
            }

            time -= cur->runtime_sec;
            count--;

            delete cur;
            return true;
        }

        cur = nextNode;
    } while (cur != head);

    // 해당 곡이 없을 경우
    return false;
}

bool PlayList::is_existTitle(const string &title)
{
    if (!head)
    {
        return false;
    }

    PlayListNode *cur = head;
    do
    {
        if (cur->getTitle() == title)
        {
            return true;
        }
        cur = cur->next;
    } while (cur != head);

    return false;
}

bool PlayList::is_existArtist(const string &artist)
{
    if (!head)
    {
        return false;
    }

    PlayListNode *cur = head;
    do
    {
        if (cur->getArtist() == artist)
        {
            return true;
        }
        cur = cur->next;
    } while (cur != head);

    return false;
}