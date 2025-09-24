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
    // 명령어 파일 열기
    fcmd.open(command);
    if (!fcmd.is_open())
    {
        cout << "명령어 파일 열기 실패!\n";
        return;
    }

    // 로그 파일 열기
    flog.open("log.txt");
    if (!flog.is_open())
    {
        cout << "로그 파일 열기 실패!\n";
        return;
    }

    string line;
    while (getline(fcmd, line))
    {
        // 공백 제거
        if (line.empty())
        {
            continue;
        }

        // 명령어 이름 추출
        stringstream ss(line);
        string cmd;
        ss >> cmd;

        // 명령어 분기 처리
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
            break; // 종료
        }
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
    // 이미 큐에 데이터가 있는 경우
    if (!q.empty())
    {
        flog << "========ERROR========\n";
        flog << "100\n";
        flog << "=======================\n";
        return;
    }

    // 음악 파일 열기
    ifstream fin("Music_List.txt");
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

        getline(ss, artist, '|');
        getline(ss, title, '|');
        getline(ss, time);

        // 노드 생성
        MusicQueueNode *node = new MusicQueueNode(artist, title, time);
        // 큐에 삽입
        q.push(node);

        flog << artist << "/" << title << "/" << time << "\n";
    }
    flog << "=======================\n";

    fin.close();
}

void Manager::ADD(const string &line)
{
    string artist, title, time;

    // line에서 ADD 다음 파라미터만 추출
    size_t pos = line.find("ADD ");
    // "ADD "를 찾은 경우에만 실행
    if (pos != string::npos)
    {
        string trimmed = line.substr(pos + 4); // "ADD "가 4글자니까 시작 위치 + 4부터 끝까지 잘라냄
        stringstream ss(trimmed);
        getline(ss, artist, '|');
        getline(ss, title, '|');
        getline(ss, time);
    }

    // 유효성 검사
    if (artist.empty() || title.empty() || time.empty())
    {
        flog << "========ERROR========\n";
        flog << "200\n";
        flog << "======================\n";
        return;
    }

    if (q.isFull())
    {
        cerr << "Queue의 사이즈가 100을 초과하여 프로그램을 종료합니다.\n";
        exit(1);
    }

    // 중복 체크 (큐 내부에 동일한 데이터가 존재하는 지 확인)
    if (q.exist(artist, title, time))
    {
        flog << "========ERROR========\n";
        flog << "200\n";
        flog << "======================\n";
        return;
    }

    // 큐에 삽입
    MusicQueueNode *newNode = new MusicQueueNode(artist, title, time);
    q.push(newNode);

    // 삽입 성공
    flog << "========ADD========\n";
    flog << artist << "/" << title << "/" << time << "\n";
    flog << "====================\n";
}

void Manager::QPOP()
{
    // 1. 큐가 비어있는지 확인
    if (q.empty())
    {
        flog << "========ERROR========\n";
        flog << "300\n";
        flog << "======================\n";
        return;
    }

    bool is_success = true;

    // 큐에서 하나씩 꺼내서 ab, tb에 삽입
    while (!q.empty())
    {
        MusicQueueNode *node = q.pop();

        bool insertToAB = ab.insert(node);
        bool insertToTB = tb.insert(node);

        if (!insertToAB || !insertToTB)
        {
            is_success = false;
        }

        delete node;
    }

    if (is_success)
    {
        flog << "========QPOP========\n";
        flog << "Success\n";
        flog << "====================\n";
    }
    else
    {
        flog << "========ERROR========\n";
        flog << "300\n";
        flog << "======================\n";
    }
}

void Manager::SEARCH(const string &line)
{
    stringstream ss(line);
    string cmd, option;
    ss >> cmd >> option;

    // 옵션 파싱 실패 시
    if (option != "ARTIST" && option != "TITLE" && option != "SONG")
    {
        flog << "========ERROR========\n";
        flog << "400\n";
        flog << "======================\n";
        return;
    }

    if (option == "ARTIST")
    {
        string artist;
        getline(ss, artist);       // 남은 부분 통째로
        artist = artist.substr(1); // 앞의 공백 제거

        if (ab.searchArtist(artist, flog, false))
        {
            flog << "========SEARCH========\n";
            ab.searchArtist(artist, flog, true);
            flog << "====================\n";
        }
        else
        {
            flog << "========ERROR========\n";
            flog << "400\n";
            flog << "======================\n";
        }
    }
    else if (option == "TITLE")
    {
        string title;
        getline(ss, title);
        title = title.substr(1);

        if (tb.searchTitle(title, flog, false))
        {
            flog << "========SEARCH========\n";
            tb.searchTitle(title, flog, true);
            flog << "====================\n";
        }
        else
        {
            flog << "========ERROR========\n";
            flog << "400\n";
            flog << "======================\n";
        }
    }
    else if (option == "SONG")
    {
        string param;
        getline(ss, param);
        param = param.substr(1); // 공백제거

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

        if (ab.searchSong(artist, title, flog, false))
        {
            flog << "========SEARCH========\n";
            ab.searchSong(artist, title, flog, true);
            flog << "====================\n";
        }
        else
        {
            flog << "========ERROR========\n";
            flog << "400\n";
            flog << "======================\n";
        }
    }
}

