/*
	Contiguous Memory Allocation
	�ִ� ������ �ִ� process�� ���̰� ���� ���� ���� ����(Best fit)
	compaction : ���� ���� �������� �Ѵ�. => ������ ������ ���� �� ���� �������� �����̵��� �ؾ��Ѵ�.
	Coalescing : �� ������ ����Ǿ��� ��, ����Ǿ����� ǥ������� �Ѵ�.
	�ڷᱸ�� ���� : List? array? vector?
*/
#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
struct Space
{
	//process�� �Ҵ�Ǿ����� Ȯ��
	bool check = false;
	int alloc_pid;
	int start_address;
	int end_address;
	int size;
};
struct Process
{
	//process�� release����, allocation���� üũ
	bool state;
	int pid;
	int alloc_size;
	int start_address;
	int end_address;
};

//input�� �Է¹ޱ� ���� ����
vector<int> input;
vector<Space> space;

//space ������ ���� �Լ�
bool compare(Space x, Space y) {
	return x.start_address < y.start_address;
}
/*
	���� ������ �ִٸ� ���� ������ ����(space - process�� ũ�Ⱑ ���� ���� ���)�� �ִ´�.
	���� ������ ���� ��� false�� ��ȯ�Ѵ�.
*/
bool findSpace(Process P) {
	int index = -1;
	int best = 1000000;
	for (int i = 0; i < space.size(); i++) {
		if (!space[i].check && P.alloc_size <= space[i].size) {
			int temp_best = min(best, (space[i].size - P.alloc_size));
			if (best != temp_best) {
				best = temp_best;
				index = i;
			}
		}
	}
	if (index != -1) {
		//���ο� ���� ����
		Space alloc_space;
		alloc_space.check = true;
		alloc_space.start_address = space[index].start_address;
		alloc_space.end_address = (space[index].start_address + P.alloc_size);
		alloc_space.size = P.alloc_size;

		//���� �������� �� ������ ����
		//start_address�� ã�� size�� ������ �״�� �ִ´�. size�� �ٸ� ��쿡�� ���� space�� start_address�� end_address�� �����Ѵ�.
		for (int i = 0; i < space.size(); i++) {
			if (space[i].start_address == alloc_space.start_address && space[i].size == alloc_space.size) {
				space[i].check = true;
				space[i].alloc_pid = P.pid;
			}
			else if (space[i].start_address == alloc_space.start_address && space[i].size != alloc_space.size) {
				//���ο� ���� ����(����)
				Space new_space;
				new_space.end_address = space[i].end_address;
				new_space.start_address = alloc_space.end_address;
				new_space.size = new_space.end_address - new_space.start_address;
				vector<Space>::iterator itor = space.begin();
				itor += i;
				space.erase(itor);
				space.push_back(new_space);
				//alloc_space �߰�
				space.push_back(alloc_space);
			}
		}
		//������ ������ start_address ������ ����
		sort(space.begin(), space.end(), compare);
			return true;
	}
	//���� �� �ִ� ������ ���� ���
	else 
		return false;
}

//������� ���
void printState() {
	cout << "==========================Current Memory=========================\n";
	for (int i = 0; i < space.size(); i++) {
		if (space[i].check) {
			cout << "Allocate :  Yes" << endl;
			cout << "Pid : " << space[i].alloc_pid << endl;
			cout << "Start Address : " << space[i].start_address << endl;
			cout << "End Address : " << space[i].end_address << endl;
			cout << "=================================================================\n";

		}
		else if (!space[i].check) {
			cout << "Allocate :  No" << endl;
			cout << "Start Address : " << space[i].start_address << endl;
			cout << "End Address : " << space[i].end_address << endl;
			cout << "=================================================================\n";
		}
	}
}

