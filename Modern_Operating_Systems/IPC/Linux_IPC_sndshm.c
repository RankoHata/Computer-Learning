/* 共享内存的发送程序
先运行发送程序，再运行接收程序 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/shm.h>


int main(){
    int shmid;  // 共享内存的内部标识
    char *viraddr;  // 附接到共享内存的虚拟地址
    char buffer[BUFSIZ];

    /* 创建共享内存 */
    shmid = shmget(1234, BUFSIZ, 0666|IPC_CREAT);
    /* 附接到进程的虚拟地址空间 */
    viraddr = (char *)shmat(shmid, 0, 0);
    while(1){
        puts("Enter some text:");
        fgets(buffer, BUFSIZ, stdin);
        strcat(viraddr, buffer);  // 追加到共享内存
    
        if(strncmp(buffer, "end", 3) == 0)  // 输入end结束
            break;
    }
    shmdt(viraddr);  // 断开连接
    return 0;
}