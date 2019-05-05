/*
	Senario 
	1. Input으로 Task 개수를 입력 (Task의 종류 무작위 할당)
	2. 각 IDE Controller에게 해당하는 Task를 할당
	3. IDE Controller는 할당된 Task를 처리 후, DMA Controller에게 전달 
		(한 번에 하나의 task만 수행, 대기 중인 task는 queue에 저장)
	4. DMA Controller는 Memory Buffer에 순서대로 저장
	5. 모든 IDE Controlller가 종료되면 Output으로 알림
*/
#include "Controller.h"
#include <random>
#include <ctime>
#include <thread>
// task 할당

void alloc_task(Task* task_input, int task_num) {
	srand((unsigned int)time(NULL));
	for (int i = 0; i < task_num; i++) {
		task_input[i].id = i;
		task_input[i].state = (TaskState)(rand() %5 +1);
	}
}
//task state 출력
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
	//task 할당
	alloc_task(task_input, task_num);
	Controller Control(task_input,task_num);

	//thread 생성
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