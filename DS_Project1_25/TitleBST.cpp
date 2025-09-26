#include "TitleBST.h"
#include "TitleBSTNode.h"
#include "PlayList.h"
#include <iostream>
#include <fstream>
using namespace std;

// Constructor
TitleBST::TitleBST()
{
    root = nullptr;
}

// Helper to recursively delete all nodes
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

// Destructor
TitleBST::~TitleBST()
{
    destroyTree(root);
}

// Insert a new MusicQueueNode
bool TitleBST::insert(MusicQueueNode *node)
{
    // Extract data from the incoming node
    string title = node->getTitle();
    string artist = node->getArtist();
    string run_time = node->getRunTime();

    // If the BST if empty, create the root node
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
            // Check if this artist already has the same title
            for (int i = 0; i < cur->artist.size(); i++)
            {
                if (cur->artist[i] == artist)
                {
                    return false;
                }
            }
            // If same title exists, append new artist and run_time
            cur->artist.push_back(artist);
            cur->run_time.push_back(run_time);
            cur->count++;
            return true;
        }
        // If title is smaller
        else if (title < cur->title)
        {
            if (!cur->left)
            {
                cur->left = new TitleBSTNode(title, artist, run_time);
                return true;
            }
            cur = cur->left;
        }
        // If title is larger
        else
        {
            if (!cur->right)
            {
                cur->right = new TitleBSTNode(title, artist, run_time);
                return true;
            }
            cur = cur->right;
        }
    }
}

// Helper to print all nodes in-order
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

// Check whether the tree is empty
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

// Print entire tree in-order
void TitleBST::printTree(ofstream &flog)
{
    printNode(root, flog);
}

// Search for a title in the BST and optionally print its data
bool TitleBST::searchTitle(const string &title, ofstream &flog, bool is_print)
{
    TitleBSTNode *cur = root;

    // Traverse the BST to find the node with matching title
    while (cur)
    {
        if (title == cur->title)
        {
            // If print is required
            if (is_print)
            {
                for (int i = 0; i < cur->count; i++)
                {
                    flog << cur->artist[i] << "/" << cur->title << "/" << cur->run_time[i] << "\n";
                }
            }
            return true;
        }
        // If title is smaller
        else if (title < cur->title)
        {
            cur = cur->left;
        }
        // If title is larger
        else
        {
            cur = cur->right;
        }
    }

    // If not found
    return false;
}

// Search a title and add it to PlayList if found
bool TitleBST::searchTitleToPlayList(const string &title, PlayList &pl, ofstream &flog)
{
    // Check if the title already exists in the playlist
    if (pl.is_existTitle(title))
    {
        return false;
    }

    TitleBSTNode *cur = root;
    // Traverse the BST to find the node with matching title
    while (cur)
    {
        if (title == cur->title)
        {
            // Check if adding all songs exceeds the playlist limit
            if (pl.size() + cur->count > 10)
            {
                return false;
            }

            // Insert each song into the playlist
            for (int i = 0; i < cur->count; i++)
            {
                // Convert runtime from "mm:ss" format to seconds
                string time_str = cur->run_time[i];
                size_t colon = time_str.find(':');
                int minutes = stoi(time_str.substr(0, colon));
                int seconds = stoi(time_str.substr(colon + 1));
                int runtime_sec = minutes * 60 + seconds;

                // Add the song to the playlist
                pl.insert_node(cur->artist[i], title, runtime_sec);
            }
            return true;
        }
        // If title is smaller
        else if (title < cur->title)
        {
            cur = cur->left;
        }
        // If title is larger
        else
        {
            cur = cur->right;
        }
    }

    // If not found
    return false;
}

// Delete all songs by a specific artist
bool TitleBST::deleteArtist(const string &artist)
{
    bool deleted = false;
    root = deleteArtistRecursive(root, artist, deleted);

    return deleted;
}

// Helper to delete all songs by a specific artist
TitleBSTNode *TitleBST::deleteArtistRecursive(TitleBSTNode *node, const string &artist, bool &deleted)
{
    // If node not found
    if (!node)
    {
        return nullptr;
    }

    // Recursively process left and right subtrees first
    node->left = deleteArtistRecursive(node->left, artist, deleted);
    node->right = deleteArtistRecursive(node->right, artist, deleted);

    bool found = false;
    // Search and remove all matching artist entries in current node
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

    // If all songs under this title were deleted, remove this node
    if (node->count == 0)
    {
        // case 1: No children
        if (!node->left && !node->right)
        {
            delete node;
            return nullptr;
        }

        // case 2: Only left child
        if (node->left && !node->right)
        {
            TitleBSTNode *temp = node->left;
            delete node;
            return temp;
        }

        // case 3: Only left child
        if (!node->left && node->right)
        {
            TitleBSTNode *temp = node->right;
            delete node;
            return temp;
        }

        // case 4: Two children
        TitleBSTNode *replacementParent = node;
        TitleBSTNode *replacement = node->right;

        // Find leftmost node in right subtree
        while (replacement->left)
        {
            replacementParent = replacement;
            replacement = replacement->left;
        }

        // Copy successor data into current node
        node->title = replacement->title;
        node->artist = replacement->artist;
        node->run_time = replacement->run_time;
        node->count = replacement->count;

        // Remove successor node
        if (replacementParent == node)
            replacementParent->right = replacement->right;

        else
            replacementParent->left = replacement->right;
        delete replacement;
    }
    return node;
}

// Delete a specific title
bool TitleBST::deleteTitle(const string &title)
{
    TitleBSTNode *cur = root;
    TitleBSTNode *parent = nullptr;

    // Search for the node with the given title
    while (cur && cur->title != title)
    {
        parent = cur;
        // If title is smaller
        if (title < cur->title)
        {
            cur = cur->left;
        }
        // If title is larger
        else
        {
            cur = cur->right;
        }
    }

    // If title not found
    if (!cur)
    {
        return false;
    }

    // case 1: Node has no children
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

    // case 2: Node has only left child
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

    // case 3: Node has only right child
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

    // case 4: Node has two children
    else
    {
        // Find inorder successor
        TitleBSTNode *replacementParent = cur;
        TitleBSTNode *replacement = cur->right;
        while (replacement->left)
        {
            replacementParent = replacement;
            replacement = replacement->left;
        }

        // Copy replacement node data into current node
        cur->title = replacement->title;
        cur->artist = replacement->artist;
        cur->run_time = replacement->run_time;
        cur->count = replacement->count;

        // Remove the successor node
        if (replacementParent == cur)
            replacementParent->right = replacement->right;
        else
            replacementParent->left = replacement->right;

        delete replacement;
    }

    return true;
}

// Delete a specific song
bool TitleBST::deleteSong(const string &artist, const string &title)
{
    TitleBSTNode *cur = root;

    // Search for the node that matches the given title
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

    // If title node not found
    if (!cur)
    {
        return false;
    }

    // Search for the artist in the song list and delete it
    for (int i = 0; i < cur->count; i++)
    {
        if (cur->artist[i] == artist)
        {
            cur->artist.erase(cur->artist.begin() + i);
            cur->run_time.erase(cur->run_time.begin() + i);
            cur->count--;

            // If no songs remain, delete the entire title node
            if (cur->count == 0)
            {
                deleteTitle(title);
            }

            return true;
        }
    }

    // If artist not found
    return false;
}