/*
	���� ������ ���� ��� �籸���ؾ��ϴµ� �籸���� �� ���� ���� �����̴� ����� ���� �������� �Ѵ�.
	0K ���� ������ Ž��
	end K���� process Ž��
	����� ���� ũ�⸦ ���� �Ҵ�� ������ �ִٸ� ���鿡 ����
*/
void compaction(int size) {
	bool flag = false;
	for (int i = 0; i < space.size() - 1; i++) {
		if (!space[i].check) {
			for (int j = space.size()-1; j >= i+1; j--) {
				if (space[j].check && space[j].size <= space[i].size) {
					flag = true;
					//�ؿ� ���� �Ҵ� ����
					space[j].check = false;
					//���� ������ �Ҵ��ϰ� ��й�
					if (space[i].size == space[j].size) {
						space[i].end_address = space[i].start_address + space[j].size;
						space[i].size = space[j].size;
						space[i].check = true;
						space[i].alloc_pid = space[j].alloc_pid;
					}
					else {
						Space temp;
						temp.check = false;
						temp.end_address = space[i].end_address;
						space[i].end_address = space[i].start_address + space[j].size;
						space[i].size = space[j].size;
						space[i].check = true;
						space[i].alloc_pid = space[j].alloc_pid;
						temp.start_address = space[i].end_address;
						space.push_back(temp);
					}
					sort(space.begin(), space.end(), compare);
					break;
				}
			}
		}
		if (flag) {
			flag = false;
			i = 0;
		}
	}
}

/*
	compaction�� ����ǰ� �� ��, ���Ӱ� block�� ���Եǰ� �� ��
	���� ����Ǿ� �ϴ� space �� process�� �ִٸ� ����
*/
void coalescing() {	
	for (int i = 0; i < space.size()-1; i++) {
		if (!space[i].check && !space[i + 1].check && space[i].end_address == space[i + 1].start_address) {
			cout << "\nCoalescing blocks at addresses " << space[i].start_address << "K and " << space[i + 1].start_address << "K" << endl;
			space[i].end_address = space[i + 1].end_address;
			space[i].size += space[i + 1].size;
			vector<Space>::iterator itor = space.begin();
			itor += (i + 1);
			space.erase(itor);
		}
	}
}

/*
	�Ҵ�� �޸𸮸� �����Ѵ�.
*/
void freeMemory(int pid) {
	for (int i = 0; i < space.size(); i++) {
		if (space[i].check && space[i].alloc_pid == pid)
			space[i].check = false;
	}
}

int main() {
	//�޸� ������ �Է�
	int Memory_Size;
	cout << "Memory Size : ";
	cin >> Memory_Size;
	//�ʱ���� �Ҵ�
	Space initspace;
	initspace.check = false;
	initspace.start_address = 0;
	initspace.end_address = Memory_Size;
	initspace.size = Memory_Size;
	space.push_back(initspace);

	//���μ��� ���� �Է�
	int NumberOfProcess;
	cout << "Numer Of Process : ";
	cin >> NumberOfProcess;

	//Process pid �Ҵ� �� �ʱⰪ ����
	Process* process = new Process[NumberOfProcess+1];
	for(int i=1;i<=NumberOfProcess;i++){
		process[i].pid = i;
		process[i].state = false;
		process[i].alloc_size = 0;
		process[i].start_address = -1;
		process[i].end_address = -1;
	}

	//-1�� �Է¹����� �Է� ����
	while (1) {
		int process_id, alloc_size;
		cout << "Input Pid and Allocate Size (Last Input : -1 -1) \n";
		cin >> process_id >> alloc_size;
		input.push_back(process_id);
		input.push_back(alloc_size);
		//���� ����
		if (process_id == -1 && alloc_size ==-1)
			break;
	}

	//�� �ܰ躰�� ó���ϱ�
	for (int i = 0; i < input.size()-2; i += 2) {
		int process_id = input[i];
		int alloc_size = input[i + 1];
		if (alloc_size != 0) {
			printf("\nRequest %d : %d K\n", process_id, alloc_size);
			//process�� alloc_size �Ҵ�
			process[process_id].alloc_size = alloc_size;
			//������ ���� �� : best fit�� ����
			if (findSpace(process[process_id])) {
				printState();
				system("pause");
				system("cls");
			}
			//������ ���� �� compaction ���� �� �ִ´�.
			else if (!findSpace(process[process_id])) {
				compaction(process[process_id].alloc_size);
				coalescing();
				if (!findSpace(process[process_id]))
					cout << "Can't Allocate ! " << endl;
				printState();
				system("pause");
				system("cls");
			}
		}
		else {
			//�Ҵ�� ���μ����� �������ְ� state üũ
			printf("\nFree Request %d : %d\n", process_id, process[process_id].alloc_size);
			freeMemory(process_id);
			coalescing();
			printState();
			system("pause");
			system("cls");
		}
	}
	return 0;
}
