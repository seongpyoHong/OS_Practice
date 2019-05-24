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

//�Ҵ��� ������ ã�� �Լ�
bool findSpace(Process P) {
	/*
	���� ������ �ִٸ� ���� ������ ����(space - process�� ũ�Ⱑ ���� ���� ���)�� �ִ´�.
	���� ������ ���� ��� false�� ��ȯ�Ѵ�.
	*/
	int index = -1;
	int best = 1000000;
	for (int i = 0; i < space.size(); i++) {
		if (space[i].check && P.alloc_size < space[i].size) {
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
	else return false;
}
void compaction() {
	/*
	���� ������ ���� ��� �籸���ؾ��ϴµ� �籸���� �� ���� ���� �����̴� ����� ���� �������� �Ѵ�.
	*/
}

void coalescing() {
	/*
	compaction�� ����ǰ� �� ��, ���Ӱ� block�� ���Եǰ� �� �� 
	���� ����Ǿ� �ϴ� space �� process�� �ִٸ� ���������� �Ѵ�.
	
	*/
}


int main() {
	//�޸� ������ �Է�
	int Memory_Size;
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
			printf("Request %d : %d K\n", process_id, alloc_size);
			//process�� alloc_size �Ҵ�
			process[process_id].alloc_size = alloc_size;
			//������ ���� �� : best fit�� ����
			if (findSpace(process[process_id])) {
				
			}
			//������ ���� �� compaction ���� �� �ִ´�.
			else if (!findSpace()) {

			}
		}
		else {
			printf("Free Request %d : %d\n", process_id, process[process_id].alloc_size);
			//�Ҵ�� ���μ����� �������ְ� state üũ
		}
	}
	system("pause");
	return 0;
}