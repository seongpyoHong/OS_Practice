/*
	Senario 
	1. Input���� Task ������ �Է� (Task�� ���� ������ �Ҵ�)
	2. �� IDE Controller���� �ش��ϴ� Task�� �Ҵ�
	3. IDE Controller�� �Ҵ�� Task�� ó�� ��, DMA Controller���� ���� 
		(�� ���� �ϳ��� task�� ����, ��� ���� task�� queue�� ����)
	4. DMA Controller�� Memory Buffer�� ������� ����
	5. ��� IDE Controlller�� ����Ǹ� Output���� �˸�
*/
#include "Controller.h"

// task �Ҵ�
void alloc_task(Task* task_input, int task_num) {
	srand((unsigned int)time(NULL));
	for (int i = 0; i < task_num; i++) {
		task_input[i].id = i;
		task_input[i].state = (TaskState)(rand() %5);
	}
}

//main
int main() {
	int task_num;
	//Task ���� �Է�
	while (1) {
		cout << "Please Input Count : ";
		cin >> task_num;
		if (task_num < 500)
			cout << "Please Retry ! " << endl;
		else break;
	}
	
	Task* task_input;
	task_input = new Task[task_num];
	//task �Ҵ�
	alloc_task(task_input, task_num);
	Controller Control(task_input,task_num);

	//thread ����
	thread printer{ &Controller::printer,&Control};
	thread monitor{ &Controller::monitor,&Control};
	thread speaker{ &Controller::spearker,&Control };
	thread keyboard{ &Controller::keyboard,&Control};
	thread disk{ &Controller::disk,&Control};

	//thread ���Ḧ ��ٸ���.
	printer.join();
	monitor.join();
	speaker.join();
	keyboard.join();
	disk.join();

	system("pause");
}