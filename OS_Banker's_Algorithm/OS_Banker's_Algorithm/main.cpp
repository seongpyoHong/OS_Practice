#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <queue>
using namespace std;

#define SIZE 4
//Resource의 종류 : 4개
struct Resource
{
	int a, b, c, d;
};
//Process 구조체
struct Process
{
	int Pid;
	Resource Max;
	Resource Need;
	Resource Allocation;
};

//연산자 오버로드
Resource operator+ (Resource a, Resource b) {
	Resource temp;
	temp.a = a.a + b.a;
	temp.b = a.b + b.b;
	temp.c = a.c + b.c;
	temp.d = a.d + b.d;

	return temp;
}
Resource operator- (Resource a, Resource b) {
	Resource temp;
	temp.a = a.a - b.a;
	temp.b = a.b - b.b;
	temp.c = a.c - b.c;
	temp.d = a.d - b.d;

	return temp;
}

//프로세스의 개수 = 4개
Process arr[4];
//자원의 총량 
Resource Total_Resource;
//이용 가능한 자원의 양
Resource Available;
//방문 했는지 확인
bool Finish[4] = { false, };

//process 필요한 resource 입력
void input(Process& P) {
	printf("\nProcess %d [Max] : ", (P.Pid));
	int w, x, y, z;
	cin >> w >> x >> y >> z;
	P.Max.a = w;
	P.Max.b = x;
	P.Max.c = y;
	P.Max.d = z;

	printf("Process %d [Allocation] : ", (P.Pid));
	cin >> w >> x >> y >> z;
	P.Allocation.a = w;
	P.Allocation.b = x;
	P.Allocation.c = y;
	P.Allocation.d = z;

	//Need 계산
	P.Need.a = P.Max.a - P.Allocation.a;
	P.Need.b = P.Max.b - P.Allocation.b;
	P.Need.c = P.Max.c - P.Allocation.c;
	P.Need.d = P.Max.d - P.Allocation.d;

	//Need 출력
	printf("Process %d [Need] : ", (P.Pid));
	cout << P.Need.a << " " << P.Need.b << " " << P.Need.c << " " << P.Need.d << endl;

	//Available 계산
	Available.a -= P.Allocation.a;
	Available.b -= P.Allocation.b;
	Available.c -= P.Allocation.c;
	Available.d -= P.Allocation.d;

}

//Need와 Available 크기 비교
bool comp_Resouce(Resource Need) {
	if (Need.a <= Available.a && Need.b <= Available.b && Need.c <= Available.c && Need.d <= Available.d)
		return true;
	else return false;
}

int main() {
	//Total Resouce를 15로 정해준다.
	Total_Resource.a = 15;
	Total_Resource.b = 15;
	Total_Resource.c = 15;
	Total_Resource.d = 15;
	//Available을 계산하기 위해 초기값 : total resource로 맞춘다.
	Available = Total_Resource;

	//각 프로세스에 필요한 resource를 입력한다.
	for (int i = 0; i < SIZE; i++) {
		arr[i].Pid = i + 1;
		input(arr[i]);
	}
	//Avaliable 출력
	cout << "\nAvailable Resource : " << Available.a << " " << Available.b << " " << Available.c << " " << Available.d << endl;

	//index를 관리하기 위한 변수
	int i = 0;

	int safe_count = 0;
	queue<int> safety_state;
	while (i < SIZE) {
		//Need가 Available보다 작고, 아직 방문하지 않은 변수를 찾는다.
		if (!Finish[i] && comp_Resouce(arr[i].Need)) {
			//이용했다고 가정하고 Available에 자원을 사용후 반환한 양을 더해준다
			Available = (Available + arr[i].Allocation);
			//process가 처리했으므로 finish를 true로 바꿔준다.
			Finish[i] = true;
			//safety_state에 process를 추가한다.
			safety_state.push(arr[i].Pid);
			//index 초기화
			i = 0;
			safe_count++;
		}
		//위에 조건에 해당하지 않는 경우 다음 process로 넘어간다.
		else
			i++;
	}

	//모든 process의 finish가 true가 아닌 경우
	if (safe_count != SIZE)
		cout << "Safety State가 존재하지 않습니다." << endl;
	//safety_state가 존재하는 경우 순서를 출력
	else {
		cout << "\nSafety State" << endl;
		cout << safety_state.front();
		safety_state.pop();
		while (!safety_state.empty()) {
			cout << " -> "<< safety_state.front();
			safety_state.pop();
		}
	}
}