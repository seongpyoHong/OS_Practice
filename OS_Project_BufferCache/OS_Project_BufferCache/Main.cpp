#include <iostream>
#include <random>
#include <ctime>
#include <vector>
#include <list>
#include <string>
#include <iomanip>
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

//freelist에 추가
void addFreelist(int i, int j) {
	FreeList.push_back(HashQueue[i][j]);
}

//Enum 출력을 위한 함수
string process_numbers_str(int num) {
	switch (num) {
	case 1: return "UNLOCK";
		break;
	case 2: return "LOCK";
		break;
	case 3: return "DELAY";
		break;
	case 4: return "WRITE";
		break;
	}
}

//초기 HashQueue 생성 함수
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
		Buffer temp2 = temp;
		//HashQueue에 블록 연결
		HashQueue[index].push_back(temp2);
	}
}

//HashQueue 및 FreeList 출력 함수
void printHash(int modNum) {
	cout << "----------------------------------------------------------------" << endl;
	cout << " < HashQueue > " << endl << endl;
	cout.setf(ios::left);

	for (int i = 0; i < modNum; i++) {
		cout << "MOD " << i << "  |  ";
		for (int j = 0; j < HashQueue[i].size(); j++) {
			string state = process_numbers_str(HashQueue[i][j].state);
			cout <<"Value : " << setw(2)<< HashQueue[i][j].value << "   " << "State : "<< setw(6) << state << " | ";
		}
		cout << endl;
	}
	cout << endl;
	cout << "----------------------------------------------------------------" << endl;
	cout << "< FreeList >" << endl;
	for (int i = 0; i < FreeList.size(); i++) {
		string state = process_numbers_str(FreeList[i].state);
		cout << "Value : " <<setw(2) <<FreeList[i].value << " | " << "State : " << setw(6)<<state << endl;;
	}
	cout << "----------------------------------------------------------------\n\n";
}

//block을 받아오는 함수
Buffer getblk(int input,int modNum) {

	//HashQueue에 존재하는 체크하기 위한 변수
	bool check = false;
	//인덱스 위치를 기억하기 위한 변수
	int I, J;
	while (!check) {
		check = false;
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
		system("pause");

		//HashQueue에 있는 경우
		if (check) {
			//--------------------Scenario 5----------------------
			if (HashQueue[I][J].state == LOCK || HashQueue[I][J].state==DELAY) {
				cout << "\n---------------------- < Scenario 5 > ------------------------\n";
				cout << "Block in HashQueue but buffer is busy! \n";
				cout << "Process Sleep in 3 seconds ! \n" ;
				Sleep(3000);
				cout << endl;
				cout << "Because we have one process, change block's state ( LOCK ==> UNLOCK ) \n\n";
				HashQueue[I][J].state = UNLOCK;
				check = false;
				system("pause");
				system("cls");
				continue;
			}

			//--------------------Scenario 1----------------------
			//HashQueue의 block을 busy로 만든다.
			cout << endl << "\n---------------------- < Scenario 1 > ------------------------\n";
			cout << "Block in HashQueue but Block in FreeList.\n\n" ;
			system("pause");
			system("cls");

			HashQueue[I][J].state = LOCK;
			//FreeList에서 삭제한다.
			vector<Buffer>::iterator itor = FreeList.begin();
			for (int i = 0; i < FreeList.size(); i++) {
				if (HashQueue[I][J].value == FreeList[i].value) {
					itor += i;
					FreeList.erase(itor);
					freeCount--;
					break;
				}
			}

			//사용했으므로 FreeList에 추가
			addFreelist(I, J);
			//block을 반환한다.
			Buffer temp = HashQueue[I][J];
			return temp;
		}
		else {
			//--------------------Scenario 4----------------------
			if (FreeList.size() == 0) {
				cout << endl << "---------------------- < Scenario 4 > ------------------------\n";
				cout  << endl;
				cout << "Block not in HashQueue and FreeList is empty.\n\n";
				cout << "Process Sleep in 3 seconds ! \n\n";
				Sleep(3000);
				cout << "Because we have one process, Add random block to FreeList." << endl << endl << endl;
				system("pause");
				system("cls");

				//상태가 unlock인 블록을 Freelist에 연결시켜준다.
				for (int i = 0; i < modNum; i++) {
					for (int j = 0; j < HashQueue[i].size(); j++) {
						if (HashQueue[i][j].state == UNLOCK) {
							Buffer newBuffer = HashQueue[i][j];
							FreeList.push_back(newBuffer);
							freeCount++;
							break;
						}
					}
				}
				continue;
			}


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
				cout << endl << "------------------------ < Scenario 3 > --------------------------" << endl;
				cout << endl;
				cout << "Change first block's state in FreeList.  ( DELAY  ==> WRITE ) " << endl << endl;
				HashQueue[I][J].state = WRITE;
				cout << "Because we have one process, change the state. ( WRITE ==> UNLOCK )" << endl <<endl << endl;
				HashQueue[I][J].state = UNLOCK;

				system("pause");
				system("cls");
				continue;
			}

			//--------------------Scenario 2----------------------
			cout << "\n---------------------- < Scenario 2 > ------------------------\n";
			int index= input % modNum;
			cout << "Add new block in MOD " << index << endl << endl << endl;
			system("pause");
			system("cls");

			//FreeList의 처음에 위치했던 값을 HashQueue에서도 삭제
			HashQueue[I].erase(HashQueue[I].begin() + J);
			//새로운 값 추가
			Buffer temp;
			temp.value = input;
			temp.state = LOCK;
			HashQueue[index].push_back(temp);
			//사용했으므로 FreeList에 추가
			addFreelist(index, HashQueue[index].size() - 1);
			//block을 반환한다.
			return temp;
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

	//버퍼 생성
	cout << "---------------------- < Initial State > -----------------------\n";
	makeInit(modNum, bufferNum);
	//초기 상태 확인
	printHash(modNum);
	while (1) {
		//찾고자 하는 데이터 입력
		int seekNum;
		cout << "\nNumber of Buffer to GetBlk ( EXIT : -1 ) : ";
		cin >> seekNum;
		system("cls");
		//종료
		if (seekNum == -1) break;
		//getblock 함수 실행 및 결과 값 리턴
		Buffer result = getblk(seekNum, modNum);

		//값 확인
		cout << "\n---------------------- < Final State > -----------------------\n";
		printHash(modNum);
		cout << setw(10) << "Value : " << result.value << endl<<endl;
		cout << setw(10) << "Find  Success !" << endl << endl;
		system("pause");
		system("cls");
	}	
	system("pause");
	return 0;
}

