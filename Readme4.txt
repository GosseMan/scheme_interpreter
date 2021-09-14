구현환경 : visual studio 2017
입력은 폴더 내의 input.txt 파일에서 받아오며 형식은 다음과 같습니다.
--------------------------------------------------------------------
starting point
vertex수
edge수
edge(weight, from, to)
edge(weight, from, to)
...

ex)
0
3
2
1, 0, 2
3, 1, 2

현재 main에서 input을 받아오면, undirected로 인식하여 adjacency list를 생성한 후 list를 출력하고
DFS, BFS, MST를 각각 실행하여 출력합니다.
이후 directed로 인식하여 adjacency list를 생성한 후 출력하고
ShortestPath를 실행하여 출력합니다.

DFS는 시작점부터 DFS tree를 만들어가며 모든 vertex를 포함할 수 있도록 필요할 경우 여러개의 tree를 생성합니다.
BFS는 시작점부터 하나의 BFS tree만을 생성합니다.
MST 실행시에 minimum spanning tree를 생성할 수 없으면 "no spanning tree" 를 출력합니다.
ShortestPath 실행시에 각 vertex에 대해 shortest path와 경로의 cost를 보여줍니다. 두 vertex가 연결되어있지 않으면 "Doesn't connecte" 를 출력합니다.