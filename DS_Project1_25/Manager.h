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
	/*
		PlayList pl;
	*/
	ifstream fcmd;
	ofstream flog;

public:
	Manager();
	~Manager();

	void run(const char *command);

	void LOAD();
	void ADD(const string &line);
	void QPOP();
	void SEARCH();
	void MAKEPL();
	void PRINT(const string &line);
	void DELETE();
	void EXIT();
};