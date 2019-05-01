/*
	Senario 
	1. Input으로 Task 개수를 입력 (Task의 종류 무작위 할당)
	2. 각 IDE Controller에게 해당하는 Task를 할당
	3. IDE Controller는 할당된 Task를 처리 후, DMA Controller에게 전달 
		(한 번에 하나의 task만 수행, 대기 중인 task는 queue에 저장)
	4. DMA Controller는 Memory Buffer에 순서대로 저장
	5. 모든 IDE Controlller가 종료되면 Output으로 알림
*/

#include <iostream>
#include <thread>
#include <queue>
#include <vector>
#include <string>
using namespace std;

//Memory Buffer
vector<Task> Memory_Buffer;
//Printer queue
queue<Task> Printer_Queue;
//Monitor queue
queue<Task> Monitor_Queue;
//Speaker queue
queue<Task> Speaker_Queue;
//Keyboard queue
queue<Task> Keyboaed_Queue;
//Disk queue
queue<Task> Disk_Queue;

//Task Struct
struct Task {
	int id = 0;
	string state;
};

enum TaskState {
PRINTER, MONITOR,SPEAKER,KEYBOARD,DISK
};

//IDE Controller
void printer() {

}
void monitor() {

}
void mouse() {

}
void spearker() {

}
void keyboard() {

}
void disk() {

}
//DMA Controller
void DMA_controller() {

}
//main
int main() {

}