#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX 1000

int current = -1;
int str_index = 0;  // 程序字符长度
char str[MAX];

char aftstr[MAX];
int aftindex = 0;

char get_char(){
    if(current < str_index - 1){
        current += 1;
        return str[current]; 
    }
    else{
        exit(0);  // 无可再处理字符，退出程序
    }
}

void go_back(){
    current--;
}

void save_char(){
    aftstr[aftindex++] = str[current];
}

int main(){
    FILE *fp = fopen("test.c", "r");
    char c, cur_str;
    while((c = getc(fp)) != EOF){
        str[str_index++] = c;
    }
    fclose(fp);
    for(int i = 0; i < str_index; i++){
        cur_str = get_char();
        
        switch (cur_str){
            case '/':
                switch (cur_str = get_char()){
                    case '/':
                        while((cur_str = get_char()) != '\n');
                        break;
                    case '*':
                        while((cur_str = get_char()))
                        break;
                    default:
                        ;
                }
                break;
            case ' ':
                save_char();
                while((cur_str = get_char()) == ' ');
                go_back();
                break;
            default:
                save_char();
        }
    }
    return 0;
}