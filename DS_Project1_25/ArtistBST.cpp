#include "ArtistBST.h"
#include "ArtistBSTNode.h"
#include <iostream>
#include <fstream>
using namespace std;

// Constructor
ArtistBST::ArtistBST()
{
    root = nullptr;
}

// Recursively delete all nodes in the tree
void ArtistBST::destroyTree(ArtistBSTNode *node)
{
    if (node == nullptr)
    {
        return;
    }

    destroyTree(node->left);
    destroyTree(node->right);

    delete node;
}

// Destructor
ArtistBST::~ArtistBST()
{
    destroyTree(root);
}

// Insert a node from MusicQueue
bool ArtistBST::insert(MusicQueueNode *node)
{
    string artist = node->getArtist();
    string title = node->getTitle();
    string run_time = node->getRunTime();

    // If the tree is empty, create a new root node
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
            // Check for duplicate song title under the same artist
            for (int i = 0; i < cur->title.size(); i++)
            {
                if (cur->title[i] == title)
                {
                    return false;
                }
            }

            // Artists already exists, so add the song to their list
            cur->title.push_back(title);
            cur->run_time.push_back(run_time);
            cur->count++;
            return true;
        }
        else if (artist < cur->getArtist())
        {
            // Move to left child if artist name is smaller
            if (!cur->left)
            {
                cur->left = new ArtistBSTNode(artist, title, run_time);
                return true;
            }
            cur = cur->left;
        }
        else
        {
            // Move to right child if artist name is larger
            if (!cur->right)
            {
                cur->right = new ArtistBSTNode(artist, title, run_time);
                return true;
            }
            cur = cur->right;
        }
    }
}

// Check if tree is empty
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

// Print all songs in in-order traversal
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

// Print all artists and songs
void ArtistBST::printTree(ofstream &flog)
{
    printNode(root, flog);
}

// Search for artist and optionally print
bool ArtistBST::searchArtist(const string &artist, ofstream &flog, bool is_print)
{
    ArtistBSTNode *cur = root;

    // Traverse the BST to find the matching artist
    while (cur)
    {
        // If the current node matches the artist
        if (artist == cur->artist)
        {
            // If printing is requested, write all songs of the artist to the file
            if (is_print)
            {
                for (int i = 0; i < cur->count; i++)
                {
                    flog << cur->artist << "/" << cur->title[i] << "/" << cur->run_time[i] << "\n";
                }
            }
            return true;
        }
        // If artist name is smaller
        else if (artist < cur->artist)
        {
            cur = cur->left;
        }
        // If artist name is larger
        else
        {
            cur = cur->right;
        }
    }
    // Artist not found
    return false;
}

// Search for specific song
bool ArtistBST::searchSong(const string &artist, const string &title, ofstream &flog, bool is_print)
{
    ArtistBSTNode *cur = root;
    // Traverse the BST to find the artist node
    while (cur)
    {
        // If artist found, search through their song list
        if (artist == cur->artist)
        {
            for (int i = 0; i < cur->count; i++)
            {
                // If the song title matches
                if (cur->title[i] == title)
                {
                    // If printing is requested, write song info to log file
                    if (is_print)
                    {
                        flog << cur->artist << "/" << cur->title[i] << "/" << cur->run_time[i] << "\n";
                    }
                    return true;
                }
            }

            // If title not matched
            return false;
        }
        // If artist name is smaller
        else if (artist < cur->artist)
        {
            cur = cur->left;
        }
        // If artist name is larger
        else
        {
            cur = cur->right;
        }
    }

    // Artist not found
    return false;
}

// Search by artist and add it to PlayList
bool ArtistBST::searchArtistToPlayList(const string &artist, PlayList &pl, ofstream &flog)
{
    // Check if the artist already exists in the playlist
    if (pl.is_existArtist(artist))
    {
        return false;
    }
    ArtistBSTNode *cur = root;

    // Traverse the BST to find the artist node
    while (cur)
    {

        if (artist == cur->artist)
        {
            // Check if adding all songs exceeds playlist list
            if (pl.size() + cur->count > 10)
            {
                return false;
            }

            // Add each song of the artist to the playlist
            for (int i = 0; i < cur->count; i++)
            {
                // Parse runtime from "mm:ss" to seconds
                string time_str = cur->run_time[i];
                size_t colon = time_str.find(':');
                int minutes = stoi(time_str.substr(0, colon));
                int seconds = stoi(time_str.substr(colon + 1));
                int runtime_sec = minutes * 60 + seconds;

                // Insert into playlist
                pl.insert_node(artist, cur->title[i], runtime_sec);
            }

            return true;
        }
        // If artist name is smaller
        else if (artist < cur->artist)
        {
            cur = cur->left;
        }
        // If artist name is larger
        else
        {
            cur = cur->right;
        }
    }

    // If artist not found
    return false;
}

