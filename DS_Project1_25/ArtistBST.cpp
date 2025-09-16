#include "ArtistBST.h"
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
