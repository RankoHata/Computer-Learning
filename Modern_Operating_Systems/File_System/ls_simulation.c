#include<stdio.h>
#include<stdlib.h>
#include<string.h>  // 使用strncmp
#include<dirent.h>  // 目录操作

int list(DIR *dir){
    struct dirent *pdir;  // 结构体指针
    while((pdir = readdir(dir)) != NULL){  // 读目录
        /* strncmp 把以.开头的文件名全部过滤，包括 . or .gitignore等 */
        if(strncmp(pdir->d_name, ".", 1) == 0) continue;
        printf("%s\n", pdir->d_name);
    }
    printf("\n");
    return 0;
}

int main(int argc, char **argv){
    DIR *dir;
    if(argc < 2){
        fprintf(stderr, "Usage: %s argv[1].\n", argv[0]);  // 没有输入
        return -1;
    }
    if((dir = opendir(argv[1])) == NULL){  // 打不开目录
        perror("Fail to opendir.");
        return -1;
    }
    list(dir);
    return 0;
}