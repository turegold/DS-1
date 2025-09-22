#include "ArtistBST.h"
#include "ArtistBSTNode.h"
#include <iostream>
#include <fstream>
using namespace std;

ArtistBST::ArtistBST()
{
    root = nullptr;
}

void ArtistBST::destroyTree(ArtistBSTNode *node)
{
    if (node == nullptr)
    {
        return;
    }

    // 좌, 우 자식 먼저 제거
    destroyTree(node->left);
    destroyTree(node->right);

    // 현재 노드 제거
    delete node;
}
ArtistBST::~ArtistBST()
{
    // 재귀적으로 노드를 제거
    destroyTree(root);
}

void ArtistBST::insert(MusicQueueNode *node)
{
    string artist = node->getArtist();
    string title = node->getTitle();
    string run_time = node->getRunTime();

    // 루트가 비어있을 경우 새로운 노드를 생성하여 루트로 설정
    if (!root)
    {
        root = new ArtistBSTNode(artist, title, run_time);
        return;
    }

    ArtistBSTNode *cur = root;

    while (true)
    {
        if (artist == cur->getArtist())
        {
            // 해당 아티스트가 이미 있는 경우: 노래 추가
            cur->title.push_back(title);
            cur->run_time.push_back(run_time);
            cur->count++;
            return;
        }
        else if (artist < cur->getArtist())
        {
            // 왼쪽 서브트리로 이동
            if (!cur->left)
            {
                cur->left = new ArtistBSTNode(artist, title, run_time);
                return;
            }
            cur = cur->left;
        }
        else
        {
            // 오른쪽 서브트리로 이동
            if (!cur->right)
            {
                cur->right = new ArtistBSTNode(artist, title, run_time);
                return;
            }
            cur = cur->right;
        }
    }
}

void ArtistBST::search()
{
}

void ArtistBST::print()
{
}

void ArtistBST::delete_node()
{
}

bool ArtistBST::isEmpty() const
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

void ArtistBST::printNode(ArtistBSTNode *node, ofstream &flog)
{
    if (!node)
    {
        return;
    }

    printNode(node->left, flog);
    flog << node->artist << " (" << node->count << " songs)\n";
    for (int i = 0; i < node->count; i++)
    {
        flog << "  └ " << node->title[i] << " | " << node->run_time[i] << '\n';
    }

    printNode(node->right, flog);
}

void ArtistBST::printTree(ofstream &flog)
{
    printNode(root, flog);
}