// Search specific song and add it playList
bool ArtistBST::searchSongToPlayList(const string &artist, const string &title, PlayList &pl, ofstream &flog)
{
    ArtistBSTNode *cur = root;

    // Traverse the BST to find the artist node
    while (cur)
    {
        if (artist == cur->artist)
        {
            // Search for the song title in the artist node
            for (int i = 0; i < cur->count; i++)
            {
                if (cur->title[i] == title)
                {
                    if (pl.exist(artist, title))
                    {
                        return false;
                    }
                    // Check if adding one song exceeds the playlist size limit
                    if (pl.size() + 1 > 10)
                    {
                        return false;
                    }

                    // Convert run_time from "mm:ss" format to seconds
                    string time_str = cur->run_time[i];
                    size_t colon = time_str.find(':');
                    int minutes = stoi(time_str.substr(0, colon));
                    int seconds = stoi(time_str.substr(colon + 1));
                    int runtime_sec = minutes * 60 + seconds;

                    // Insert the song into the playlist
                    pl.insert_node(artist, title, runtime_sec);
                    return true;
                }
            }

            // Artist found, but song title does not exist
            return false;
        }
        // If artist name is smaller
        else if (artist < cur->artist)
        {
            cur = cur->left;
        }
        // If artist name is larger
        else
        {
            cur = cur->right;
        }
    }

    // If not found artist
    return false;
}

// Recursively delete a node by artist
ArtistBSTNode *ArtistBST::deleteArtistRecursive(ArtistBSTNode *node, const string &artist, bool &deleted)
{
    // Node not found
    if (!node)
    {
        return nullptr;
    }

    // If artist is smaller
    if (artist < node->artist)
    {
        node->left = deleteArtistRecursive(node->left, artist, deleted);
    }
    // If artist is larger
    else if (artist > node->artist)
    {
        node->right = deleteArtistRecursive(node->right, artist, deleted);
    }
    else
    {
        // Found the artist node to delete
        deleted = true;

        // Case 1: Node has no children
        if (!node->left && !node->right)
        {
            delete node;
            return nullptr;
        }

        // Case 2: Node has only right child
        else if (!node->left)
        {
            ArtistBSTNode *temp = node->right;
            delete node;
            return temp;
        }
        // Case 3: Node has only left child
        else if (!node->right)
        {
            ArtistBSTNode *temp = node->left;
            delete node;
            return temp;
        }

        // Case 4: Node has two children
        ArtistBSTNode *replacementParent = node;
        ArtistBSTNode *replacement = node->right;

        // Find the leftmost node in the right subtree
        while (replacement->left)
        {
            replacementParent = replacement;
            replacement = replacement->left;
        }

        // Copy successor's data to current node
        node->artist = replacement->artist;
        node->title = replacement->title;
        node->run_time = replacement->run_time;
        node->count = replacement->count;

        // Remove successor node from the tree
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

// Delete artist and all their songs
bool ArtistBST::deleteArtist(const string &artist)
{
    bool deleted = false;
    root = deleteArtistRecursive(root, artist, deleted);

    return deleted;
}

// Recursively delete a node by title
ArtistBSTNode *ArtistBST::deleteTitleRecursive(ArtistBSTNode *node, const string &title, bool &deleted)
{
    // Node not founded
    if (!node)
    {
        return nullptr;
    }

    // Recursively check left subtree
    node->left = deleteTitleRecursive(node->left, title, deleted);

    // Recursively check right subtree
    node->right = deleteTitleRecursive(node->right, title, deleted);

    // Check current node's song list for matching title
    for (int i = 0; i < node->count; i++)
    {
        if (node->title[i] == title)
        {
            // Remove the song title and corresponding run time
            node->title.erase(node->title.begin() + i);
            node->run_time.erase(node->run_time.begin() + i);
            node->count--;
            i--;
            deleted = true;
        }
    }

    // If all songs of this artist are deleted, delete the artist node
    if (node->count == 0)
    {
        // case 1: No children
        if (!node->left && !node->right)
        {
            delete node;
            return nullptr;
        }
        // case 2: Only left child
        else if (node->left && !node->right)
        {
            ArtistBSTNode *temp = node->left;
            delete node;
            return temp;
        }
        // case 3: Only right child
        else if (!node->left && node->right)
        {
            ArtistBSTNode *temp = node->right;
            delete node;
            return temp;
        }

        // case 4: Two children
        else
        {
            // Find in-order successor
            ArtistBSTNode *replacementParent = node;
            ArtistBSTNode *replacement = node->right;

            while (replacement->left)
            {
                replacementParent = replacement;
                replacement = replacement->left;
            }

            // Copy successor's data into current node
            node->artist = replacement->artist;
            node->title = replacement->title;
            node->run_time = replacement->run_time;
            node->count = replacement->count;

            // Remove the successor node
            if (replacementParent == node)
                replacementParent->right = replacement->right;
            else
                replacementParent->left = replacement->right;
            delete replacement;
        }
    }

    return node;
}

// Delete song by title
bool ArtistBST::deleteTitle(const string &title)
{
    bool deleted = false;
    root = deleteTitleRecursive(root, title, deleted);
    return deleted;
}

// Delete specific song by artist and title
bool ArtistBST::deleteSong(const string &artist, const string &title)
{
    ArtistBSTNode *cur = root;
    ArtistBSTNode *parent = nullptr;

    // Search for the artist node in the BST
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

    // If artist is not found, return false
    if (!cur)
    {
        return false;
    }

    // Search for the song title within the artist's song list
    for (int i = 0; i < cur->count; i++)
    {
        if (cur->title[i] == title)
        {
            // Remove the song title and its run time
            cur->title.erase(cur->title.begin() + i);
            cur->run_time.erase(cur->run_time.begin() + i);
            cur->count--;

            // If the artist has no more songs, delete the artist node
            if (cur->count == 0)
            {
                deleteArtist(artist);
            }
            return true;
        }
    }

    // If the song was not found under the artist
    return false;
}