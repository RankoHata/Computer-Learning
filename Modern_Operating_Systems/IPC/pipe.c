/* 管道文件 */
#include<stdio.h>
#include<unistd.h>
#include<wait.h>

int main(){
    int p1, fd[2];
    char outpipe[50];  // 定义读缓冲区

    char inpipe[50] = "This is a message from child!";  // 定义写缓冲区
    pipe(fd);
    while((p1 = fork()) == -1);
    if(p1 == 0){  // 子进程中写
        write(fd[1], inpipe, 50);
    }
    else{  // 父进程中读
        wait(0);
        read(fd[0], outpipe, 50);
        printf("%s\n", outpipe);
    }
    return 0;
}
