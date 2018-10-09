/* 完成互斥的Peterson算法 */

#define FALSE 0
#define TRUE 1
#define N 2  // 进程数

int turn;
int interested[N];

void enter_region(int process){  // 进程0或1
    int other;                   // 另一进程号

    other = 1 - process;
    interested[process] = TRUE;
    turn = process;              // 设置标志
    while(turn == process && interested[other] == TRUE);  //关键语句
}

void leave_region(int process){
    interested[process] = FALSE;  // 离开临界区
}

/* 当两个进程几乎同时调用enter_region,turn被分别赋值,只有后保存进去的进程号才有效,
前一个因为turn重写而丢失.若进程1后写入,则turn为1.当两个进程同时运行至while时,进程0将
循环零次并进入临界区,进程1将不停循环直到进程0退出临界区才能进入临界区. */