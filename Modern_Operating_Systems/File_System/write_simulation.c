/* 文件操作 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>

int main(){
    int fd0, record_len, buff_len, cnt;
    char filename[50], f_buff[2000];

    strcpy(filename, "./example.txt");  // 为文件名赋值
    fd0 = open(filename, O_RDWR | O_CREAT, 0644);  /* 打开文件 */
    if(fd0 < 0){
        printf("Can't create example.txt file!\n");
        exit(0);
    }
    buff_len = 20;
    record_len = 10;

    lseek(fd0, 0, SEEK_SET);  // 定位到文件开始处

    /* 读文件，长度为record_len * buff_len的内容到f_buff中 */
    cnt = read(fd0, f_buff, record_len * buff_len);
    cnt = cnt / record_len;
    printf("%s\n", f_buff);  // 输出缓冲区数据

    strcpy(f_buff, "1234567890");  // 写入的信息
    printf("%s\n", f_buff);  // strcpy 拼接了字符串
    write(fd0, f_buff, strlen(f_buff));  // 写入
    write(fd0, "\n\r", 2);
    close(fd0);
    return 0;
}