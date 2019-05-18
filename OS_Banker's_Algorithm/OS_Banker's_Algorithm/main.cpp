#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <queue>
using namespace std;

#define SIZE 4
//Resource�� ���� : 4��
struct Resource
{
	int a, b, c, d;
};
//Process ����ü
struct Process
{
	int Pid;
	Resource Max;
	Resource Need;
	Resource Allocation;
};

//������ �����ε�
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

//���μ����� ���� = 4��
Process arr[4];
//�ڿ��� �ѷ� 
Resource Total_Resource;
//�̿� ������ �ڿ��� ��
Resource Available;
//�湮 �ߴ��� Ȯ��
bool Finish[4] = { false, };

//process �ʿ��� resource �Է�
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

	//Need ���
	P.Need.a = P.Max.a - P.Allocation.a;
	P.Need.b = P.Max.b - P.Allocation.b;
	P.Need.c = P.Max.c - P.Allocation.c;
	P.Need.d = P.Max.d - P.Allocation.d;

	//Need ���
	printf("Process %d [Need] : ", (P.Pid));
	cout << P.Need.a << " " << P.Need.b << " " << P.Need.c << " " << P.Need.d << endl;

	//Available ���
	Available.a -= P.Allocation.a;
	Available.b -= P.Allocation.b;
	Available.c -= P.Allocation.c;
	Available.d -= P.Allocation.d;

}

//Need�� Available ũ�� ��
bool comp_Resouce(Resource Need) {
	if (Need.a <= Available.a && Need.b <= Available.b && Need.c <= Available.c && Need.d <= Available.d)
		return true;
	else return false;
}

int main() {
	//Total Resouce�� 15�� �����ش�.
	Total_Resource.a = 15;
	Total_Resource.b = 15;
	Total_Resource.c = 15;
	Total_Resource.d = 15;
	//Available�� ����ϱ� ���� �ʱⰪ : total resource�� �����.
	Available = Total_Resource;

	//�� ���μ����� �ʿ��� resource�� �Է��Ѵ�.
	for (int i = 0; i < SIZE; i++) {
		arr[i].Pid = i + 1;
		input(arr[i]);
	}
	//Avaliable ���
	cout << "\nAvailable Resource : " << Available.a << " " << Available.b << " " << Available.c << " " << Available.d << endl;

	//index�� �����ϱ� ���� ����
	int i = 0;

	int safe_count = 0;
	queue<int> safety_state;
	while (i < SIZE) {
		//Need�� Available���� �۰�, ���� �湮���� ���� ������ ã�´�.
		if (!Finish[i] && comp_Resouce(arr[i].Need)) {
			//�̿��ߴٰ� �����ϰ� Available�� �ڿ��� ����� ��ȯ�� ���� �����ش�
			Available = (Available + arr[i].Allocation);
			//process�� ó�������Ƿ� finish�� true�� �ٲ��ش�.
			Finish[i] = true;
			//safety_state�� process�� �߰��Ѵ�.
			safety_state.push(arr[i].Pid);
			//index �ʱ�ȭ
			i = 0;
			safe_count++;
		}
		//���� ���ǿ� �ش����� �ʴ� ��� ���� process�� �Ѿ��.
		else
			i++;
	}

	//��� process�� finish�� true�� �ƴ� ���
	if (safe_count != SIZE)
		cout << "Safety State�� �������� �ʽ��ϴ�." << endl;
	//safety_state�� �����ϴ� ��� ������ ���
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