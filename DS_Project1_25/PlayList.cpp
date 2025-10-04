#include "PlayList.h"
#include "PlayListNode.h"
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace std;

// Constructor
PlayList::PlayList()
{
    head = nullptr;
    cursor = nullptr;
    count = 0;
    time = 0;
}

// Destructor
PlayList::~PlayList()
{
    if (!head)
    {
        return;
    }

    // Traverse and delete all nodes
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

// Inserts a new node into the playlist
void PlayList::insert_node(const string &artist, const string &title, int runtime_sec)
{
    // Check for duplication
    if (head)
    {
        PlayListNode *cur = head;
        do
        {
            // Duplicate found, do not insert
            if (cur->equals(artist, title))
            {
                return;
            }
            cur = cur->next;
        } while (cur != head);
    }

    PlayListNode *newNode = new PlayListNode(artist, title, runtime_sec);

    // First insertion
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

// Returns true if the playlist is empty
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

// Returns true if the playlist is full
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

// Check existence of any node
bool PlayList::exist(const string &artist, const string &title)
{
    if (!head)
    {
        return false;
    }

    PlayListNode *cur = head;
    do
    {
        if (cur->artist == artist && cur->title == title)
        {
            return true;
        }
        cur = cur->next;
    } while (cur != head);

    return false;
}

// Returns the formatted string of the playlist for logging
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
        ss << mm << ":" << setfill('0') << setw(2) << ss_sec << "\n";

        cur = cur->next;
    } while (cur != head);

    return ss.str();
}

// Returns total runtime of the playlist
int PlayList::run_time()
{
    return time;
}

// Returns number of songs in the playlist
int PlayList::size() const
{
    return count;
}

// Deletes all songs by the given artist
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
        PlayListNode *nextNode = cur->next;

        if (cur->artist == artist)
        {

            // Only one node in the list
            if (cur->next == cur && cur->prev == cur)
            {
                delete cur;
                head = nullptr;
                cursor = nullptr;
                count = 0;
                time = 0;
                return true;
            }

            // Remove current node from the list
            cur->prev->next = cur->next;
            cur->next->prev = cur->prev;

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

    if (count == 0)
    {
        head = nullptr;
        cursor = nullptr;
    }

    return deleted;
}

// Deletes all songs with the given title
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
        PlayListNode *nextNode = cur->next;

        if (cur->title == title)
        {
            // Only one node in the list
            if (cur->next == cur && cur->prev == cur)
            {
                delete cur;
                head = nullptr;
                cursor = nullptr;
                count = 0;
                time = 0;
                return true;
            }

            // Remove current node from the list
            cur->prev->next = cur->next;
            cur->next->prev = cur->prev;

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

    if (count == 0)
    {
        head = nullptr;
        cursor = nullptr;
    }

    return deleted;
}

// Delete a specific song from the playlist
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
        // If artist and title all founded
        if (cur->artist == artist && cur->title == title)
        {
            // Only one node in the list
            if (cur->next == cur && cur->prev == cur)
            {
                time -= cur->runtime_sec;
                count--;
                delete cur;
                head = nullptr;
                cursor = nullptr;
                return true;
            }

            // Remove current node from the list
            cur->prev->next = cur->next;
            cur->next->prev = cur->prev;

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

    return false;
}

// Checks if any song with the given title exists
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

// Checks if any song with the given artist exists
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