bool ArtistBST::searchArtist(const string &artist, ofstream &flog)
{
    ArtistBSTNode *cur = root;

    while (cur)
    {
        // 찾은 경우
        if (artist == cur->artist)
        {
            flog << cur->artist << " (" << cur->count << " songs)\n";
            for (int i = 0; i < cur->count; i++)
            {
                flog << "  └ " << cur->title[i] << " | " << cur->run_time[i] << '\n';
            }
            return true;
        }
        else if (artist < cur->artist)
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

bool ArtistBST::searchSong(const string &artist, const string &title, ofstream &flog)
{
    ArtistBSTNode *cur = root;

    while (cur)
    {
        // 아티스트 찾음
        if (artist == cur->artist)
        {
            for (int i = 0; i < cur->count; i++)
            {
                // 제목 찾음
                if (cur->title[i] == title)
                {
                    flog << cur->artist << '\n';
                    flog << "  └ " << cur->title[i] << " | " << cur->run_time[i] << '\n';
                    return true;
                }
            }

            // 아티스트는 맞는데 제목이 없는 경우
        }
        else if (artist < cur->artist)
        {
            cur = cur->left;
        }
        else
        {
            cur = cur->right;
        }
    }

    // 아티스트 자체를 못 찾은 경우
    return false;
}

bool ArtistBST::searchArtistToPlayList(const string &artist, PlayList &pl, ofstream &flog)
{
    ArtistBSTNode *cur = root;

    // BST 탐색
    while (cur)
    {
        // 아티스트를 찾은 경우
        if (artist == cur->artist)
        {
            // 곡 수 초과 체크
            if (pl.size() + cur->count > 10)
            {
                return false;
            }

            for (int i = 0; i < cur->count; i++)
            {
                // 러닝타임 문자열 -> 초로 변환
                string time_str = cur->run_time[i];
                size_t colon = time_str.find(':');
                int minutes = stoi(time_str.substr(0, colon));
                int seconds = stoi(time_str.substr(colon + 1));
                int runtime_sec = minutes * 60 + seconds;

                // PlayList에 삽입
                pl.insert_node(artist, cur->title[i], runtime_sec);
            }

            return true;
        }
        else if (artist < cur->artist)
        {
            cur = cur->left;
        }
        else
        {
            cur = cur->right;
        }
    }

    // 아티스트를 못 찾았을 경우
    return false;
}

bool ArtistBST::searchSongToPlayList(const string &artist, const string &title, PlayList &pl, ofstream &flog)
{
    ArtistBSTNode *cur = root;

    // BST 탐색
    while (cur)
    {
        // artist를 찾았을 경우
        if (artist == cur->artist)
        {

            // 찾은 artist에서 title 검색
            for (int i = 0; i < cur->count; i++)
            {

                // 제목까지 찾았을 경우
                if (cur->title[i] == title)
                {
                    // 곡 수 초과 체크
                    if (pl.size() + 1 > 10)
                    {
                        return false;
                    }

                    // 러닝타임 문자열 -> 초로 변환
                    string time_str = cur->run_time[i];
                    size_t colon = time_str.find(':');
                    int minutes = stoi(time_str.substr(0, colon));
                    int seconds = stoi(time_str.substr(colon + 1));
                    int runtime_sec = minutes * 60 + seconds;

                    // PlayList에 삽입
                    pl.insert_node(artist, title, runtime_sec);
                    return true;
                }
            }

            // artist는 맞지만 title이 없는 경우
            return false;
        }
        else if (artist < cur->artist)
        {
            cur = cur->left;
        }
        else
        {
            cur = cur->right;
        }
    }

    // artist 자체를 못 찾았을 경우
    return false;
}

bool ArtistBST::deleteArtist(const string &artist)
{
    ArtistBSTNode *cur = root;
    ArtistBSTNode *parent = nullptr;

    // 1. 삭제할 노드 탐색
    while (cur && cur->artist != artist)
    {
        parent = cur;
        if (artist < cur->artist)
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

    // 3. 삭제 로직
    // case 1: 자식이 없을 경우
    if (!cur->left && !cur->right)
    {
        if (cur == root)
        {
            root = nullptr;
        }
        else if (parent->left == cur)
        {
            parent->left = nullptr;
        }
        else
        {
            parent->right = nullptr;
        }
        delete cur;
    }

    // case 2-1: 자식이 왼쪽에만 있을 경우
    else if (cur->left && !cur->right)
    {
        if (cur == root)
        {
            root = cur->left;
        }
        else if (parent->left == cur)
        {
            parent->left = cur->left;
        }
        else
        {
            parent->right = cur->left;
        }
        delete cur;
    }

    // case 2-2: 자식이 오른쪽에만 있을 경우
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

    // case 3: 자식이 왼쪽, 오른쪽에 모두 있을 경우
    else
    {
        ArtistBSTNode *replacementParent = cur;
        ArtistBSTNode *replacementNode = cur->right;
        while (replacementNode->left)
        {
            replacementParent = replacementNode;
            replacementNode = replacementNode->left;
        }

        // 현재 노드에 후계자의 데이터를 복사
        cur->artist = replacementNode->artist;
        cur->title = replacementNode->title;
        cur->run_time = replacementNode->run_time;
        cur->count = replacementNode->count;

        // 후계자 삭제
        if (replacementParent->left == replacementNode)
        {
            replacementParent->left = replacementNode->right;
        }
        /*
        이런 경우
         50
           \
            60
              \
               70
        */
        else
        {
            replacementParent->right = replacementNode->right;
        }

        delete replacementNode;
    }

    return true;
}

ArtistBSTNode *ArtistBST::deleteTitleRecursive(ArtistBSTNode *node, const string &title, bool &deleted)
{
    if (!node)
    {
        return nullptr;
    }

    // 왼쪽 재귀
    node->left = deleteTitleRecursive(node->left, title, deleted);

    // 오른쪽 재귀
    node->right = deleteTitleRecursive(node->right, title, deleted);

    // 현재 노드에서 title 삭제
    for (int i = 0; i < node->count; i++)
    {
        if (node->title[i] == title)
        {
            node->title.erase(node->title.begin() + i);
            node->run_time.erase(node->run_time.begin() + i);
            node->count--;
            i--;
            deleted = true;
        }
    }

    // title을 삭제했는데 count가 0일 경우
    if (node->count == 0)
    {
        // case1: 리프노드일 경우
        if (!node->left && !node->right)
        {
            delete node;
            return nullptr;
        }
        // case2: 왼쪽 자식만 있을 경우
        else if (node->left && !node->right)
        {
            ArtistBSTNode *temp = node->left;
            delete node;
            return temp;
        }
        // case3: 오른쪽 자식만 있을 경우
        else if (!node->left && node->right)
        {
            ArtistBSTNode *temp = node->right;
            delete node;
            return temp;
        }

        // case4: 양쪽 모두 있을 경우
        else
        {
            ArtistBSTNode *replacementParent = node;
            ArtistBSTNode *replacement = node->right;

            while (replacement->left)
            {
                replacementParent = replacement;
                replacement = replacement->left;
            }

            // 데이터 복사
            node->artist = replacement->artist;
            node->title = replacement->title;
            node->run_time = replacement->run_time;
            node->count = replacement->count;

            // 후계자 삭제
            if (replacementParent == node)
                replacementParent->right = replacement->right;
            else
                replacementParent->left = replacement->right;
            delete replacement;
        }
    }

    return node;
}

bool ArtistBST::deleteTitle(const string &title)
{
    bool deleted = false;
    root = deleteTitleRecursive(root, title, deleted);
    return deleted;
}