/* 对源程序文件进行预处理
1. 合并空白符
    把原始程序中相邻的空格、制表符、回车等空白符合并成一个空格，便于后续处理
2. 消除注释
    消除原始程序中的注释内容
*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX 10000  // 输入文件字符最大长度

int current = -1;  // 当前读到的字符的index
int str_index = 0;  // 输入程序字符长度
char str[MAX];  // 存储源文件字符

FILE *fp;  // 源程序文件
FILE *nfp;  // 预处理之后的文件

char aftstr[MAX];  // 存储预处理之后的字符
int aftindex = 0;  // 预处理之后的字符长度

char get_char();  // 获取下一个字符
void go_back();  // 回退一个字符
void save_char();  // 将当前字符保存至预处理之后的字符数组中
void save_spec_char(char);  // 保存一个特定的字符
int previous_is_space();  // 查看已处理字符的最后一个是否是空格

int main(int argc, char *argv[]){
    fp = fopen(argv[1], "r");
    nfp = fopen(argv[2], "w");
    char c, cur_str;
    while((c = getc(fp)) != EOF){  // 从源文件中读入字符
        str[str_index++] = c;
    }
    fclose(fp);
    // for(int i = 0; i < str_index; i++)  // debug
    //     printf("%c", str[i]);
    while(1){
        switch (get_char()){
            case '/':  // 消除注释
                switch (get_char()){
                    case '/':
                        while(get_char() != '\n');  // 识别："//" 将一整行去掉，包括最后的\n
                        break;  // 注释结束
                    case '*':
                        while(1){
                            if(get_char() == '*'){
                                if(get_char() == '/')  // 采用 "/ * while * /" 结构识别: /* */ 
                                    break;  // 注释结束
                            }
                        }
                        break;
                    default:  // 默认放弃所有注释内容
                        ;
                }
                break;
            case '\t':  // 制表符  合并空白行
            case '\n':  // 换行符
            case '\r':  // 回车符
            case ' ':
                if(!previous_is_space())  // 防止因为中间注释等，出现多个space
                    save_spec_char(' ');  // 均替换为单个的空格字符
                do{
                    cur_str = get_char();
                }while(cur_str == ' ' || cur_str == '\t' || cur_str == '\r' || cur_str == '\n');
                go_back();
                break;
            default:
                save_char();
        }
    }
    return 0;
}

char get_char(){
    if(current < str_index - 1){
        current += 1;
        return str[current]; 
    }
    else{  // 如果访问超过了界限，已无需处理的字符，则将预处理结果写入文件，结束程序
        for(int i = 0; i < aftindex; i++){
            putc(aftstr[i], nfp);
        }
        fclose(nfp);
        exit(0);  // 无可再处理字符，退出程序
    }
}

void go_back(){
    current--;
}

void save_char(){
    aftstr[aftindex++] = str[current];
}

void save_spec_char(char tmp){
    aftstr[aftindex++] = tmp;
}

int previous_is_space(){
    if(aftindex > 0){
        if(aftstr[aftindex - 1] == ' '){
            return 1;
        }
        else{
            return 0;
        }
    }
    else{
        return 0;
    }
}