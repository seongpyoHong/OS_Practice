#include <iostream>
#include <random>
#include <ctime>
#include <vector>
#include <list>
#include <string>
#include <Windows.h>
using namespace std;

enum State { UNLOCK = 1, LOCK = 2, DELAY = 3, WRITE = 4 };

struct Buffer {
	//버퍼 상태 
	int state;
	int value;
};

//각 상태의 최대값 지정
#define DELAY_MAX 4
#define LOCK_MAX 4
#define FREE_MAX 5

//각 상태의 갯수
int freeCount = 0;
int  delayCount = 0;
int lockCount = 0;

vector<Buffer>* HashQueue;
vector<Buffer> FreeList;

void makeInit(int modNum, int bufferNum) {
	//랜덤한 seed 생성
	srand((unsigned int)time(0));

	//HashQueue 메모리 할당
	HashQueue = new vector<Buffer>[modNum];

	//HashQueue에 값 대입 (input <100)
	for (int i = 0; i < bufferNum; i++) {
		int input = rand() % 100;
		int index = input % modNum;

		//블록 생성
		Buffer temp;
		temp.value = input;

		//delayCount는 최대 4으로 제한
		if (delayCount < DELAY_MAX) {
			temp.state = (State)((rand() % 3) + 1);
		}
		else {
			//lockCount 최대 4로 제한
			if (lockCount < LOCK_MAX) {
				temp.state = (State)((rand() % 2) + 1);
			}
			else
				temp.state = UNLOCK;
		}
		if (temp.state == DELAY)
			delayCount++;
		if (temp.state == LOCK)
			lockCount++;

		//freelist에 연결 (확률  :  25% , DELAY는 무조건 뿌려준다.)
		if (rand() % 4 == 1 || temp.state == DELAY) {
				FreeList.push_back(temp);
		}

		//HashQueue에 블록 연결
		HashQueue[index].push_back(temp);
	}
}
void printHash(int modNum) {
	cout << "---------------------------------------------------" << endl;
	cout << " < HashQueue > " << endl << endl;
	for (int i = 0; i < modNum; i++) {
		for (int j = 0; j < HashQueue[i].size(); j++) {
			cout << "Value : " << HashQueue[i][j].value << " " << "State : " << (HashQueue[i][j].state) << " / ";
		}
		cout << endl;
	}
	cout << endl;
	cout << "---------------------------------------------------" << endl;
	cout << "< FreeList >" << endl;
	for (int i = 0; i < FreeList.size(); i++) {
		cout << "Value : " << FreeList[i].value << " " << "State : " << (FreeList[i].state) << endl;;
	}
	cout << endl;
}
Buffer getblk(int input,int modNum) {

	//HashQueue에 존재하는 체크하기 위한 변수
	bool check = false;
	//인덱스 위치를 기억하기 위한 변수
	int I, J;
	while (!check) {
		//HashQueue에서 검색
		for (int i = 0; i < modNum; i++) {
			for (int j = 0; j < HashQueue[i].size(); j++) {
				if (HashQueue[i][j].value == input) {
					check = true;
					I = i;
					J = j;
					break;
				}
			}
		}
		printHash(modNum);
		//HashQueue에 있는 경우
		if (check) {
			//--------------------Scenario 5----------------------
			if (HashQueue[I][J].state == LOCK) {
				cout << "--------------------Scenario 5----------------------" << endl;
				cout << "This situation is Scenario 5!" << endl << endl;
				cout << "Block in HashQueue but buffer is busy! " << endl;
				cout << "Process Sleep in 3 seconds ! " << endl << endl;
				Sleep(3000);
				cout << "Because we have one process, change block state to other state" << endl;
				HashQueue[I][J].state = UNLOCK;
				continue;
			}
			//---------------------------------------------------
			//--------------------Scenario 1----------------------
			//HashQueue의 block을 busy로 만든다.
			cout << "--------------------Scenario 1----------------------" << endl;
			cout << "This situation is Scenario 1!" << endl << endl;
			cout << "Block in HashQueue but Block in FreeList." << endl << endl;
			HashQueue[I][J].state = LOCK;
			//FreeList에서 삭제한다.
			vector<Buffer>::iterator itor = FreeList.begin();
			for (int i = 0; i < FreeList.size(); i++) {
				if (HashQueue[I][J].value == FreeList[i].value) {
					itor += i;
					FreeList.erase(itor);
					break;
				}
			}
			//block을 반환한다.
			return HashQueue[I][J];
			//---------------------------------------------------
		}
		else {
			//--------------------Scenario 4----------------------
			if (FreeList.size() == 0) {
				cout << "--------------------Scenario 4----------------------" << endl;
				cout << "This situation is Scenario 4!" << endl << endl;
				cout << "Block not in HashQueue and FreeList is empty." << endl << endl;
				cout << "Process Sleep in 3 seconds ! " << endl << endl;
				Sleep(3000);
				cout << "Because we have one process, Add random block to FreeList." << endl;
				//상태가 unlock인 블록을 Freelist에 연결시켜준다.
				for (int i = 0; i < modNum; i++) {
					for (int j = 0; j < HashQueue[i].size(); j++) {
						if (HashQueue[i][j].state == UNLOCK) {
							Buffer newBuffer = HashQueue[i][j];
							FreeList.push_back(newBuffer);
							break;
						}
					}
				}
				continue;
			}
			//---------------------------------------------------
			//FreeList의 첫 번쨰 값 임시 저장
			Buffer temp2 = FreeList[0];
			//첫번째 값 삭제
			FreeList.erase(FreeList.begin());
			//HashTable에서 검색
			for (int i = 0; i < modNum; i++) {
				for (int j = 0; j < HashQueue[i].size(); j++) {
					if (temp2.value == HashQueue[i][j].value) {
						I = i;
						J = j;
						break;
					}
				}
			}
			//--------------------Scenario 3----------------------
			if (temp2.state == DELAY) {
				cout << "--------------------Scenario 3----------------------" << endl;
				cout << "This situation is Scenario 3!" << endl << endl;
				cout << "Change first block's state in FreeList.  ( DELAY  ==> WRITE ) " << endl << endl;
				HashQueue[I][J].state = WRITE;
				cout << "Because we have one process, Add random block to FreeList." << endl;
				HashQueue[I][J].state = UNLOCK;
				continue;
			}
			//---------------------------------------------------
			//--------------------Scenario 2----------------------
			cout << "--------------------Scenario 2----------------------" << endl;
			cout << "This situation is Scenario 2!" << endl << endl;
			int index= input % modNum;
			cout << "Add new block in index " << index << endl;
			//FreeList의 처음에 위치했던 값을 HashQueue에서도 삭제
			HashQueue[I].erase(HashQueue[I].begin() + J);
			//새로운 값 추가
			Buffer temp;
			temp.value = input;
			temp.state = UNLOCK;
			HashQueue[index].push_back(temp);
			return temp;
			//---------------------------------------------------
		}

	}
}

int main(){
	//mod를 할 숫자
	int modNum;
	// 버퍼의 총 갯수
	int bufferNum;
	// 사용자 입력
	cout << "Mod Number : ";
	cin >> modNum;
	cout << "Numbers of Buffer : ";
	cin >> bufferNum;

	//------------------버퍼 생성----------------------
	makeInit(modNum, bufferNum);
	//------------------------------------------------

	//찾고자 하는 데이터 입력
	int seekNum;
	cout << "Number of Buffer to GetBlk : ";
	cin >> seekNum;

	//getblock 알고리즘 구현
	Buffer result = getblk(seekNum,modNum);

	//값 확인
	cout << "--------------------Final State--------------------" << endl;
	printHash(modNum);

	cout << "------------------------------------------------" << endl;
	cout << "Find Value : "<<result.value << "  Success !" <<endl;
	
	system("pause");
}

