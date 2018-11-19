/* 对预处理过后的.c文件进行词法分析 */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

#define MAXSIZE 10000  // 输入字符的最大长度
#define WORDSIZE 256  // 关键字或单个变量最大长度
#define KEYWORDNUM 34  // 关键词个数
#define KEYWORDMAXSIZE 10  // 关键词最大长度

/* 分类 */
#define $keyword    1  // 关键字
#define $ID         2  // 标识符
#define $INT        3  // 正整数
#define $operator   4  // 操作符 + - * / = < > += -= *= /= == <= >= != !
#define $delimiter  5  // 分隔符 , . ; ( ) [ ] { } :

char CHAR;  // 新读入的源程序字符
int current = -1;  // 当前读到的字符的index
int str_index = 0;  // 输入成旭的字符长度
char str[MAXSIZE];  // 存储源文件的字符

char token[WORDSIZE] = {'\0'};  // 构成单词符号的字符串
int token_index = 0;

FILE *fp;  // 程序源文件

char keyword[KEYWORDNUM][KEYWORDMAXSIZE] = { "main",
    "auto", "short", "int", "long", "float", "double", "char", "struct",
    "union", "enum", "typedef", "const", "unsigned", "signed", "extern", "register",
    "static", "volatile", "void", "if", "else", "switch", "case", "for",
    "do", "while", "goto", "continue", "break", "default", "sizeof", "return","then"
};

void get_char();  // 读入下一个源程序字符至CHAR中，并将字符指针后移一个位置
void getnbc();  // 若CHAR为空白字符，反复调用get_char，直至CHAR读入的是一个非空白字符，否则不操作
void concat();  // token += CHAR
void go_back();  // 回退一个字符
bool is_letter();  // 是否为字母
bool is_digit();  // 是否为数字
int reserve();  // 由token查找保留字表，返回保留编码，若无，返回0
void set_blank();  // 将token置为全\0

int main(int argc, char *argv[]){
    fp = fopen(argv[1], "r");
    char c;
    while((c = getc(fp)) != EOF){  // 从源文件中读入字符
        str[str_index++] = c;
    }
    fclose(fp);
    int type_code;
    while(1){
        set_blank();
        get_char();
        getnbc();
        if(is_letter() || CHAR == '_'){
            do{
                concat();
                get_char();
            }while(is_letter() | is_digit());
            go_back();
            type_code = reserve();
            if(type_code == 0){
                printf("<%d, %s>\n", $ID, token);
            }
            else{
                printf("<%d, %s>\n", type_code, token);
            }
        }
        else if(is_digit()){
            do{
                concat();
                get_char();
            }while(is_digit());
            go_back();
            printf("<%d, %s>\n", $INT, token);
        }
        else{
            char before = CHAR;
            switch(CHAR){
                case '+':
                case '-':
                case '*':
                case '/':
                case '=':
                case '<':
                case '>':
                case '!':
                    get_char();
                    switch(CHAR){
                        case '=':
                            printf("<%d, %c%c>\n", $operator, before, CHAR);
                            break;
                        default:
                            go_back();
                            printf("<%d, %c>\n", $operator, before);
                            break;
                    }
                    break;
                case ',':
                case '.':
                case ';':
                case '(':
                case ')':
                case '[':
                case ']':
                case '{':
                case '}':
                case ':':
                    printf("<%d, %c>\n", $delimiter, CHAR);
                    break;
                default:
                    break;
            }
        }
    }
    return 0;
}

void set_blank(){
    for(int i = 0; i < token_index; i++){
        token[i] = '\0';
    }
    token_index = 0;
}

void get_char(){
    if(current < str_index - 1){
        current += 1;
        CHAR = str[current];
    }
    else{
        exit(0);
    }
}

void getnbc(){
    while(CHAR == ' '){
        get_char();
    }
}

void concat(){
    token[token_index++] = CHAR;
}

void go_back(){
    current--;
    CHAR = ' ';
}

bool is_letter(){
    if((CHAR >= 'A' && CHAR <= 'Z') || (CHAR >= 'a' && CHAR <= 'z')){
        return true;
    }
    else{
        return false;
    }
}

bool is_digit(){
    if(CHAR >= '0' && CHAR <= '9'){
        return true;
    }
    else{
        return false;
    }
}

/* 本身应该实现为查保留字表，但现在只查询关键字表，是则返回1，不是则0 */
int reserve(){
    for(int i = 0; i < KEYWORDNUM; i++){
        if(strcmp(token, keyword[i]) == 0){
            return $keyword;
        }
    }
    return 0;
}