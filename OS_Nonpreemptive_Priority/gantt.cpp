#include <iostream>
#include <vector>
#include <cstdio>
#include <queue>
#include <string>
#include <algorithm>
using namespace std;


//작업 정보를 나타내는 구조체
struct task {
	//공장 이름
	string f_name;
	//작업에 걸리는 시간
	int time;
};

//기계가 작업 정보를 나타내는 구조체
struct machine
{
	//작업 시작 시간
	int start_time;
	//작업 종료 시간
	int end_time;
	// 작업 번호
	int alloc_job = 0;
};

//vector 비교를 위한 함수
bool compare(machine A, machine B) {
	return A.start_time < B.start_time;
}

//공장 제조 상황
vector<machine> factory[3];

//job 목록
queue<task> job[5];

//총 처리시간
int process_time() {
	//모든 작업의 종료시간 - 시작시간을 더하여 총 처리시간을 구한다.
	int result = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < factory[i].size(); j++) {
			result += (factory[i][j].end_time - factory[i][j].start_time);
		}
	}
	return result;
}
//총 완료시간
int makespan() {
	//완료시간 중 가장 큰 값을 선택
	int result = 0;
	for (int i = 0; i < 3; i++) {
		int end = factory[i].size() - 1;
		result = max(result, factory[i][end].end_time);
	}
	return result;
}

//총 대기시간
int latency_time() {
	int result = 0;
	//각 작업별로 분류하기 위한 벡터
	vector<machine> job_latency[5];
	//각 작업 별 latency를 저장
	int latency[5] = { 0 };
	//각 작업별로 분류
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < factory[j].size(); k++) {
				if (factory[j][k].alloc_job == i+1) {
					job_latency[i].push_back(factory[j][k]);
				}
			}
			//start_time 순으로 정렬
			sort(job_latency[i].begin(), job_latency[i].end(), compare);
		}

		//각 작업별로 중간 쉬는 시간과 처음 offset을 더해준다.
		for (int index = 0; index < job_latency[i].size() - 1; index++) {
			if (index == 0)
				latency[i] += (job_latency[i][index].start_time);
			latency[i] += (job_latency[i][index + 1].start_time - job_latency[i][index].end_time);
		}

	}

	//total 대기시간
	int total_latency = 0;

	//각 작업의 latency를 구해준다.
	for (int i = 0; i < 5; i++) {
		total_latency += latency[i];
	}
	return total_latency;
}
//유휴시간
int idle_time() {
	//총 유휴시간
	int idle = 0;
	// 모든 쉬는 시간
	int result = 0;

	//모든 쉬는 시간을 구해준다.
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < factory[i].size()-1; j++) {
			result += (factory[i][j + 1].start_time - factory[i][j].end_time);
		}
	}

	//총 완료시간에서 각 완료시간을 빼준다.
	int total = makespan();
	for (int i = 0; i < 3; i++) {
		int end = factory[i].size() - 1;
		idle += (total - factory[i][end].end_time);
	}
	return result+idle;
}
//흐름 시간
int flow_time() {
	//처리시간
	int process = process_time();
	//대기시간
	int latency = latency_time();
	//작업의 수로 나누어 준다.
	return (process + latency)/5;
}
//기계 가동률
int machine_utilization() {
	//처리시간
	double process = process_time();
	//총 완료시간
	double total = makespan();
	return (process / (total * 3))*100;
}

//납기 위반 작업 수
void check_fail() {
	//각 납기 일수
	int deadline[5] = { 3, 2, 4, 4, 2 };
	//작업의 끝나는 시간
	int job_end[5] = { 0 };

	//최대값으로 갱신
	for (int i = 0; i < 5; i++) {
		job_end[i] = 0;
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < factory[j].size(); k++) {
				if (factory[j][k].alloc_job == i+1) {
					job_end[i] = max(job_end[i], factory[j][k].end_time);
				}
			}
		}
		//일정을 지킨 경우 0올 초기화
		if (job_end[i] < (8 * deadline[i])) {
			job_end[i] = 0;
		}
	}
	cout << "납기 위반 작업 수 :  ";
	int cnt = 0;
	//기한을 지키지 못한 작업 출력
	for (int i = 0; i < 5; i++) {
		if (job_end[i] != 0) {
			cnt++;
			printf("작업 %d , ",i+1);
		}
	}
	printf("으로 총 %d 개 입니다.\n", cnt);
}

//push할 공간을 찾는다.
int search(int index,task alloc,int offset,int job_num) {

	//초기 크기
	int init_size = factory[index].size();
	// 앞에 공간이 있는지 체크
	bool check = false;
	//factory를 돌면서 앞에 들어갈 공간이 있는지 체크
	for (int i = 0; i < init_size; i++) {
		//offset(선행 작업이 끝나는 시간)보다 start가 크고 작업의 수행 시간보다 start_time이 큰 경우
		if (alloc.time <= factory[index][i].start_time && offset<= factory[index][i].start_time) {
			//사이의 빈 공간의 크기가 작업의 수행시간보다 크거나 같은 경우
			if ((factory[index][i].start_time - factory[index][i - 1].end_time) >= alloc.time) {
				//새로운 작업 삽입
				machine temp;
				temp.start_time = factory[index][i - 1].end_time;
				temp.end_time = temp.start_time + alloc.time;
				temp.alloc_job = job_num;
				factory[index].push_back(temp);
				check = true;
				//start_time 순으로 정렬
				sort(factory[index].begin(), factory[index].end(), compare);
				return temp.end_time;
			}
		}
	}

	//앞에 넣을 공간이 없는 경우
	if (!check) {
		//가장 뒤에 삽입
		machine temp;
		temp.alloc_job = job_num;
		//offset과 비교하여 index를 조정
		if (offset <= factory[index][init_size - 1].end_time) {
			temp.start_time = factory[index][init_size - 1].end_time;
			temp.end_time = temp.start_time + alloc.time;
		}
		else {
			temp.start_time = offset;
			temp.end_time = temp.start_time + alloc.time;
		}
		factory[index].push_back(temp);
		sort(factory[index].begin(), factory[index].end(), compare);
		return temp.end_time;
	}
}

