/* 页面置换算法 
FIFO LRU OPT
*/

#include<stdio.h>
#include<stdlib.h>

#define M 12

// 从文件读入reqeust序列
void init_request(int request[], char filename[]){
    const int num = 10;  // 最大读几位
    int tmp_num;  // 当前数字有几位
    FILE *fp = fopen(filename, "r");
    char tmp_str[num+1];
    int tmp_ten;  // 10的几次方
    int cur_num = 0;  // 当前数字下标
    while(fgets(tmp_str, num, fp) && *tmp_str != EOF){  // 不能合在一起写貌似，不会报错，但会莫名中断程序
        if(cur_num >= M) break;
        request[cur_num] = 0;
        tmp_num = 0;
        tmp_ten = 1;
        for(int i = 0; i < num; i++){
            if(tmp_str[i] <= '9' && tmp_str[i] >= '0')
                tmp_num++;  // 数字位数
        }
        for(int i = tmp_num - 1; i >= 0; i--){
            request[cur_num] += (int)(tmp_str[i] - '0') * tmp_ten;
            tmp_ten *= 10;
        }
        cur_num++;  // 下一个数字  
    }
    fclose(fp);
}

// 数组初始化
void setarray(int arr[], int num, int val){
    for(int i = 0; i < num; i++){
        arr[i] = val;
    }
}

// 查找内存中是否存在该页面
int findexist(int memory[], int mnum, int page){
    for(int i = 0; i < mnum; i++){
        if(memory[i] == page)
            return i;
    }
    return -1;
}

// 查看内存中是否有空
int findempty(int memory[], int mnum){
    for(int i = 0; i < mnum; i++)
        if(memory[i] == -1)
            return i;
    return -1;
}

// 二维数组初始化
void initarr(int **arr, int rnum, int cnum){
    for(int r = 0; r < rnum; r++){
        for(int c = 0; c < cnum; c++){
            arr[r][c] = -1;
        }
    }
}

// 输出记录
void output_record(int **arr, int rnum, int cnum){
    printf("Memory Records:\n");
    for(int c = 0; c < cnum; c++){  // 注意循环顺序
        for(int r = 0; r < rnum; r++){
            printf("%3d", arr[r][c]);
        }
        printf("\n");
    }
}

// 先进先出页面置换算法
int fifo(int req[], int rnum, int mnum){
    int count = 0;  // 缺页次数
    int pos;

    // 分配内存空间
    int *memory = (int *)malloc(sizeof(int) * mnum);
    setarray(memory, mnum, -1);

    // 记录每个页面进入时的时间点
    int *time = (int *)malloc(sizeof(int) * mnum);
    setarray(time, mnum, M);

    // 初始化记录数组
    int **record = (int **)malloc(sizeof(int *) * rnum);
    for(int i = 0; i < rnum; i++)
        record[i] = (int *)malloc(sizeof(int) * mnum);
    initarr(record, rnum, mnum);

    for(int i = 0; i < rnum; i++){
        pos = findexist(memory, mnum, req[i]);
        if(pos != -1){  // 内存中存在页面请求
            ;
        }
        else{
            count++;
            pos = findempty(memory, mnum);  // 内存中是否有空闲
            if(pos != -1){  // 直接存入新页面，不做置换
                memory[pos] = req[i];
                time[pos] = i;  // 记录时间点
            }
            else{
                pos = 0;
                for(int j = 1; j < mnum; j++){  // 寻找时间点最小
                    if(time[j] < time[pos])
                        pos = j;
                }
                memory[pos] = req[i];  // 替换时间点最小，即最早的
                time[pos] = i;
            }
        }
        for(int tmp = 0; tmp < mnum; tmp++){
            record[i][tmp] = memory[tmp];
        }
    }
    output_record(record, rnum, mnum);  // 输出记录
    free(time);
    free(memory);
    return count;  // 返回缺页次数
}

