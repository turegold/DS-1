#include "Manager.h"
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

Manager::Manager()
{
}

Manager::~Manager()
{
}

void Manager::run(const char *command)
{
    // Open command file
    fcmd.open(command);
    if (!fcmd.is_open())
    {
        cout << "Failed to open command file.\n";
        return;
    }

    // Open log file
    flog.open("log.txt", ios::app);
    if (!flog.is_open())
    {
        cout << "Failed to open log file.\n";
        return;
    }

    string line;
    while (getline(fcmd, line))
    {
        // Skip empty lines
        if (line.empty())
        {
            continue;
        }

        // Extract command
        stringstream ss(line);
        string cmd;
        ss >> cmd;

        // Command dispatch
        if (cmd == "LOAD")
        {
            this->LOAD();
        }
        else if (cmd == "ADD")
        {
            this->ADD(line);
        }
        else if (cmd == "QPOP")
        {
            this->QPOP();
        }
        else if (cmd == "SEARCH")
        {
            this->SEARCH(line);
        }
        else if (cmd == "MAKEPL")
        {
            this->MAKEPL(line);
        }
        else if (cmd == "PRINT")
        {
            this->PRINT(line);
        }
        else if (cmd == "DELETE")
        {
            this->DELETE(line);
        }
        else if (cmd == "EXIT")
        {
            this->EXIT();
            break;
        }
        // Invalid command error
        else
        {
            flog << "========ERROR========\n";
            flog << "1000\n";
            flog << "======================\n";
        }
    }

    fcmd.close();
    flog.close();
}

void Manager::LOAD()
{
    cout << "LOAD 실행\n";
    // If the queue is not empty, the LOAD command should fial.
    if (!q.empty())
    {
        flog << "========ERROR========\n";
        flog << "100\n";
        flog << "=======================\n";
        return;
    }

    // Open the music list file
    ifstream fin("Music_List.txt");

    // If the file can't be opened, log error code 100
    if (!fin.is_open())
    {
        flog << "========ERROR========\n";
        flog << "100\n";
        flog << "=======================\n";
        return;
    }

    flog << "========LOAD========\n";

    string line;
    while (getline(fin, line))
    {
        stringstream ss(line);
        string artist, title, time;

        // Music_List.txt format: artist|title|time
        getline(ss, artist, '|');
        getline(ss, title, '|');
        getline(ss, time);

        // Create a new MusicQueueNode and push it to the queue
        MusicQueueNode *node = new MusicQueueNode(artist, title, time);
        q.push(node);

        // Log the pushed song in the format artist/title/time
        flog << artist << "/" << title << "/" << time << "\n";
    }
    flog << "=======================\n";

    fin.close();
}

void Manager::ADD(const string &line)
{
    cout << "ADD 실행\n";
    string artist, title, time;

    // parse artist|title|time
    size_t pos = line.find("ADD ");

    if (pos != string::npos)
    {
        string trimmed = line.substr(pos + 4); // Skip "ADD "
        stringstream ss(trimmed);
        getline(ss, artist, '|');
        getline(ss, title, '|');
        getline(ss, time);
    }

    // Missing required fields
    if (artist.empty() || title.empty() || time.empty())
    {
        flog << "========ERROR========\n";
        flog << "200\n";
        flog << "======================\n";
        return;
    }

    // Queue can hold up to 100 songs
    if (q.isFull())
    {
        flog << "========ERROR========\n";
        flog << "200\n";
        flog << "======================\n";
        return;
    }

    // Duplicate song check in queue
    if (q.exist(artist, title, time))
    {
        flog << "========ERROR========\n";
        flog << "200\n";
        flog << "======================\n";
        return;
    }

    // Push to queue
    MusicQueueNode *newNode = new MusicQueueNode(artist, title, time);
    q.push(newNode);

    // Log the successfully added song
    flog << "========ADD========\n";
    flog << artist << "/" << title << "/" << time << "\n";
    flog << "====================\n";
}

void Manager::QPOP()
{
    cout << "QPOP 실행\n";
    // If queue is empty, return error code 300
    if (q.empty())
    {
        flog << "========ERROR========\n";
        flog << "300\n";
        flog << "======================\n";
        return;
    }

    bool is_success = true;

    // Pop songs from queue and insert into both BSTs
    while (!q.empty())
    {
        // Dequeue one song
        MusicQueueNode *node = q.pop();

        // Insert into ArtistBST
        bool insertToAB = ab.insert(node);
        // Insert into TitleBST
        bool insertToTB = tb.insert(node);

        // One of the inserts failed
        if (!insertToAB || !insertToTB)
        {
            is_success = false;
        }

        delete node;
    }

    // Write result to log file
    if (is_success)
    {
        flog << "========QPOP========\n";
        flog << "Success\n";
        flog << "====================\n";
        return;
    }
    else
    {
        flog << "========ERROR========\n";
        flog << "300\n";
        flog << "======================\n";
        return;
    }
}

