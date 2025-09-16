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