//첫번째 값 push
void pushFirst(int index) {
	// 첫번 째 작업 예외처리
	int init_size = job[index].size();
	int offset = 0;
	int fac_ind = 0;
	for (int i = 0; i < init_size; i++) {
		//먼저 queue에 삽입된 작업부터 처리
		task alloc = job[index].front();
		//공장 분류
		if (job[index].front().f_name == "A") {
			 fac_ind= 0;
		}
		else if (job[index].front().f_name == "B") {
			fac_ind = 1;
		}
		else if (job[index].front().f_name == "C") {
			fac_ind = 2;
		}
		//공장에 작업 배치
		machine temp;
		temp.start_time = offset;
		temp.end_time = temp.start_time + alloc.time;
		temp.alloc_job = index+1;
		factory[fac_ind].push_back(temp);
		//offset 수정
		offset = temp.end_time;
		//queue에서 작업 pop
		job[index].pop();
	}
}

//gantt chart 알고리즘
void gantt(int order[]) {
	//첫 번째 작업 예외처리
	pushFirst(order[0] - 1);

	//두 번째 작업부터 처리
	for (int i = 1; i < 5; i++) {
		int index = order[i] - 1;
		int size = job[index].size();
		int offset = 0;
		//각 job 순서대로 실행
		for (int j = 0; j < size; j++) {
			task alloc = job[index].front();
			//공장 분류
			if (job[index].front().f_name == "A") {
				offset = search(0,alloc,offset,index+1);
			}
			else if (job[index].front().f_name == "B") {
				offset = search(1,alloc,offset,index+1);
			}
			else if (job[index].front().f_name == "C") {
				offset = search(2,alloc,offset,index+1);
			}
			job[index].pop();
		}
	}
}
int main() {
	//첫 벡터 삽입 (인덱스 컨트롤을 위해)
	for (int i = 0; i < 3; i++) {
		machine temp;
		temp.start_time = 0;
		temp.end_time = 0;
		factory[i].push_back(temp);
	}
	//공정 입력
	//1번
	task first_one;
	first_one.f_name = "A";
	first_one.time = 2;
	job[0].push(first_one);

	task first_two;
	first_two.f_name = "B";
	first_two.time = 3;
	job[0].push(first_two);

	task first_thr;
	first_thr.f_name = "C";
	first_thr.time = 4;
	job[0].push(first_thr);

	//2번
	first_one.f_name = "C";
	first_one.time = 6;
	job[1].push(first_one);

	first_two.f_name = "A";
	first_two.time = 4;
	job[1].push(first_two);

	//3번
	first_one.f_name = "B";
	first_one.time = 3;
	job[2].push(first_one);

	first_two.f_name = "C";
	first_two.time = 2;
	job[2].push(first_two);

	first_thr.f_name = "A";
	first_thr.time = 1;
	job[2].push(first_thr);

	//4번
	first_one.f_name = "C";
	first_one.time = 4;
	job[3].push(first_one);

	first_two.f_name = "B";
	first_two.time = 3;
	job[3].push(first_two);

	first_thr.f_name = "A";
	first_thr.time = 3;
	job[3].push(first_thr);

	//5번
	first_one.f_name = "A";
	first_one.time = 5;
	job[4].push(first_one);

	first_two.f_name = "B";
	first_two.time = 3;
	job[4].push(first_two);

	//순서 입력
	int order[5];
	cout << "작업을 진행할 순서를 입력하세요. =>  ";
	for (int i = 0; i < 5; i++)
		cin >> order[i];

	//gantt chart algorithm
	gantt(order);
	
	//공장 작업 배치 출력
	cout << "---------------- Machine ------------------" << endl;
	for (int i = 0; i < 3; i++) {
		if (i == 0)
			cout << "< Machine A >\n";
		if (i == 1)
			cout << "< Machine B >\n";
		if (i == 2)
			cout << "< Machine C >\n";
		for (int j = 1; j < factory[i].size(); j++) {
			cout << "Job number: " << factory[i][j].alloc_job<< " ";
			cout << "Start: " << factory[i][j].start_time << " ";
			cout << "End :  " << factory[i][j].end_time << endl;
		}

		cout<<"------------------------------------------"<<endl;
	}

	//각 평가 지표 출력
	cout << "총 처리시간 : " << process_time() << endl;
	cout << "총 대기 시간 : " << latency_time() << endl;
	cout << "유휴 시간 : " << idle_time() << endl;
	cout << "총 완료 시간 : " << makespan() << endl;
	cout << "평균 흐름 시간 : " << flow_time() << endl;
	cout << "기계 가동률 : " << machine_utilization() <<"%"<< endl;
	check_fail();

}