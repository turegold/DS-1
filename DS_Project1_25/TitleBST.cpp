#include "TitleBST.h"
#include <iostream>
#include <fstream>
using namespace std;

TitleBST::TitleBST()
{
    root = nullptr;
}

void TitleBST::destroyTree(TitleBSTNode *node)
{
    if (!node)
    {
        return;
    }
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}

TitleBST::~TitleBST()
{
    // 재귀적으로 노드 삭제
    destroyTree(root);
}

bool TitleBST::insert(MusicQueueNode *node)
{
    // 1. 값 꺼내기
    string title = node->getTitle();
    string artist = node->getArtist();
    string run_time = node->getRunTime();

    // 2. root가 비어있으면 새 노드 생성
    if (!root)
    {
        root = new TitleBSTNode(title, artist, run_time);
        return true;
    }

    TitleBSTNode *cur = root;

    while (true)
    {
        if (title == cur->title)
        {
            // 중복 가수 검사
            for (int i = 0; i < cur->artist.size(); i++)
            {
                if (cur->artist[i] == artist)
                {
                    return false;
                }
            }
            // 3. 같은 제목이면 해당 노드에 artist, run_time 추가
            cur->artist.push_back(artist);
            cur->run_time.push_back(run_time);
            cur->count++;
            return true;
        }
        else if (title < cur->title)
        {
            // 4. 왼쪽으로 탐색
            if (!cur->left)
            {
                cur->left = new TitleBSTNode(title, artist, run_time);
                return true;
            }
            cur = cur->left;
        }
        else
        {
            // 5. 오른쪽으로 탐색
            if (!cur->right)
            {
                cur->right = new TitleBSTNode(title, artist, run_time);
                return true;
            }
            cur = cur->right;
        }
    }
}

void TitleBST::printNode(TitleBSTNode *node, ofstream &flog)
{
    if (!node)
    {
        return;
    }

    printNode(node->left, flog);
    for (int i = 0; i < node->count; i++)
    {
        flog << node->artist[i] << "/" << node->title << "/" << node->run_time[i] << '\n';
    }

    printNode(node->right, flog);
}

void TitleBST::search()
{
}

void TitleBST::print()
{
}

void TitleBST::delete_node()
{
}

