#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>

#define BUF_SIZE 1024*8  // 缓冲区大小

int main(int argc, char **argv){
    int fds, fdd;  // source + destination
    char buf[BUF_SIZE];

    size_t count = 0;  // 通常是 typedef unsigned int size_t

    fds = open(argv[1], O_RDONLY);  // 只读模式
    fdd = open(argv[2], O_WRONLY | O_CREAT);  // 只写 + 创建
    if(fds && fdd){  // 两个文件都成功打开
        while((count = read(fds, buf, sizeof(buf))) > 0){  // 读入数据
            write(fdd, buf, count);
        }
    }
    close(fds);
    close(fdd);
}