void Manager::SEARCH(const string &line)
{
    cout << "SEARCH 실행\n";
    stringstream ss(line);
    string cmd, option;
    ss >> cmd >> option;

    // Check if option is one of the valid types
    if (option != "ARTIST" && option != "TITLE" && option != "SONG")
    {
        flog << "========ERROR========\n";
        flog << "400\n";
        flog << "======================\n";
        return;
    }

    // Case 1: SEARCH ARTIST
    if (option == "ARTIST")
    {
        string artist;
        getline(ss, artist);
        artist = artist.substr(1);

        // Only check existence
        if (ab.searchArtist(artist, flog, false))
        {
            flog << "========SEARCH========\n";
            // Write to log
            ab.searchArtist(artist, flog, true);
            flog << "====================\n";
            return;
        }
        else
        {
            flog << "========ERROR========\n";
            flog << "400\n";
            flog << "======================\n";
            return;
        }
    }

    // Case 2: SEARCH TITLE
    else if (option == "TITLE")
    {
        string title;
        getline(ss, title);
        title = title.substr(1);

        // Only check existence
        if (tb.searchTitle(title, flog, false))
        {
            flog << "========SEARCH========\n";
            // Write to log
            tb.searchTitle(title, flog, true);
            flog << "====================\n";
            return;
        }
        else
        {
            flog << "========ERROR========\n";
            flog << "400\n";
            flog << "======================\n";
            return;
        }
    }

    // Case 3: SEARCH SONG
    else if (option == "SONG")
    {
        string param;
        getline(ss, param);
        param = param.substr(1);

        // No delimiter found
        size_t sep = param.find('|');
        if (sep == string::npos)
        {
            flog << "========ERROR========\n";
            flog << "400\n";
            flog << "======================\n";
            return;
        }

        string artist = param.substr(0, sep);
        string title = param.substr(sep + 1);

        // Only check existence
        if (ab.searchSong(artist, title, flog, false))
        {
            flog << "========SEARCH========\n";
            // Write to log
            ab.searchSong(artist, title, flog, true);
            flog << "====================\n";
            return;
        }
        else
        {
            flog << "========ERROR========\n";
            flog << "400\n";
            flog << "======================\n";
            return;
        }
    }
}

void Manager::MAKEPL(const string &line)
{
    cout << "MAKEPL 실행\n";
    stringstream ss(line);
    string cmd, option;
    ss >> cmd >> option;

    // Check if option is one of the valid types
    if (option != "ARTIST" && option != "TITLE" && option != "SONG")
    {
        flog << "========ERROR========\n";
        flog << "500\n";
        flog << "======================\n";
        return;
    }

    bool success = false;

    // CASE 1: ARTIST
    if (option == "ARTIST")
    {
        string artist;
        getline(ss, artist);
        artist = artist.substr(1);

        // Add all songs of the artist to the PlayList from ArtistBST
        success = ab.searchArtistToPlayList(artist, pl, flog);
    }

    // CASE 2: TITLE
    else if (option == "TITLE")
    {
        string title;
        getline(ss, title);
        title = title.substr(1);

        // Add all songs with matching title to the PlayList from TitleBST
        success = tb.searchTitleToPlayList(title, pl, flog);
    }
    // CASE 3: SONG
    else if (option == "SONG")
    {
        string param;
        getline(ss, param);
        param = param.substr(1);

        // Parse artist|title format
        size_t sep = param.find('|');
        if (sep == string::npos)
        {
            flog << "========ERROR========\n";
            flog << "500\n";
            flog << "======================\n";
            return;
        }

        string artist = param.substr(0, sep);
        string title = param.substr(sep + 1);

        // Add a single song to the PlayList from ArtistBST
        success = ab.searchSongToPlayList(artist, title, pl, flog);
    }

    // If insertion failed
    if (!success)
    {
        flog << "========ERROR========\n";
        flog << "500\n";
        flog << "======================\n";
        return;
    }

    // If insertion success
    flog << "========MAKEPL========\n";
    flog << pl.print();
    flog << "Count : " << pl.size() << " / 10\n";
    int total_sec = pl.run_time();
    int minutes = total_sec / 60;
    int seconds = total_sec % 60;
    flog << "Time : " << minutes << "min " << seconds << "sec\n";
    flog << "====================\n";
    return;
}

