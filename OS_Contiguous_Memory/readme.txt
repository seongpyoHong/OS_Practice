2015104232 홍성표 Assignment 02
Contiguous Memory Allocation

시나리오
1. 전체 Memory size를 입력받는다.
2. 최대 Process의 개수를 입력받는다.
3. 프로세스와 할당될 메모리를 입력받는다.
   - Input Form : [Process Id] [Allocation Size]
   - Allocation Size : 0 일 경우 해당 프로세스가 메모리에서 해제됨을 의미한다.
   - 종료를 위한 input 값은 -1 -1
4. 입력 값을 순서대로 처리한다.
   - 전체 메모리에서 할당될 수 있는 공간이 있다면 프로세스를 넣어준다.
   - 전체 메모리에서 할당될 수 있는 공간이 없다면 Compaction과 Coalescing을 진행한 후 넣어준다. (이후에도 넣을 수 없다면 에러메세지 출력)
   - 프로세스를 메모리에서 해제할 때, 연속된 빈공간이 존재할 수 있으므로 Coalescing을 진행한 후 넣어준다.
5. 모든 프로세스의 할당 및 해제가 완료되면 최종 상태를 출력한다.

- Compaction
메모리를 정리하기 위한 작업
작업을 수행하면서 움직이는 비용이 가장 적게 발생하는 작업을 선택해서 동작
1. 메모리의 0K부터 빈공간을 찾고 End_Address부터 할당된 공간을 찾는다.
2. 할당된 공간의 크기 <= 빈 공간의 크기, 비어있는 공간에 할당된 공간을 분배
3. 모든 작업이 완료된 후 공백이 존재하지 않도록 위로 올리는 작업 실행
- Coalescing
연속된 빈 공간이 존재할 경우, 연결되었음을 표시한다.