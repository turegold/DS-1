#pragma once
#include "MusicQueue.h"
#include "TitleBST.h"
#include "ArtistBST.h"
#include "PlayList.h"
#include <fstream>

using namespace std;

class Manager
{
private:
	MusicQueue q;
	ArtistBST ab;
	TitleBST tb;
	PlayList pl;
	ifstream fcmd;
	ofstream flog;

public:
	// Constructor
	Manager();
	// Destructor
	~Manager();

	// Entry point for processing commands
	void run(const char *command);
	// Load music data from "music_list.txt" into MusicQueue
	void LOAD();
	// Add a song to MusicQueue
	void ADD(const string &line);
	// Pop all songs from the MsuicQueue to ArtistBST and TitleBST
	void QPOP();
	// Search for artist or title or song
	void SEARCH(const string &line);
	// Generate or insert to Playlist from BSTs
	void MAKEPL(const string &line);
	// Print BSTs or playlist's information
	void PRINT(const string &line);
	// Delete title, song or title
	void DELETE(const string &line);
	// Finalize and clean up
	void EXIT();
};