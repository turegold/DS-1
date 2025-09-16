#pragma once
#include "MusicQueueNode.h"
#include <string>
#include <vector>

using namespace std;

class ArtistBST;

class ArtistBSTNode
{
	friend class ArtistBST;

private:
	string artist;			 // 가수 이름
	vector<string> title;	 // 노래 제목 리스트
	vector<string> run_time; // 각 노래의 재생 시간
	int count = 0;
	ArtistBSTNode *left = nullptr;
	ArtistBSTNode *right = nullptr;

public:
	ArtistBSTNode() = default;

	ArtistBSTNode(const string &artist, const string &title, const string &run_time)
		: artist(artist), title{title}, run_time{run_time}, count(1), left(nullptr), right(nullptr) {}
	~ArtistBSTNode() = default;

	const string &getArtist() const
	{
		return artist;
	}

	const vector<string> &getTitles() const
	{
		return title;
	}

	const vector<string> &getRunTimes() const
	{
		return run_time;
	}

	int getCount() const
	{
		return count;
	}

	ArtistBSTNode *getLeft() const
	{
		return left;
	}

	ArtistBSTNode *getRight() const
	{
		return right;
	}

	// 왜 있는 건지 모르겠음
	// void set()
	// {
	// }
	// void search()
	// {
	// }
};