void Manager::PRINT(const string &line)
{
    cout << "PRINT 실행\n";
    stringstream ss(line);
    string cmd, option;
    ss >> cmd >> option;

    bool is_success = false;

    // Check if option is one of the valid types
    if (option != "ARTIST" && option != "TITLE" && option != "LIST")
    {
        flog << "========ERROR========\n";
        flog << "500\n";
        flog << "======================\n";
        return;
    }

    // Case 1: ARTIST
    if (option == "ARTIST")
    {
        // Check if ArtistBST is not empty
        if (!ab.isEmpty())
        {
            flog << "========PRINT========\n";
            flog << "ArtistBST\n";
            ab.printTree(flog);
            flog << "====================\n";
            is_success = true;
        }
    }

    // Case 2: TITLE
    else if (option == "TITLE")
    {
        // Check if TitleBST is not empty
        if (!tb.isEmpty())
        {
            flog << "========PRINT========\n";
            flog << "TitleBST\n";
            tb.printTree(flog);
            flog << "====================\n";
            is_success = true;
        }
    }

    // Case 3: LIST
    else if (option == "LIST")
    {
        // Check if PlayList is not empty
        if (!pl.empty())
        {
            flog << "========PRINT========\n";
            flog << pl.print();
            flog << "Count : " << pl.size() << " / 10\n";

            int total_sec = pl.run_time();
            int minutes = total_sec / 60;
            int seconds = total_sec % 60;
            flog << "Time : " << minutes << "min " << seconds << "sec\n";
            flog << "====================\n";
            is_success = true;
        }
    }

    // If none of the above options succeeded
    if (!is_success)
    {
        flog << "========ERROR========\n";
        flog << "600\n";
        flog << "======================\n";
        return;
    }
}

void Manager::DELETE(const string &line)
{
    cout << "DELETE 실행\n";
    stringstream ss(line);
    string cmd, option;
    ss >> cmd >> option;

    bool success = false;

    // Check if option is one of the valid types
    if (option != "ARTIST" && option != "TITLE" && option != "LIST" && option != "SONG")
    {
        flog << "========ERROR========\n";
        flog << "700\n";
        flog << "======================\n";
        return;
    }

    // Case 1: ARTIST
    if (option == "ARTIST")
    {
        string artist;
        getline(ss, artist);
        artist = artist.substr(1);

        // Delete all data related to the artist from both BSTs
        bool ab_deleted = ab.deleteArtist(artist);
        bool tb_deleted = tb.deleteArtist(artist);

        // Try deleting from PlayList as well
        pl.deleteArtist(artist);

        // Consider successful only if both BST deletions succeeded
        success = ab_deleted && tb_deleted;
    }

    // Case 2: TITLE
    else if (option == "TITLE")
    {
        string title;
        getline(ss, title);
        title = title.substr(1);

        // Delete all songs with the given title from both BSTs
        bool ab_deleted = ab.deleteTitle(title);
        bool tb_deleted = tb.deleteTitle(title);

        // Try deleting from PlayList as well
        pl.deleteTitle(title);

        // Success only if both BST deletions succeeded
        success = ab_deleted && tb_deleted;
    }

    // Case 3: LIST
    else if (option == "LIST")
    {
        string param;
        getline(ss, param);
        param = param.substr(1);

        // Parse artist|title format
        size_t sep = param.find('|');
        if (sep == string::npos)
        {
            flog << "========ERROR========\n";
            flog << "700\n";
            flog << "======================\n";
            return;
        }

        string artist = param.substr(0, sep);
        string title = param.substr(sep + 1);

        // Only delete from PlayList
        success = pl.deleteFromList(artist, title);
    }

    // Case 4: SONG
    else if (option == "SONG")
    {
        string param;
        getline(ss, param);
        param = param.substr(1);

        // Parse artist|title format
        size_t sep = param.find('|');
        if (sep == string::npos)
        {
            flog << "========ERROR========\n";
            flog << "700\n";
            flog << "======================\n";
            return;
        }

        string artist = param.substr(0, sep);
        string title = param.substr(sep + 1);

        // Delete one specific song from both BSTs and PlayList
        bool ab_deleted = ab.deleteSong(artist, title);
        bool tb_deleted = tb.deleteSong(artist, title);
        pl.deleteFromList(artist, title);

        // Success only if both BST deletions succeeded
        success = ab_deleted && tb_deleted;
    }
    else
    {
        flog << "========ERROR========\n";
        flog << "700\n";
        flog << "======================\n";
        return;
    }

    // Output results
    if (success)
    {
        flog << "========DELETE========\n";
        flog << "Success\n";
        flog << "======================\n";
        return;
    }
    else
    {
        flog << "========ERROR========\n";
        flog << "700\n";
        flog << "======================\n";
        return;
    }
}

void Manager::EXIT()
{
    cout << "EXIT 실행\n";
    flog << "========EXIT========\n";
    flog << "Success\n";
    flog << "====================\n";
    return;
}