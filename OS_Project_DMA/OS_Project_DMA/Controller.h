#pragma once
#include <iostream>
#include <thread>
#include <queue>
#include <vector>
#include <string>
using namespace std;

struct Task {
	int id = 0;
	enum TaskState state;
};
enum TaskState {
	PRINTER, MONITOR, SPEAKER, KEYBOARD, DISK
};

class Controller {	
public:
	//Memory Buffer
	vector<Task> Memory_Buffer;
	//Printer queue
	queue<Task> Printer_Queue;
	//Monitor queue
	queue<Task> Monitor_Queue;
	//Speaker queue
	queue<Task> Speaker_Queue;
	//Keyboard queue
	queue<Task> Keyboard_Queue;
	//Disk queue
	queue<Task> Disk_Queue;
	//task_num
	int task_cnt;
	Controller(){};
	// 생성자 overiding : task를 각 Queue에 분배
	Controller(Task* task, int task_num) {
		task_cnt = task_num;
		for (int i = 0; i < task_num; i++) {
			if (task[i].state == 1)
				Printer_Queue.push(task[i]);
			else if (task[i].state == 2)
				Monitor_Queue.push(task[i]);
			else if (task[i].state == 3)
				Speaker_Queue.push(task[i]);
			else if (task[i].state == 4)
				Keyboard_Queue.push(task[i]);
			else
				Disk_Queue.push(task[i]);
		}
	}
	//IDE Controller : Printer
	static void printer(int& cnt) {

		if (Printer_Queue.size() != 0) {
			Task temp = Printer_Queue.back();
			cout << "\nOperate Print < pId : " << temp.id<<" >"<<endl;
			Printer_Queue.pop();
			cnt--;
			DMA_controller(temp);
		}
		else 
			cout << "\nAll of Print Task is done" << endl;
	}

	//IDE Controller : Monitor
	static void monitor(int& cnt) {
		if(Monitor_Queue.size() != 0) {
			Task temp = Monitor_Queue.back();
			cout << "\nOperate Monitor < pId : " << temp.id << " >" << endl;
			Monitor_Queue.pop();
			cnt--;
			DMA_controller(temp);
		}
		else 
			cout << "\nAll of Monitor Task is done" << endl;
	}

	//IDE Controller : Speaker
	static void spearker(int& cnt) {
		if (Speaker_Queue.size() != 0) {
			Task temp = Speaker_Queue.back();
			cout << "\nOperate Speaker < pId : " << temp.id << " >" << endl;
			Speaker_Queue.pop();
			cnt--;
			DMA_controller(temp);
		}
		else
			cout << "\nAll of Speaker Task is done" << endl;
	}

	//IDE Controller : Keyboard
	static void keyboard(int& cnt) {
		if (Keyboard_Queue.size() != 0) {
			Task temp = Keyboard_Queue.back();
			cout << "\nOperate Keyboard < pId : " << temp.id << " >" << endl;
			Keyboard_Queue.pop();
			cnt--;
			DMA_controller(temp);
		}
		else
			cout << "\nAll of Keyboard Task is done" << endl;
	}

	//IDE Controller : Disk
	void disk(int& cnt) {
		if (Disk_Queue.size() != 0) {
			Task temp = Disk_Queue.back();
			cout << "\nOperate Disk < pId : " << temp.id << " >" << endl;
			Disk_Queue.pop();
			cnt--;
			DMA_controller(temp);
		}
		else
			cout << "\nAll of Disk Task is done" << endl;
	}

	//DMA Controller
	void DMA_controller(Task task) {
		Memory_Buffer.push_back(task);
	}
};