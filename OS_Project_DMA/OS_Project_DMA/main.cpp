/*
	Senario 
	1. Input���� Task ������ �Է� (Task�� ���� ������ �Ҵ�)
	2. �� IDE Controller���� �ش��ϴ� Task�� �Ҵ�
	3. IDE Controller�� �Ҵ�� Task�� ó�� ��, DMA Controller���� ���� 
		(�� ���� �ϳ��� task�� ����, ��� ���� task�� queue�� ����)
	4. DMA Controller�� Memory Buffer�� ������� ����
	5. ��� IDE Controlller�� ����Ǹ� Output���� �˸�
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