void Manager::MAKEPL(const string &line)
{
    stringstream ss(line);
    string cmd, option;
    ss >> cmd >> option;

    // 옵션 유효성 확인
    if (option != "ARTIST" && option != "TITLE" && option != "SONG")
    {
        flog << "========ERROR========\n";
        flog << "500\n";
        flog << "======================\n";
        return;
    }

    bool success = false;

    if (option == "ARTIST")
    {
        string artist;
        getline(ss, artist);
        artist = artist.substr(1); // 공백 제거

        success = ab.searchArtistToPlayList(artist, pl, flog);
    }
    else if (option == "TITLE")
    {
        string title;
        getline(ss, title);
        title = title.substr(1);

        success = tb.searchTitleToPlayList(title, pl, flog);
    }
    else if (option == "SONG")
    {
        string param;
        getline(ss, param);
        param = param.substr(1);

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

        success = ab.searchSongToPlayList(artist, title, pl, flog);
    }

    // 삽입 성공 여부 확인
    if (!success)
    {
        flog << "========ERROR========\n";
        flog << "500\n";
        flog << "======================\n";
        return;
    }

    flog << "========MAKEPL========\n";

    // 플레이리스트 내용 출력
    flog << pl.print();

    // count 출력
    flog << "Count : " << pl.size() << " / 10\n";

    // 삽입 성공
    int total_sec = pl.run_time();
    int minutes = total_sec / 60;
    int seconds = total_sec % 60;

    flog << "Time : " << minutes << "min " << seconds << "sec\n";
    flog << "====================\n";
}

void Manager::PRINT(const string &line)
{
    stringstream ss(line);
    string cmd, option;
    ss >> cmd >> option;

    bool is_success = false;

    if (option == "ARTIST")
    {
        if (!ab.isEmpty())
        {
            flog << "========Print========\n";
            flog << "ArtistBST\n";
            ab.printTree(flog);
            flog << "====================\n";
            is_success = true;
        }
    }
    else if (option == "TITLE")
    {
        if (!tb.isEmpty())
        {
            flog << "========Print========\n";
            flog << "TitleBST\n";
            tb.printTree(flog);
            flog << "====================\n";
            is_success = true;
        }
    }
    else if (option == "LIST")
    {

        if (!pl.empty())
        {
            flog << "========Print========\n";
            flog << pl.print();
            flog << "====================\n";
            is_success = true;
        }
    }

    if (!is_success)
    {
        flog << "========ERROR========\n";
        flog << "600\n";
        flog << "======================\n";
    }
}

void Manager::DELETE(const string &line)
{
    stringstream ss(line);
    string cmd, option;
    ss >> cmd >> option;

    flog << "========DELETE========\n";

    bool success = false;

    if (option == "ARTIST")
    {
        string artist;
        getline(ss, artist);
        artist = artist.substr(1); // 공백 제거

        success = ab.deleteArtist(artist) & tb.deleteArtist(artist) & pl.deleteArtist(artist);
    }
    else if (option == "TITLE")
    {
        string title;
        getline(ss, title);
        title = title.substr(1);

        success = ab.deleteTitle(title) & tb.deleteTitle(title) & pl.deleteTitle(title);
    }
    else if (option == "LIST")
    {
        string param;
        getline(ss, param);
        param = param.substr(1);

        size_t sep = param.find('|');
        if (sep == string::npos)
        {
            flog << "========ERROR========\n";
            flog << "700\n";
            return;
        }

        string artist = param.substr(0, sep);
        string title = param.substr(sep + 1);

        success = pl.deleteFromList(artist, title);
    }
    else if (option == "SONG")
    {
        string param;
        getline(ss, param);
        param = param.substr(1); // 공백 제거

        size_t sep = param.find('|');
        if (sep == string::npos)
        {
            flog << "========ERROR========\n";
            flog << "700\n";
            return;
        }

        string artist = param.substr(0, sep);
        string title = param.substr(sep + 1);

        success = ab.deleteSong(artist, title) & tb.deleteSong(artist, title) & pl.deleteFromList(artist, title);
    }
    else
    {
        flog << "========ERROR========\n";
        flog << "700\n";
        return;
    }

    // 성공하면 로그 출력
    if (success)
    {
        flog << "Success\n";
    }
    else
    {
        flog << "========ERROR========\n";
        flog << "700\n";
    }
    flog << "======================\n";
}

void Manager::EXIT()
{
    flog << "========EXIT========\n";
    flog << "Success\n";
    flog << "====================\n";
}