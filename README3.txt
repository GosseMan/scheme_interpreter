컴파일 환경
- Visual Studio 2019

read하는 중 메모리가 부족하여 allocate할 수 없으면 바로 garbage collector를 실행하고 현재 작성중이던 tree를
free list에 이어서 작성하였습니다.
(example에서 미리 garbage collector를 수행하고 앞에서부터 tree를 만든 것과 출력에 차이가 있습니다.)