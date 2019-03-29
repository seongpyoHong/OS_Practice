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
	//���� ���� 
	int state;
	int value;
};

//�� ������ �ִ밪 ����
#define DELAY_MAX 4
#define LOCK_MAX 4
#define FREE_MAX 5

//�� ������ ����
int freeCount = 0;
int  delayCount = 0;
int lockCount = 0;

vector<Buffer>* HashQueue;
vector<Buffer> FreeList;

void makeInit(int modNum, int bufferNum) {
	//������ seed ����
	srand((unsigned int)time(0));

	//HashQueue �޸� �Ҵ�
	HashQueue = new vector<Buffer>[modNum];

	//HashQueue�� �� ���� (input <100)
	for (int i = 0; i < bufferNum; i++) {
		int input = rand() % 100;
		int index = input % modNum;

		//��� ����
		Buffer temp;
		temp.value = input;

		//delayCount�� �ִ� 4���� ����
		if (delayCount < DELAY_MAX) {
			temp.state = (State)((rand() % 3) + 1);
		}
		else {
			//lockCount �ִ� 4�� ����
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

		//freelist�� ���� (Ȯ��  :  25% , DELAY�� ������ �ѷ��ش�.)
		if (rand() % 4 == 1 || temp.state == DELAY) {
				FreeList.push_back(temp);
		}

		//HashQueue�� ��� ����
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

	//HashQueue�� �����ϴ� üũ�ϱ� ���� ����
	bool check = false;
	//�ε��� ��ġ�� ����ϱ� ���� ����
	int I, J;
	while (!check) {
		//HashQueue���� �˻�
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
		//HashQueue�� �ִ� ���
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
			//HashQueue�� block�� busy�� �����.
			cout << "--------------------Scenario 1----------------------" << endl;
			cout << "This situation is Scenario 1!" << endl << endl;
			cout << "Block in HashQueue but Block in FreeList." << endl << endl;
			HashQueue[I][J].state = LOCK;
			//FreeList���� �����Ѵ�.
			vector<Buffer>::iterator itor = FreeList.begin();
			for (int i = 0; i < FreeList.size(); i++) {
				if (HashQueue[I][J].value == FreeList[i].value) {
					itor += i;
					FreeList.erase(itor);
					break;
				}
			}
			//block�� ��ȯ�Ѵ�.
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
				//���°� unlock�� ����� Freelist�� ��������ش�.
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
			//FreeList�� ù ���� �� �ӽ� ����
			Buffer temp2 = FreeList[0];
			//ù��° �� ����
			FreeList.erase(FreeList.begin());
			//HashTable���� �˻�
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
			//FreeList�� ó���� ��ġ�ߴ� ���� HashQueue������ ����
			HashQueue[I].erase(HashQueue[I].begin() + J);
			//���ο� �� �߰�
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
	//mod�� �� ����
	int modNum;
	// ������ �� ����
	int bufferNum;
	// ����� �Է�
	cout << "Mod Number : ";
	cin >> modNum;
	cout << "Numbers of Buffer : ";
	cin >> bufferNum;

	//------------------���� ����----------------------
	makeInit(modNum, bufferNum);
	//------------------------------------------------

	//ã���� �ϴ� ������ �Է�
	int seekNum;
	cout << "Number of Buffer to GetBlk : ";
	cin >> seekNum;

	//getblock �˰��� ����
	Buffer result = getblk(seekNum,modNum);

	//�� Ȯ��
	cout << "--------------------Final State--------------------" << endl;
	printHash(modNum);

	cout << "------------------------------------------------" << endl;
	cout << "Find Value : "<<result.value << "  Success !" <<endl;
	
	system("pause");
}

