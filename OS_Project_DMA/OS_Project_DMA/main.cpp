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

// task 할당
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
	//Task 갯수 입력
	while (1) {
		cout << "Please Input Count : ";
		cin >> task_num;
		if (task_num < 500)
			cout << "Please Retry ! " << endl;
		else break;
	}
	
	Task* task_input;
	task_input = new Task[task_num];
	//task 할당
	alloc_task(task_input, task_num);
	Controller Control(task_input,task_num);

	//thread 생성
	thread printer{ &Controller::printer,&Control};
	thread monitor{ &Controller::monitor,&Control};
	thread speaker{ &Controller::spearker,&Control };
	thread keyboard{ &Controller::keyboard,&Control};
	thread disk{ &Controller::disk,&Control};

	//thread 종료를 기다린다.
	printer.join();
	monitor.join();
	speaker.join();
	keyboard.join();
	disk.join();

	system("pause");
}