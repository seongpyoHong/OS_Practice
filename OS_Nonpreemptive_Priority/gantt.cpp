#include <iostream>
#include <vector>
#include <cstdio>
#include <queue>
#include <string>
#include <algorithm>
using namespace std;


//�۾� ������ ��Ÿ���� ����ü
struct task {
	//���� �̸�
	string f_name;
	//�۾��� �ɸ��� �ð�
	int time;
};

//��谡 �۾� ������ ��Ÿ���� ����ü
struct machine
{
	//�۾� ���� �ð�
	int start_time;
	//�۾� ���� �ð�
	int end_time;
	// �۾� ��ȣ
	int alloc_job = 0;
};

//vector �񱳸� ���� �Լ�
bool compare(machine A, machine B) {
	return A.start_time < B.start_time;
}

//���� ���� ��Ȳ
vector<machine> factory[3];

//job ���
queue<task> job[5];

//�� ó���ð�
int process_time() {
	//��� �۾��� ����ð� - ���۽ð��� ���Ͽ� �� ó���ð��� ���Ѵ�.
	int result = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < factory[i].size(); j++) {
			result += (factory[i][j].end_time - factory[i][j].start_time);
		}
	}
	return result;
}
//�� �Ϸ�ð�
int makespan() {
	//�Ϸ�ð� �� ���� ū ���� ����
	int result = 0;
	for (int i = 0; i < 3; i++) {
		int end = factory[i].size() - 1;
		result = max(result, factory[i][end].end_time);
	}
	return result;
}

//�� ���ð�
int latency_time() {
	int result = 0;
	//�� �۾����� �з��ϱ� ���� ����
	vector<machine> job_latency[5];
	//�� �۾� �� latency�� ����
	int latency[5] = { 0 };
	//�� �۾����� �з�
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < factory[j].size(); k++) {
				if (factory[j][k].alloc_job == i+1) {
					job_latency[i].push_back(factory[j][k]);
				}
			}
			//start_time ������ ����
			sort(job_latency[i].begin(), job_latency[i].end(), compare);
		}

		//�� �۾����� �߰� ���� �ð��� ó�� offset�� �����ش�.
		for (int index = 0; index < job_latency[i].size() - 1; index++) {
			if (index == 0)
				latency[i] += (job_latency[i][index].start_time);
			latency[i] += (job_latency[i][index + 1].start_time - job_latency[i][index].end_time);
		}

	}

	//total ���ð�
	int total_latency = 0;

	//�� �۾��� latency�� �����ش�.
	for (int i = 0; i < 5; i++) {
		total_latency += latency[i];
	}
	return total_latency;
}
//���޽ð�
int idle_time() {
	//�� ���޽ð�
	int idle = 0;
	// ��� ���� �ð�
	int result = 0;

	//��� ���� �ð��� �����ش�.
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < factory[i].size()-1; j++) {
			result += (factory[i][j + 1].start_time - factory[i][j].end_time);
		}
	}

	//�� �Ϸ�ð����� �� �Ϸ�ð��� ���ش�.
	int total = makespan();
	for (int i = 0; i < 3; i++) {
		int end = factory[i].size() - 1;
		idle += (total - factory[i][end].end_time);
	}
	return result+idle;
}
//�帧 �ð�
int flow_time() {
	//ó���ð�
	int process = process_time();
	//���ð�
	int latency = latency_time();
	//�۾��� ���� ������ �ش�.
	return (process + latency)/5;
}
//��� ������
int machine_utilization() {
	//ó���ð�
	double process = process_time();
	//�� �Ϸ�ð�
	double total = makespan();
	return (process / (total * 3))*100;
}

//���� ���� �۾� ��
void check_fail() {
	//�� ���� �ϼ�
	int deadline[5] = { 3, 2, 4, 4, 2 };
	//�۾��� ������ �ð�
	int job_end[5] = { 0 };

	//�ִ밪���� ����
	for (int i = 0; i < 5; i++) {
		job_end[i] = 0;
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < factory[j].size(); k++) {
				if (factory[j][k].alloc_job == i+1) {
					job_end[i] = max(job_end[i], factory[j][k].end_time);
				}
			}
		}
		//������ ��Ų ��� 0�� �ʱ�ȭ
		if (job_end[i] < (8 * deadline[i])) {
			job_end[i] = 0;
		}
	}
	cout << "���� ���� �۾� �� :  ";
	int cnt = 0;
	//������ ��Ű�� ���� �۾� ���
	for (int i = 0; i < 5; i++) {
		if (job_end[i] != 0) {
			cnt++;
			printf("�۾� %d , ",i+1);
		}
	}
	printf("���� �� %d �� �Դϴ�.\n", cnt);
}

