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

void TitleBST::insert(MusicQueueNode *node)
{
    // 1. 값 꺼내기
    string title = node->getTitle();
    string artist = node->getArtist();
    string run_time = node->getRunTime();

    // 2. root가 비어있으면 새 노드 생성
    if (!root)
    {
        root = new TitleBSTNode(title, artist, run_time);
        return;
    }

    TitleBSTNode *cur = root;

    while (true)
    {
        if (title == cur->title)
        {
            // 3. 같은 제목이면 해당 노드에 artist, run_time 추가
            cur->artist.push_back(artist);
            cur->run_time.push_back(run_time);
            cur->count++;
            return;
        }
        else if (title < cur->title)
        {
            // 4. 왼쪽으로 탐색
            if (!cur->left)
            {
                cur->left = new TitleBSTNode(title, artist, run_time);
                return;
            }
            cur = cur->left;
        }
        else
        {
            // 5. 오른쪽으로 탐색
            if (!cur->right)
            {
                cur->right = new TitleBSTNode(title, artist, run_time);
                return;
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
    flog << node->title << " (" << node->count << " songs)\n";
    for (int i = 0; i < node->count; i++)
    {
        flog << "  └ " << node->artist[i] << " | " << node->run_time[i] << '\n';
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

bool TitleBST::searchTitle(const string &title, ofstream &flog)
{
    TitleBSTNode *cur = root;

    while (cur)
    {
        // 찾았을 경우
        if (title == cur->title)
        {
            flog << cur->title << " (" << cur->count << " songs)\n";
            for (int i = 0; i < cur->count; i++)
            {
                flog << "  └ " << cur->artist[i] << " | " << cur->run_time[i] << '\n';
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
    TitleBSTNode *cur = root;
    TitleBSTNode *parent = nullptr;

    while (cur)
    {
        // artist가 존재하는지 확인
        bool found = false;
        for (int i = 0; i < cur->count; i++)
        {
            if (cur->artist[i] == artist)
            {
                found = true;
                break;
            }
        }

        if (found)
        {
            // 1. 해당 artist에 해당하는 곡만 삭제
            for (int i = 0; i < cur->count; i++)
            {
                if (cur->artist[i] == artist)
                {
                    cur->artist.erase(cur->artist.begin() + i);
                    cur->run_time.erase(cur->run_time.begin() + i);
                    cur->count--;
                    i--;
                }
            }

            // 2. 삭제 후 노드 내 곡이 하나도 없다면 노드 삭제
            if (cur->count == 0)
            {
                // case 1: 리프 노드일 경우
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
                }

                // case2: 왼쪽 자식만 있을 경우
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

                // case 3: 오른쪽 자식만 있을 경우
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

                // case 4: 양쪽 자신이 존재할 경우
                else
                {
                    TitleBSTNode *replacementParent = cur;
                    TitleBSTNode *replacementNode = cur->right;
                    while (replacementNode->left)
                    {
                        replacementParent = replacementNode;
                        replacementNode = replacementNode->left;
                    }

                    // 후계자 데이터 복사
                    cur->title = replacementNode->title;
                    cur->artist = replacementNode->artist;
                    cur->run_time = replacementNode->run_time;
                    cur->count = replacementNode->count;

                    // 후계자 제거
                    if (replacementParent->left == replacementNode)
                    {
                        replacementParent->left = replacementNode->right;
                    }
                    else
                    {
                        replacementParent->right = replacementNode->right;
                    }
                    delete replacementNode;
                }
            }

            // 삭제 성공
            return true;
        }

        // 다음 노드로 이동
        parent = cur;
        if (artist < cur->artist[0])
        {
            cur = cur->left;
        }
        else
        {
            cur = cur->right;
        }
    }
    return false;
}