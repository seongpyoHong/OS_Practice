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
#include <random>
#include <ctime>
#include <thread>
// task �Ҵ�

void alloc_task(Task* task_input, int task_num) {
	srand((unsigned int)time(NULL));
	for (int i = 0; i < task_num; i++) {
		task_input[i].id = i;
		task_input[i].state = (TaskState)(rand() %5 +1);
	}
}
//task state ���
string printState(Task task) {
	switch (task.state) {
	case 1: return "PRINTER";
	case 2: return "MONITOR";
	case 3: return"SPEAKER";
	case 4: return"KEYBOARD";
	case 5: return "DISK";
	}
}
//main
int main() {
	int task_num;
	cin >> task_num;
	Task* task_input;
	task_input = new Task[task_num];
	//task �Ҵ�
	alloc_task(task_input, task_num);
	Controller Control(task_input,task_num);

	//thread ����
	thread printer{ &Controller::printer,&Control ,task_num};
	thread monitor{ &Controller::monitor,&Control,task_num };
	thread speaker{ &Controller::spearker,&Control,task_num };
	thread keyboard{ &Controller::keyboard,&Control ,task_num };
	thread disk{ &Controller::disk,&Control,task_num };

	while (task_num != 0) {
		printer.join();
		monitor.join();
		speaker.join();
		keyboard.join();
		disk.join();
	}
	if (task_num == 0)
		cout << "All of Task is Done!" << endl;
	system("pause");
}