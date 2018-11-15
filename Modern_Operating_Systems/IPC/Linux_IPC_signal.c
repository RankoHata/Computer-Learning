/* 信号处理机制 
有信号处理机制，程序运行后，不断显示 Hello 字样，
按下 ctrl +c，进程接受到信号后，
将 k 置 0，跳出循环后打印 OK，进程正常终止
*/
#include<stdio.h>
#include<unistd.h>
#include<signal.h>

int k;  // 循环变量

void int_func(int sig){
    k = 0;
}

int main(){
    signal(SIGINT, int_func);  // 当Ctrl-C时,k = 0
    k = 1;
    while(k == 1){
        printf("Hello!\n");
    }
    printf("OK!\n");
    printf("%d\n", getppid());
    return 0;
}