//push�� ������ ã�´�.
int search(int index,task alloc,int offset,int job_num) {

	//�ʱ� ũ��
	int init_size = factory[index].size();
	// �տ� ������ �ִ��� üũ
	bool check = false;
	//factory�� ���鼭 �տ� �� ������ �ִ��� üũ
	for (int i = 0; i < init_size; i++) {
		//offset(���� �۾��� ������ �ð�)���� start�� ũ�� �۾��� ���� �ð����� start_time�� ū ���
		if (alloc.time <= factory[index][i].start_time && offset<= factory[index][i].start_time) {
			//������ �� ������ ũ�Ⱑ �۾��� ����ð����� ũ�ų� ���� ���
			if ((factory[index][i].start_time - factory[index][i - 1].end_time) >= alloc.time) {
				//���ο� �۾� ����
				machine temp;
				temp.start_time = factory[index][i - 1].end_time;
				temp.end_time = temp.start_time + alloc.time;
				temp.alloc_job = job_num;
				factory[index].push_back(temp);
				check = true;
				//start_time ������ ����
				sort(factory[index].begin(), factory[index].end(), compare);
				return temp.end_time;
			}
		}
	}

	//�տ� ���� ������ ���� ���
	if (!check) {
		//���� �ڿ� ����
		machine temp;
		temp.alloc_job = job_num;
		//offset�� ���Ͽ� index�� ����
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

//ù��° �� push
void pushFirst(int index) {
	// ù�� ° �۾� ����ó��
	int init_size = job[index].size();
	int offset = 0;
	int fac_ind = 0;
	for (int i = 0; i < init_size; i++) {
		//���� queue�� ���Ե� �۾����� ó��
		task alloc = job[index].front();
		//���� �з�
		if (job[index].front().f_name == "A") {
			 fac_ind= 0;
		}
		else if (job[index].front().f_name == "B") {
			fac_ind = 1;
		}
		else if (job[index].front().f_name == "C") {
			fac_ind = 2;
		}
		//���忡 �۾� ��ġ
		machine temp;
		temp.start_time = offset;
		temp.end_time = temp.start_time + alloc.time;
		temp.alloc_job = index+1;
		factory[fac_ind].push_back(temp);
		//offset ����
		offset = temp.end_time;
		//queue���� �۾� pop
		job[index].pop();
	}
}

//gantt chart �˰���
void gantt(int order[]) {
	//ù ��° �۾� ����ó��
	pushFirst(order[0] - 1);

	//�� ��° �۾����� ó��
	for (int i = 1; i < 5; i++) {
		int index = order[i] - 1;
		int size = job[index].size();
		int offset = 0;
		//�� job ������� ����
		for (int j = 0; j < size; j++) {
			task alloc = job[index].front();
			//���� �з�
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
	//ù ���� ���� (�ε��� ��Ʈ���� ����)
	for (int i = 0; i < 3; i++) {
		machine temp;
		temp.start_time = 0;
		temp.end_time = 0;
		factory[i].push_back(temp);
	}
	//���� �Է�
	//1��
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

	//2��
	first_one.f_name = "C";
	first_one.time = 6;
	job[1].push(first_one);

	first_two.f_name = "A";
	first_two.time = 4;
	job[1].push(first_two);

	//3��
	first_one.f_name = "B";
	first_one.time = 3;
	job[2].push(first_one);

	first_two.f_name = "C";
	first_two.time = 2;
	job[2].push(first_two);

	first_thr.f_name = "A";
	first_thr.time = 1;
	job[2].push(first_thr);

	//4��
	first_one.f_name = "C";
	first_one.time = 4;
	job[3].push(first_one);

	first_two.f_name = "B";
	first_two.time = 3;
	job[3].push(first_two);

	first_thr.f_name = "A";
	first_thr.time = 3;
	job[3].push(first_thr);

	//5��
	first_one.f_name = "A";
	first_one.time = 5;
	job[4].push(first_one);

	first_two.f_name = "B";
	first_two.time = 3;
	job[4].push(first_two);

	//���� �Է�
	int order[5];
	cout << "�۾��� ������ ������ �Է��ϼ���. =>  ";
	for (int i = 0; i < 5; i++)
		cin >> order[i];

	//gantt chart algorithm
	gantt(order);
	
	//���� �۾� ��ġ ���
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

	//�� �� ��ǥ ���
	cout << "�� ó���ð� : " << process_time() << endl;
	cout << "�� ��� �ð� : " << latency_time() << endl;
	cout << "���� �ð� : " << idle_time() << endl;
	cout << "�� �Ϸ� �ð� : " << makespan() << endl;
	cout << "��� �帧 �ð� : " << flow_time() << endl;
	cout << "��� ������ : " << machine_utilization() <<"%"<< endl;
	check_fail();

}