// 最近最久未使用页面置换算法
int lru(int req[], int rnum, int mnum){
    int count = 0;
    int pos;

     // 分配内存空间
    int *memory = (int *)malloc(sizeof(int) * mnum);
    setarray(memory, mnum, -1);

    // 记录每个页面最后访问的时间
    int *time = (int *)malloc(sizeof(int) * mnum);
    setarray(time, mnum, M); 

    int **record = (int **)malloc(sizeof(int *) * rnum);
    for(int i = 0; i < rnum; i++){
        record[i] = (int *)malloc(sizeof(int) * mnum); 
    }
    initarr(record, rnum, mnum);
    
    for(int i = 0; i < rnum; i++){
        pos = findexist(memory, mnum, req[i]);
        if(pos != -1){
            time[pos] = i;  // 更新最后访问时间
        }
        else{
            count++;
            pos = findempty(memory, mnum);
            if(pos != -1){
                memory[pos] = req[i];
                time[pos] = i;
            }
            else{
                pos = 0;
                for(int j = 1; j <mnum; j++)  // 最小的即为最久的
                    if(time[j] < time[pos])
                        pos = j;
                memory[pos] = req[i];
                time[pos] = i;   
            }
        }
        for(int tmp = 0; tmp < mnum; tmp++){
            record[i][tmp] = memory[tmp];
        }
    }
    output_record(record, rnum, mnum);  // 输出记录
    free(time);
    free(memory);
    return count;
}

// 最佳页面置换算法
int opt(int req[], int rnum, int mnum){
    int count = 0;
    int pos;
    int max;

     // 分配内存空间
    int *memory = (int *)malloc(sizeof(int) * mnum);
    setarray(memory, mnum, -1);

    // 记录每个页面将来要访问的时间
    int *time = (int *)malloc(sizeof(int) * mnum);
    // setarray(time, mnum, M);

    int **record = (int **)malloc(sizeof(int *) * rnum);
    for(int i = 0; i < rnum; i++){
        record[i] = (int *)malloc(sizeof(int) * mnum); 
    }
    initarr(record, rnum, mnum);
    
    for(int i = 0; i < rnum; i++){
        pos = findexist(memory, mnum, req[i]);
        if(pos != -1){
            ;
        }
        else{
            count++;
            pos = findempty(memory, mnum);
            if(pos != -1){
                memory[pos] = req[i];
                continue;
            }
            else{  // 寻找每个页面在将来出现的最近的时间
                setarray(time, mnum, M);
                for(int j = 0; j < mnum; j++){
                    for(int k = i + 1; k < rnum; k++){
                        if(memory[j] == req[k]){
                            time[j] = k;
                            break;
                        }
                    }
                }
                pos = 0;
                for(int j = 1; j < mnum; j++){  // 寻找时间最大的
                    if(time[j] > time[pos])
                        pos = j;
                }
                memory[pos] = req[i];
            }
        }
        for(int tmp = 0; tmp < mnum; tmp++){
            record[i][tmp] = memory[tmp];
        }
    }
    output_record(record, rnum, mnum);  // 输出记录
    free(time);
    free(memory);
    return count;
}

int main(){
    char filename[100] = "page_request.txt";
    // int request[M] = {2, 3, 2, 1, 5, 2, 4, 5, 3, 2, 5, 2};
    int request[M];
    int count;  // 缺页次数
    int n = 3;  // 内存中最大页面数

    init_request(request, filename);

    printf("FIFO:\n");
    count = fifo(request, M, n);
    printf("Page fault count: %d\n", count);
    printf("Page fault rate: %.2f%%\n", (float)count/M*100.0);  // 注意 %% 转义

    printf("LRU:\n");
    count = lru(request, M, n);
    printf("Page fault count: %d\n", count);
    printf("Page fault rate: %.2f%%\n", (float)count/M*100.0);  // 注意 %% 转义

    printf("OPT:\n");
    count = opt(request, M, n);
    printf("Page fault count: %d\n", count);
    printf("Page fault rate: %.2f%%\n", (float)count/M*100.0);  // 注意 %% 转义
    return 0;
}
