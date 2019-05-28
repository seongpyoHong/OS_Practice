# OS_Practice
### Project 01 Buffer Cache

#### 프로젝트 시나리오

1. Mod Number를 입력
2. Buffer의 개수를 입력
3. 초기상태를 출력 
   - 각 버퍼의 값과 상태는 랜덤하게 할당
   - 상태가 DELAY인 버퍼는 Freelist에 연결
4. 찾고 싶은 값을 입력
5. 찾는 과정 중에 거치는 시나리오들을 출력 => 시나리오를 통해 바뀐 HashQueue의 상태 출력
6. 4 - 5를 반복 (-1을 입력 할 경우 프로그램 종료)



#### 각 시나리오 설명

1. HashQueue에 존재하지만 FreeList에도 존재할 경우
    FreeList에서 제거 후 값을 가져온다. 

2. HashQueue에 찾는 값이 없고, FreeList가 비어있지 않는 경우
    FreeList에서 첫번째 값을 삭제한 후 새로운 block을 만든다.

3. HashQueue에 찾는 값이 없고, FreeList의 첫번째 상태가 DELAY인 경우
    delay인 위치를 건너 뛴 후 값을 삭제, 새로운 block을 만든다.

4. HashQueue에 찾는 값이 없고, FreeList가 비어있는 경우
    sleep -> 이 프로젝트는 하나의 process만 존재하므로 3초 sleep 후 FreeList에 임의의 값을 생성

5. HashQueue에 찾는 값이 있지만, busy인 경우
    sleep -> 이 프로젝트는 하나의 process만 존재하므로 3초 sleep 후 block의 상태를 free로 변경

---
### Project 02 DMA Controller

#### 개요
CPU를 데이터의 이동에 대해 자유롭도록 데이터의 전송을 전담하는 DMA Controller를 구현

#### 시나리오
    - Task : pid와 state를 가지고 있다.
    - Task's State : PRINT, SPEAKER, MONITOR, KEYBOARD, DISK

    1. Task의 개수(최소 500개 이상)를 입력
        (Task의 종류는 무작위로 할당)
    
    2. 입력 받은 각 Task를 해당하는 IDE Controller에 할당
        (대기 중인 큐는 각 IDE Controller의 Queue에 존재)
        (할당된 Task들을 출력)
    
    3. IDE Controller가 할당된 Task를 처리 후, DMA Controller에게 처리가 완료되었음을 알림
        (각 IDE Controller는 독립적으로 처리, 한 번에 하나의 Task만 처리 )
        (IDE Controller의 처리과정을 출력)
    
    4. DMA Controller는 IDE가 처리한 Task를 Memory buffer에 할당
    
    5. 모든 Task가 완료되었을 떄 Memory Buffer를 순서대로 출력하고 종료
---
### Assignment 01 Banker's Algorithm
### 프로그램 시나리오

Resource 개수 : 4개
Process 개수 : 4개
Total Resource : (15,15,15,15)

1. 각 Process의 Max / Allocation을 입력
2. 각 Process의 Need (Max - Allocation)을 계산
3. Available Resource 계산 (Total Resource - 모든 Process의 Allocation)
3. Banker's 알고리즘에 의해 Safety State가 존재할 경우, 순서를 출력
   존재하지 않을 경우, 에러 메세지 출력

### Banker's Algorithm
Finish[i] : i번째 Process가 실행되었는지 확인. Default = False
1. Finish[i]가 false이고 i번쨰 Process의 Need가 Available보다 작을 경우
자원을 할당 및 사용 후 반납
2. Available : Available + 반납한 Resource
3. 모든 Process가 사용된 경우 Safety State가 존재한다고 할 수 있다.
---
### Assignment 02 Contiguous Memory Allocation
### 프로그램 시나리오
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

## Compaction
메모리를 정리하기 위한 작업
작업을 수행하면서 움직이는 비용이 가장 적게 발생하는 작업을 선택해서 동작
1. 메모리의 0K부터 빈공간을 찾고 End_Address부터 할당된 공간을 찾는다.
2. 할당된 공간의 크기 <= 빈 공간의 크기, 비어있는 공간에 할당된 공간을 분배
3. 모든 작업이 완료된 후 공백이 존재하지 않도록 위로 올리는 작업 실행

## Coalescing
연속된 빈 공간이 존재할 경우, 연결되었음을 표시