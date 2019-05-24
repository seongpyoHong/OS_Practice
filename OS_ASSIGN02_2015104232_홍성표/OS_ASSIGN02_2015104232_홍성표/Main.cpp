/*
	Contiguous Memory Allocation
	넣는 공간과 넣는 process의 차이가 가장 작은 것을 고른다(Best fit)
	compaction : 가장 적게 움직여야 한다. => 삽입할 공간이 없을 때 가장 최적으로 움직이도록 해야한다.
	Coalescing : 두 조각이 연결되었을 때, 연결되었음을 표시해줘야 한다.
	자료구조 선택 : List? array? vector?
*/
#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
struct Space
{
	//process에 할당되었는지 확인
	bool check = false;
	int alloc_pid;
	int start_address;
	int end_address;
	int size;
};
struct Process
{
	//process가 release인지, allocation인지 체크
	bool state;
	int pid;
	int alloc_size;
	int start_address;
	int end_address;
};

//input을 입력받기 위한 벡터
vector<int> input;
vector<Space> space;

//space 정렬을 위한 함수
bool compare(Space x, Space y) {
	return x.start_address < y.start_address;
}

//할당할 공간을 찾는 함수
bool findSpace(Process P) {
	/*
	넣을 공간이 있다면 가장 최적의 공간(space - process의 크기가 가장 작은 경우)에 넣는다.
	넣을 공간이 없는 경우 false를 반환한다.
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
		//새로운 공간 형성
		Space alloc_space;
		alloc_space.check = true;
		alloc_space.start_address = space[index].start_address;
		alloc_space.end_address = (space[index].start_address + P.alloc_size);
		alloc_space.size = P.alloc_size;

		//기존 공간에서 저 공간을 제외
		//start_address로 찾고 size가 같으면 그대로 넣는다. size가 다를 경우에는 기존 space의 start_address와 end_address를 수정한다.
		for (int i = 0; i < space.size(); i++) {
			if (space[i].start_address == alloc_space.start_address && space[i].size == alloc_space.size) {
				space[i].check = true;
				space[i].alloc_pid = P.pid;
			}
			else if (space[i].start_address == alloc_space.start_address && space[i].size != alloc_space.size) {
				//새로운 공간 생성(분할)
				Space new_space;
				new_space.end_address = space[i].end_address;
				new_space.start_address = alloc_space.end_address;
				new_space.size = new_space.end_address - new_space.start_address;
				space.push_back(new_space);
				//alloc_space 추가
				space.push_back(alloc_space);
			}
		}
		//분할이 끝나면 start_address 순으로 정렬
		sort(space.begin(), space.end(), compare);
			return true;
	}
	//넣을 수 있는 공간이 없는 경우
	else return false;
}
void compaction() {
	/*
	넣을 공간이 없는 경우 재구성해야하는데 재구성할 때 가장 적게 움직이는 경우의 수로 움직여야 한다.
	*/
}

void coalescing() {
	/*
	compaction이 수행되고 난 후, 새롭게 block이 삽입되고 난 후 
	서로 연결되야 하는 space 및 process가 있다면 연결시켜줘야 한다.
	
	*/
}


int main() {
	//메모리 사이즈 입력
	int Memory_Size;
	cin >> Memory_Size;
	//초기공간 할당
	Space initspace;
	initspace.check = false;
	initspace.start_address = 0;
	initspace.end_address = Memory_Size;
	initspace.size = Memory_Size;
	space.push_back(initspace);

	//프로세스 개수 입력
	int NumberOfProcess;
	cin >> NumberOfProcess;

	//Process pid 할당 및 초기값 설정
	Process* process = new Process[NumberOfProcess+1];
	for(int i=1;i<=NumberOfProcess;i++){
		process[i].pid = i;
		process[i].state = false;
		process[i].alloc_size = 0;
		process[i].start_address = -1;
		process[i].end_address = -1;
	}

	//-1을 입력받으면 입력 종료
	while (1) {
		int process_id, alloc_size;
		cin >> process_id >> alloc_size;
		input.push_back(process_id);
		input.push_back(alloc_size);
		//종료 조건
		if (process_id == -1 && alloc_size ==-1)
			break;
	}

	//각 단계별로 처리하기
	for (int i = 0; i < input.size()-2; i += 2) {
		int process_id = input[i];
		int alloc_size = input[i + 1];
		if (alloc_size != 0) {
			printf("Request %d : %d K\n", process_id, alloc_size);
			//process에 alloc_size 할당
			process[process_id].alloc_size = alloc_size;
			//공간이 있을 때 : best fit에 적용
			if (findSpace(process[process_id])) {
				
			}
			//공간이 없을 때 compaction 진행 후 넣는다.
			else if (!findSpace()) {

			}
		}
		else {
			printf("Free Request %d : %d\n", process_id, process[process_id].alloc_size);
			//할당된 프로세스를 해제해주고 state 체크
		}
	}
	system("pause");
	return 0;
}