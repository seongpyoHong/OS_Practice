#pragma once
#include <iostream>
#include <thread>
#include <queue>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>
#include <iomanip>
#include <random>
#include <ctime>
using namespace std;
using namespace std::literals;

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
	//mutex 
	mutex mtx_lock;
	Controller(){};
	// ������ overiding : task�� �� Queue�� �й�
	Controller(Task* task, int task_num) {
		task_cnt = task_num;
		cout << "=========================< Task Allocate >========================\n";
		for (int i = 0; i < task_num; i++) {
			//�� IDE Controller Queue�� �й�
			if (task[i].state == 0)
				Printer_Queue.push(task[i]);
			else if (task[i].state == 1)
				Monitor_Queue.push(task[i]);
			else if (task[i].state == 2)
				Speaker_Queue.push(task[i]);
			else if (task[i].state == 3)
				Keyboard_Queue.push(task[i]);
			else
				Disk_Queue.push(task[i]);
			//Task �Ҵ� ���
			cout << "pId : " << std::left << setw(4) << task[i].id << " | State : " << setw(8) << printState(task[i]) << endl;
		}
		cout << "=========================< Task Processing >========================\n";
	}
	//IDE Controller : Printer
	void printer() {
		int init_cnt = Printer_Queue.size();
		for (int i = 0; i < init_cnt; i++) {
			//IDE Controller�� ������ ���´�. => Thread 1�ʰ� Sleep
			if(rand()%5==0)
				std::this_thread::sleep_for(0.1s);
			//mutex ����
			std::lock_guard<std::mutex> guard(mtx_lock);
			if (Printer_Queue.size() != 0) {
				//���� ���� ������ ť���� task ó��
				Task temp = Printer_Queue.front();
				cout << std::left << setw(22) << "Processing Print" << " < pId : " << temp.id << " >" << endl;
				//ó���� task�� ť���� ����
				Printer_Queue.pop();
				std::this_thread::sleep_for(0.01s);
				//ó���� �Ϸ�Ǹ� DMA Controller�� ����
				DMA_controller(temp);
			}
			else
				cout << "\nAll of Print Task is done" << endl;
		}
	}

	//IDE Controller : Monitor
	void monitor() {
		int init_cnt = Monitor_Queue.size();
		for (int i = 0; i <init_cnt; i++) {
			//IDE Controller�� ������ ���´�. => Thread 1�ʰ� Sleep
			if (rand() % 5 == 0)
				std::this_thread::sleep_for(0.1s);

			if (Monitor_Queue.size() != 0) {
				//mutex ����
				std::lock_guard<std::mutex> guard(mtx_lock);
				//���� ���� ������ ť���� task ó��
				Task temp = Monitor_Queue.front();
				cout << std::left << setw(22) << "Processing Monitor" << " < pId : " << temp.id << " >" << endl;
				//ó���� task�� ť���� ����
				Monitor_Queue.pop();
				std::this_thread::sleep_for(0.01s);
				//ó���� �Ϸ�Ǹ� DMA Controller�� ����
				DMA_controller(temp);
			}
			else
				cout << "\nAll of Monitor Task is done" << endl;
		}
	}

	//IDE Controller : Speaker
	void spearker() {
		int init_cnt = Speaker_Queue.size();
		for (int i = 0; i < init_cnt; i++) {
			//IDE Controller�� ������ ���´�. => Thread 1�ʰ� Sleep
			if (rand() % 5 == 0)
				std::this_thread::sleep_for(0.1s);

			if (Speaker_Queue.size() != 0) {
				//mutex ����
				std::lock_guard<std::mutex> guard(mtx_lock);
				//���� ���� ������ ť���� task �Ҵ�
				Task temp = Speaker_Queue.front();
				cout << std::left << setw(22) << "Processing Speaker" << " < pId : " << temp.id << " >" << endl;				
				//�Ҵ�� task�� ť���� ����
				Speaker_Queue.pop();
				std::this_thread::sleep_for(0.01s);
				//ó���� �Ϸ�Ǹ� DMA Controller�� ����
				DMA_controller(temp);
			}
			else
				cout << "\nAll of Speaker Task is done" << endl;
		}	
	}

	//IDE Controller : Keyboard
	void keyboard() {
		int init_cnt = Keyboard_Queue.size();
		for (int i = 0; i < init_cnt; i++) {
			//IDE Controller�� ������ ���´�. => Thread 1�ʰ� Sleep
			if (rand() % 5 == 0)
				std::this_thread::sleep_for(0.1s);

			if (Keyboard_Queue.size() != 0) {
				//mutex ����
				std::lock_guard<std::mutex> guard(mtx_lock);
				//���� ���� ������ ť���� task ó��
				Task temp = Keyboard_Queue.front();
				cout << std::left << setw(22) << "Processing Keyboard" << " < pId : " << temp.id << " >" << endl;
				//ó���� task�� ť���� ����
				Keyboard_Queue.pop();
				std::this_thread::sleep_for(0.01s);
				//ó���� �Ϸ�Ǹ� DMA Controller�� ����
				DMA_controller(temp);
			}
			else
				cout << "\nAll of Keyboard Task is done" << endl;
		}
	}

	//IDE Controller : Disk
	void disk() {
		int init_cnt = Disk_Queue.size();
		for (int i = 0; i < init_cnt; i++) {
			//IDE Controller�� ������ ���´�. => Thread 1�ʰ� Sleep
			if (rand() % 5 == 0)
				std::this_thread::sleep_for(0.1s);

			if (Disk_Queue.size() != 0) {
				//mutex ����
				std::lock_guard<std::mutex> guard(mtx_lock);
				//���� ���� ������ ť���� task ó��
				Task temp = Disk_Queue.front();
				cout << std::left << setw(22) << "Processing Disk" << " < pId : " << temp.id << " >" << endl;				
				//ó���� task�� ť���� ����
				Disk_Queue.pop();
				std::this_thread::sleep_for(0.01s);
				//ó���� �Ϸ�Ǹ� DMA Controller�� ����
				DMA_controller(temp);
			}
			else
				cout << "\nAll of Disk Task is done" << endl;
		}
	}

	//DMA Controller : Memory Buffer�� ����
	void DMA_controller(Task task) {
		//Memory Buffer�� ����
		Memory_Buffer.push_back(task);
		//task�� ���� ������ ��� Memory buffer ���
		if (Memory_Buffer.size() == task_cnt) {
			cout <<"\nAll of task are done!" << endl;
			cout << "\n========================< Memory Buffer >========================\n";
			for (int i = 0; i < task_cnt; i++) {
				cout << "pId : " <<setw(3)<< Memory_Buffer[i].id << " | Task State : " << printState(Memory_Buffer[i]) << endl;
				}
		}
	}
	//State enum�� ����ϱ� ���� �Լ�
	string printState(Task task) {
		switch (task.state) {
		case 0: return "PRINTER";
		case 1: return "MONITOR";
		case 2: return"SPEAKER";
		case 3: return"KEYBOARD";
		case 4: return "DISK";
		}
	}
};