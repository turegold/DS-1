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

bool ArtistBST::insert(MusicQueueNode *node)
{
    string artist = node->getArtist();
    string title = node->getTitle();
    string run_time = node->getRunTime();

    // 루트가 비어있을 경우 새로운 노드를 생성하여 루트로 설정
    if (!root)
    {
        root = new ArtistBSTNode(artist, title, run_time);
        return true;
    }

    ArtistBSTNode *cur = root;

    while (true)
    {
        if (artist == cur->getArtist())
        {
            // 중복 곡 검사
            for (int i = 0; i < cur->title.size(); i++)
            {
                if (cur->title[i] == title)
                {
                    return false;
                }
            }

            // 해당 아티스트가 이미 있는 경우: 노래 추가
            cur->title.push_back(title);
            cur->run_time.push_back(run_time);
            cur->count++;
            return true;
        }
        else if (artist < cur->getArtist())
        {
            // 왼쪽 서브트리로 이동
            if (!cur->left)
            {
                cur->left = new ArtistBSTNode(artist, title, run_time);
                return true;
            }
            cur = cur->left;
        }
        else
        {
            // 오른쪽 서브트리로 이동
            if (!cur->right)
            {
                cur->right = new ArtistBSTNode(artist, title, run_time);
                return true;
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
    for (int i = 0; i < node->count; i++)
    {
        flog << node->artist << "/" << node->title[i] << "/" << node->run_time[i] << '\n';
    }

    printNode(node->right, flog);
}

void ArtistBST::printTree(ofstream &flog)
{
    printNode(root, flog);
}

bool ArtistBST::searchArtist(const string &artist, ofstream &flog, bool is_print)
{
    ArtistBSTNode *cur = root;

    while (cur)
    {
        // 찾은 경우
        if (artist == cur->artist)
        {
            if (is_print)
            {
                for (int i = 0; i < cur->count; i++)
                {
                    flog << cur->artist << "/" << cur->title[i] << "/" << cur->run_time[i] << "\n";
                }
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

bool ArtistBST::searchSong(const string &artist, const string &title, ofstream &flog, bool is_print)
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
                    if (is_print)
                    {
                        flog << cur->artist << "/" << cur->title[i] << "/" << cur->run_time[i] << "\n";
                    }
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
    // 플레이리스트에 같은 가수가 있는지 확인
    if (pl.is_existArtist(artist))
    {
        return false;
    }
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

ArtistBSTNode *ArtistBST::deleteArtistRecursive(ArtistBSTNode *node, const string &artist, bool &deleted)
{
    if (!node)
    {
        return nullptr;
    }

    if (artist < node->artist)
    {
        node->left = deleteArtistRecursive(node->left, artist, deleted);
    }
    else if (artist > node->artist)
    {
        node->right = deleteArtistRecursive(node->right, artist, deleted);
    }
    else
    {
        // 삭제 대상 노드 발견
        deleted = true;

        // 자식이 없는 경우
        if (!node->left && !node->right)
        {
            delete node;
            return nullptr;
        }

        // 오른쪽 자식만 있는 경우
        else if (!node->left)
        {
            ArtistBSTNode *temp = node->right;
            delete node;
            return temp;
        }
        // 왼쪽 자식만 있는 경우
        else if (!node->right)
        {
            ArtistBSTNode *temp = node->left;
            delete node;
            return temp;
        }

        // 자식이 둘 다 있는 경우
        ArtistBSTNode *replacementParent = node;
        ArtistBSTNode *replacement = node->right;

        // 오른쪽 트리에서 제일 왼쪽 노드를 찾음
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

        if (replacementParent == node)
        {
            replacementParent->right = replacement->right;
        }
        else
        {
            replacementParent->left = replacement->right;
        }

        delete replacement;
    }
    return node;
}

bool ArtistBST::deleteArtist(const string &artist)
{
    bool deleted = false;
    root = deleteArtistRecursive(root, artist, deleted);

    return deleted;
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

bool ArtistBST::deleteSong(const string &artist, const string &title)
{
    ArtistBSTNode *cur = root;
    ArtistBSTNode *parent = nullptr;

    // BST에서 가수 노드 탐색
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

    // 가수를 못 찾은 경우
    if (!cur)
    {
        return false;
    }

    // 해당 가수의 title에서 제목 찾아서 삭제
    for (int i = 0; i < cur->count; i++)
    {
        if (cur->title[i] == title)
        {
            cur->title.erase(cur->title.begin() + i);
            cur->run_time.erase(cur->run_time.begin() + i);
            cur->count--;
            // 노드가 비었으면 BST에서 artist 삭제
            if (cur->count == 0)
            {
                deleteArtist(artist);
            }
            return true;
        }
    }
    // 노래를 못 찾은 경우
    return false;
}