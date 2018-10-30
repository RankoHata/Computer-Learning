#include<stdio.h>
#include<stdbool.h>

#define NUM 15  // number(identifier)
#define SIZE 256  // len(token) + 1 # '\0'

#define $BEGIN  1  // BEGIN
#define $END    2  // END
#define $WHILE  3  // WHILE
#define $SEMI   4  // DO
#define $ID     5  // IDENTIFIER
#define $INT    6  // INTEGER
#define $SEMT   7  // ;
#define $PLUS   8  // +
#define $STAR   9  // *
#define $ASSIGN 10  // :=
#define $LE     11  // <=
#define $LT     12  // <
#define $COLON  13  // :

char CHAR = '\0';
char TOKEN[SIZE] = {'\0'};

char TABLE[NUM][SIZE] = {"BEGIN", "END", "WHILE", "DO", ";", "+", "*", ":=", "<=", "<", ":"};
int VALUE[NUM] = {$BEGIN, $END, $WHILE, $SEMI, $SEMT, $PLUS, $STAR, $ASSIGN, $LE, $LT, $COLON};

void GETCHAR(char* CHAR);  // 读入字符
void GETNBC(char* CHAR);  // loop读入，直到不为空
void CONCAT(char* TOKEN, char* CHAR);  // TOKEN += CHAR
bool LETTER(char CHAR);  // is a letter ?
bool DIGIT(char CHAR);  // is a digital ?
int RESERVE(char* TOKEN);  // 返回TOKEN的保留字编码
void RETRACT();  // 回退一个字符

void GETCHAR(char* CHAR){
    *CHAR = getchar();
}

void GETNBC(char* CHAR){
    while(*CHAR == ' '){
        GETCHAR(CHAR);
    }
}

void CONCAT(char* TOKEN, char* CHAR){
    for(int i = 0; i < SIZE; i++){
        if(TOKEN[i] == '\0'){
            TOKEN[i] = *CHAR;
            break;
        }
    }
}

bool LETTER(char CHAR){
    if(CHAR >= 'A' && CHAR <= 'Z'){
        return true;
    }
    else{
        return false;
    }
}

bool DIGIT(char CHAR){
    if(CHAR >= 48 && CHAR <= 57){
        return true;
    }
    else{
        return false;
    }
}

int RESERVE(char* TOKEN){
    int match;
    for(int i = 0; i < NUM; i++){
        match = VALUE[i];
        for(int j = 0; j < SIZE; j++){
            if((TABLE[i][j] == '\0') && (TOKEN[j] == '\0')){
                break;
            }
            else if(TABLE[i][j] != TOKEN[j]){
                match = 0;
                break;
            }
        }
        if(match == VALUE[i]){
            break;
        }
    }
    return match;
}


int main(){
    return 1;
}