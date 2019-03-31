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

//freelist�� �߰�
void addFreelist(int i, int j) {
	FreeList.push_back(HashQueue[i][j]);
}

//Enum ����� ���� �Լ�
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

//�ʱ� HashQueue ���� �Լ�
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
		Buffer temp2 = temp;
		//HashQueue�� ��� ����
		HashQueue[index].push_back(temp2);
	}
}

//HashQueue �� FreeList ��� �Լ�
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

//block�� �޾ƿ��� �Լ�
Buffer getblk(int input,int modNum) {

	//HashQueue�� �����ϴ� üũ�ϱ� ���� ����
	bool check = false;
	//�ε��� ��ġ�� ����ϱ� ���� ����
	int I, J;
	while (!check) {
		check = false;
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
		system("pause");

		//HashQueue�� �ִ� ���
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
			//HashQueue�� block�� busy�� �����.
			cout << endl << "\n---------------------- < Scenario 1 > ------------------------\n";
			cout << "Block in HashQueue but Block in FreeList.\n\n" ;
			system("pause");
			system("cls");

			HashQueue[I][J].state = LOCK;
			//FreeList���� �����Ѵ�.
			vector<Buffer>::iterator itor = FreeList.begin();
			for (int i = 0; i < FreeList.size(); i++) {
				if (HashQueue[I][J].value == FreeList[i].value) {
					itor += i;
					FreeList.erase(itor);
					freeCount--;
					break;
				}
			}

			//��������Ƿ� FreeList�� �߰�
			addFreelist(I, J);
			//block�� ��ȯ�Ѵ�.
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

				//���°� unlock�� ����� Freelist�� ��������ش�.
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

			//FreeList�� ó���� ��ġ�ߴ� ���� HashQueue������ ����
			HashQueue[I].erase(HashQueue[I].begin() + J);
			//���ο� �� �߰�
			Buffer temp;
			temp.value = input;
			temp.state = LOCK;
			HashQueue[index].push_back(temp);
			//��������Ƿ� FreeList�� �߰�
			addFreelist(index, HashQueue[index].size() - 1);
			//block�� ��ȯ�Ѵ�.
			return temp;
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

	//���� ����
	cout << "---------------------- < Initial State > -----------------------\n";
	makeInit(modNum, bufferNum);
	//�ʱ� ���� Ȯ��
	printHash(modNum);
	while (1) {
		//ã���� �ϴ� ������ �Է�
		int seekNum;
		cout << "\nNumber of Buffer to GetBlk ( EXIT : -1 ) : ";
		cin >> seekNum;
		system("cls");
		//����
		if (seekNum == -1) break;
		//getblock �Լ� ���� �� ��� �� ����
		Buffer result = getblk(seekNum, modNum);

		//�� Ȯ��
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