bool TitleBST::isEmpty() const
{
    if (root == nullptr)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void TitleBST::printTree(ofstream &flog)
{
    printNode(root, flog);
}

bool TitleBST::searchTitle(const string &title, ofstream &flog, bool is_print)
{
    TitleBSTNode *cur = root;

    while (cur)
    {
        // 찾았을 경우
        if (title == cur->title)
        {
            if (is_print)
            {
                for (int i = 0; i < cur->count; i++)
                {
                    flog << cur->artist[i] << "/" << cur->title << "/" << cur->run_time[i] << "\n";
                }
            }
            return true;
        }
        else if (title < cur->title)
        {
            cur = cur->left;
        }
        else
        {
            cur = cur->right;
        }
    }

    // 못 찾은 경우
    return false;
}

bool TitleBST::searchTitleToPlayList(const string &title, PlayList &pl, ofstream &flog)
{
    // 플레이리스트에 같은 제목이 있는지 확인
    if (pl.is_existTitle(title))
    {
        return false;
    }

    TitleBSTNode *cur = root;
    // BST 탐색
    while (cur)
    {
        // 찾은 경우
        if (title == cur->title)
        {
            // 곡 수 초과 체크
            if (pl.size() + cur->count > 10)
            {
                return false;
            }
            for (int i = 0; i < cur->count; i++)
            {
                // 초 변환
                string time_str = cur->run_time[i];
                size_t colon = time_str.find(':');
                int minutes = stoi(time_str.substr(0, colon));
                int seconds = stoi(time_str.substr(colon + 1));
                int runtime_sec = minutes * 60 + seconds;

                // PlayList에 삽입
                pl.insert_node(cur->artist[i], title, runtime_sec);
            }
            return true;
        }
        else if (title < cur->title)
        {
            cur = cur->left;
        }
        else
        {
            cur = cur->right;
        }
    }

    // 못 찾은 경우
    return false;
}

bool TitleBST::deleteArtist(const string &artist)
{
    bool deleted = false;
    root = deleteArtistRecursive(root, artist, deleted);
    return deleted;
}

TitleBSTNode *TitleBST::deleteArtistRecursive(TitleBSTNode *node, const string &artist, bool &deleted)
{
    if (!node)
    {
        return nullptr;
    }

    // 왼쪽 서브트리 처리
    node->left = deleteArtistRecursive(node->left, artist, deleted);

    // 오른쪽 서브트리 처리
    node->right = deleteArtistRecursive(node->right, artist, deleted);

    // 현재 노드에서 artist 삭제
    bool found = false;
    for (int i = 0; i < node->count; i++)
    {
        if (node->artist[i] == artist)
        {
            node->artist.erase(node->artist.begin() + i);
            node->run_time.erase(node->run_time.begin() + i);
            node->count--;
            i--;
            found = true;
            deleted = true;
        }
    }

    // 삭제 후 노드가 비었으면 노드 자체를 삭제
    if (node->count == 0)
    {
        // case1: 리프 노드일 경우
        if (!node->left && !node->right)
        {
            delete node;
            return nullptr;
        }

        // case2: 왼쪽만 있을 경우
        if (node->left && !node->right)
        {
            TitleBSTNode *temp = node->left;
            delete node;
            return temp;
        }

        // case3: 오른쪽에만 있을 경우
        if (!node->left && node->right)
        {
            TitleBSTNode *temp = node->right;
            delete node;
            return temp;
        }

        // case4: 양쪽에 모두 있을 경우
        TitleBSTNode *replacementParent = node;
        TitleBSTNode *replacement = node->right;

        while (replacement->left)
        {
            replacementParent = replacement;
            replacement = replacement->left;
        }

        // 후계자 정보 복사
        node->title = replacement->title;
        node->artist = replacement->artist;
        node->run_time = replacement->run_time;
        node->count = replacement->count;

        // 후계자 노드 삭제

        // 1. 지울 노드의 바로 오른쪽에 후계자가 있을 때
        if (replacementParent == node)
            replacementParent->right = replacement->right;

        // 2. 깊숙한 곳에 후계자가 있을 때
        else
            replacementParent->left = replacement->right;
        delete replacement;
    }
    return node;
}

bool TitleBST::deleteTitle(const string &title)
{
    TitleBSTNode *cur = root;
    TitleBSTNode *parent = nullptr;

    // 1. 노드 탐색
    while (cur && cur->title != title)
    {
        parent = cur;
        if (title < cur->title)
        {
            cur = cur->left;
        }
        else
        {
            cur = cur->right;
        }
    }

    // 2. 못 찾은 경우
    if (!cur)
    {
        return false;
    }

    // 3. 삭제

    // case1: 리프 노드일 경우
    if (!cur->left && !cur->right)
    {
        // 루트 노드일 경우
        if (cur == root)
        {
            root = nullptr;
        }
        // 삭제할 노드가 부모의 왼쪽일 경우
        else if (parent->left == cur)
        {
            parent->left = nullptr;
        }
        // 삭제할 노드가 부모의 오른쪽일 경우
        else
        {
            parent->right = nullptr;
        }

        delete cur;
    }

    // case 2: 왼쪽 자식만 존재할 경우
    else if (cur->left && !cur->right)
    {
        // 루트 노드일 경우
        if (cur == root)
        {
            root = cur->left;
        }
        // 삭제할 노드가 부모의 왼쪽일 경우
        else if (parent->left == cur)
        {
            parent->left = cur->left;
        }
        // 삭제할 노드가 부모의 오른쪽일 경우
        else
        {
            parent->right = cur->left;
        }
        delete cur;
    }

    // case 3: 오른쪽 자식만 존재할 경우
    else if (!cur->left && cur->right)
    {
        if (cur == root)
        {
            root = cur->right;
        }
        else if (parent->left == cur)
        {
            parent->left = cur->right;
        }
        else
        {
            parent->right = cur->right;
        }
        delete cur;
    }

    // case 4: 양쪽 모두 존재할 경우
    else
    {
        TitleBSTNode *replacementParent = cur;
        TitleBSTNode *replacement = cur->right;
        while (replacement->left)
        {
            replacementParent = replacement;
            replacement = replacement->left;
        }

        // 복사
        cur->title = replacement->title;
        cur->artist = replacement->artist;
        cur->run_time = replacement->run_time;
        cur->count = replacement->count;

        // 후계자 제거
        if (replacementParent == cur)
            replacementParent->right = replacement->right;
        else
            replacementParent->left = replacement->right;

        delete replacement;
    }

    return true;
}

bool TitleBST::deleteSong(const string &artist, const string &title)
{
    TitleBSTNode *cur = root;

    // 제목 노드 탐색
    while (cur && cur->title != title)
    {
        if (title < cur->title)
        {
            cur = cur->left;
        }
        else
        {
            cur = cur->right;
        }
    }

    // 못 찾은 경우
    if (!cur)
    {
        return false;
    }

    // 해당 아티스트의 노래 삭제
    for (int i = 0; i < cur->count; i++)
    {
        if (cur->artist[i] == artist)
        {
            cur->artist.erase(cur->artist.begin() + i);
            cur->run_time.erase(cur->run_time.begin() + i);
            cur->count--;

            // 노드가 비어있다면 노드 삭제
            if (cur->count == 0)
            {
                deleteTitle(title);
            }

            return true;
        }
    }

    // 아티스트를 못 찾은 경우
    return false;
}