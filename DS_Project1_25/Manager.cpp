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

        cout << "읽은 명령어: " << line << endl;

        // 로그에 명령어 기록
        flog << "===" << line << "===" << endl;

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
            this->SEARCH();
        }
        else if (cmd == "MAKEPL")
        {
            this->MAKEPL();
        }
        else if (cmd == "PRINT")
        {
            this->PRINT(line);
        }
        else if (cmd == "DELETE")
        {
            this->DELETE();
        }
        else if (cmd == "EXIT")
        {
            this->EXIT();
            break; // 종료
        }
        else
        {
            flog << "Unkown command" << endl;
        }

        flog << "===========" << endl;
    }

    fcmd.close();
    flog.close();
}

void Manager::LOAD()
{
    // 음악 파일 열기
    ifstream fin("Music_List.txt");
    if (!fin.is_open())
    {
        flog << "음악 파일 열기 실패!\n";
        return;
    }

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
    }
    flog << "Success\n";

    // 큐에 담겨있는 노래 출력 -> 나중에 삭제 예정
    q.printAll();

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
        flog << "ADD\n";
        flog << "유효성 검사 중 오류 발생\n";
        flog << "===========\n";
        return;
    }

    // 중복 체크 (큐 내부에 동일한 데이터가 존재하는 지 확인)
    MusicQueueNode *cur = q.front();
    while (cur)
    {
        // 큐 내부에 이미 존재할 경우
        if (cur->getArtist() == artist && cur->getTitle() == title && cur->getRunTime() == time)
        {
            flog << "ADD\n";
            flog << "큐에 중복된 노래가 추가됨\n";
            flog << "===========\n";
            return;
        }
        cur = cur->getNext();
    }

    // 큐에 삽입
    MusicQueueNode *newNode = new MusicQueueNode(artist, title, time);
    q.push(newNode);

    flog << "ADD\n";
    flog << "Success\n";
    flog << "===========\n";
}

void Manager::QPOP()
{
    // 1. 큐가 비어있는지 확인
    if (q.empty())
    {
        flog << "QPOP\n";
        flog << "Queue is empty\n";
        return;
    }

    // 2. 큐에서 노드 꺼내기
    MusicQueueNode *node = q.pop();

    // 3. BST에 삽입
    ab.insert(node);
    tb.insert(node);

    // 4. 로그 기록
    flog << "QPOP\n";
    flog << "Success\n";
    flog << "===========\n";

    delete node;
}

void Manager::SEARCH()
{
}

void Manager::MAKEPL()
{
}

void Manager::PRINT(const string &line)
{
    stringstream ss(line);
    string cmd, option;
    ss >> cmd >> option;

    if (option == "ARTIST")
    {
        flog << "PRINT ARTIST\n";
        if (!ab.isEmpty())
        {
            ab.printTree(flog);
        }
        else
        {
            flog << "Artist BST is empty\n";
        }
    }
    else if (option == "TITLE")
    {
        flog << "PRINT TITLE\n";
        if (!tb.isEmpty())
        {
            tb.printTree(flog);
        }
        else
        {
            flog << "Title BST is empty\n";
        }
    }
    else if (option == "LIST")
    {
        // 나중에 구현할 예정
    }

    flog << "===========\n";
}

void Manager::DELETE()
{
}

void Manager::EXIT()
{
}