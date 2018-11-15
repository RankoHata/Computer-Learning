// #include<stdio.h>
// #include<stdlib.h>
// #include<stdbool.h>

// #define bufsize 120

// FILE *fp = NULL;  // 代码文件指针
// char buff0[bufsize], buff1[bufsize];  // 双缓冲区
// int start_index = 0;  // 起始位置
// int search_index = 0;  // 搜索位置
// int curr_buf = 0;  // 当前缓冲区
// int end_index = 0;  // 记录读入字符的结束index
// int end_flag = 0;  // 结束标志

// void use_buf(){  // 将数据读入缓冲区
//     char tmp;
//     for(int i = 0; i < bufsize; i++){
//         if((tmp = getc(fp)) != EOF){
//             if(curr_buf == 0){
//                 buff1[i] = tmp;
//             }
//             else{
//                 buff0[i] = tmp;
//             }
//             end_index = i;  // update
//         }
//         else{
//             break;
//         }
//     }
//     curr_buf = (curr_buf + 1) % 2;  // 切换缓冲区
// }

// char get_char_from_buff(){
//     if(curr_buf == 0){

//     }
// }

// int main(){
//     char tmp;

//     if((fp = fopen("test.c", "rt")) == NULL){
//         exit(1);
//     }

//     for(int i = 0; i < bufsize; i++){  // 打开文件之后，先向0号缓冲区注入数据
//         if((tmp = getc(fp)) != EOF){
//             buff0[i] = tmp;
//             end_index = i;
//         }
//         else{
//             break;
//         }
//     }

//     curr_buf = 0;  // 当前使用0号缓冲区
//     while(end_flag == 0){
//         if(curr_buf == 0){
//             for(int i = 0; i <= end_index; i++){
//                 if(buff0[i] == '/' && buff0[])
//             }
//         }
//     }

//     return 0;
// }


#include<stdio.h>
#include<string.h>

#define MAXWORD 256
#define BUFSIZE 1024
#define BINBUFSIZE 2048
#define ENDFLAG '#'

struct binbuffer{
    char *buffer;  // 双缓冲区大小
    char *start, *forward;
    int count;  // 记录正在读的字符数
} bbuf;

void parse(char str){
    ;
}

int main(){
    FILE *fp;
    char word[MAXWORD];
    int read_size = 0;
    int end_index = 0;  // 结束字符后的index
    int end_flag = 0;
    char buffer[BINBUFSIZE];

    fp = open("test.c", "r");
    read_size = read(fp, buffer, BUFSIZE);
    if(read_size < BUFSIZE){
        end_flag = 1;
        end_index = read_size;
    }
    // buffer[BUFSIZE - 1] = ENDFLAG;
    // read(fp, buffer + BUFSIZE, BUFSIZE - 1);
    // buffer[BINBUFSIZE - 1] = ENDFLAG;
    
    bbuf.buffer = buffer;
    bbuf.start = bbuf.forward = bbuf.buffer;
    bbuf.count = 0;

    while(1){
        bbuf.forward = bbuf.forward + 1;  // 将地址向前推进
        bbuf.count ++;
        if(end_flag == 1 && end_index == bbuf.count - 1){
            break;  // 词法分析结束
        }
        if(bbuf.count == 1024){  // 正在读第1024个字符，缓冲区1的最后一个字符
            read_size = read(fp, buffer + BUFSIZE, BUFSIZE);
            if(read_size < BUFSIZE){
                end_flag = 1;
                end_index = read_size + BUFSIZE;
            }
            parse(*bbuf.forward);
        }
        else if(bbuf.count == 2048){  // 正在读缓冲区2的最后一个字符
            read_size = read(fp, buffer, BUFSIZE);
            if(read_size < BUFSIZE){
                end_flag = 1;
                end_index = read_size;
            }
            parse(*bbuf.forward);
            bbuf.count == 0;
            bbuf.forward = bbuf.buffer;
            if(end_flag == 1 && end_index == 0){  // 正好读完,不必再解析
                break;
            }
            else{
                bbuf.count ++;
                parse(*bbuf.forward);  // 如果到结尾,一口气分析两个
            }
        }
        else{
            parse(*bbuf.forward);
        }
    }
}