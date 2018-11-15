// #include<string>
// #include<stdlib.h>
// #include<stdio.h>
// using namespace std;

// bool isLetter(char ch){//isLetter 标识符
// 	if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) return true;
// 	else return false;
// }

// bool isDigit(char ch){//isDigit 常数
// 	if (ch >= '0' && ch <= '9') return true;
// 	else return false;
// }

// bool isOperators(char ch){// isOperators 运算符
// 	if (ch == '+' || ch == '*' || ch == '-' || ch == '/' || ch == '=' || ch == ':' || ch == '<' || ch == '>') return true;
// 	else return false;
// }
// bool isDelimiter(char ch){// isDelimiter 界符
// 	if (ch == ',' || ch == ';' || ch == '.' || ch == '(' || ch == ')' || ch == '[' || ch == ']' || ch == '{' || ch == '}' || ch == '#') return true;
// 	else return false;
// }
// bool isBlank(char ch){
// 	if (ch == ' ' || ch == '\t') return true;
// 	else return false;
// }
// char  key[34][10] = { "main",
// "auto", "short", "int", "long", "float", "double", "char", "struct"
// , "union", "enum", "typedef", "const", "unsigned", "signed", "extern", "register"
// , "static", "volatile", "void", "if", "else", "switch", "case", "for"
// , "do", "while", "goto", "continue", "break", "default", "sizeof", "return","then"};

// void main()
// {
// back:
// 	char Sourcecode[100] = "";
// 	int number=0;
// 	int h=0;
// 	char hh[100][10]={'\0'};
// 	//声明变量
// 	printf("请输入程序段，标识符为1，常数为2，关键字为3，运算符4，界符5\n");
// 	gets(Sourcecode);
// 	//读入程序段放入firstcode

// 	for (int i = 0; i < 100; i){
// 		//char delimiter[2] = "";
// 		int j = 0;  //letter  标识符
// 		int l = 0;   //digit  常数
// 		int k = 0;   //operators  运算符

// 		int b = 0;   //类型编号
// 		char num[10] = "";
// 		char word[10] = "";
// 		if (isBlank(Sourcecode[i]) == 1){
// 			i += 1;
// 		}//去空格
// 		else{
// 			if (isLetter(Sourcecode[i]) == 1)         //是否为字母
// 			{                                             //是

// 				do
// 				{
// 					if (j < 10){
// 						word[j] = Sourcecode[i];
// 						j++;
// 					}
// 				} while (isDigit(Sourcecode[++i]) == 1|| isLetter(Sourcecode[i]) == 1);//常数||标识符

// 				//*~~~*
// 				int a = 0;
// 				while (a < 34){
// 					if (strcmp(key[a], word) == 0){
// 						b = 3;
// 						a++;
// 						break;
// 					}
// 					else {
// 						b = 1;
// 						a++;
// 						//否
// 					}
// 				}
// 				if (b == 3){
// 					printf("(3,%s)\n", word);
// 				}//~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 				if (b == 1){
// 					int q=0;
// 					for(int qq=0;strcmp(hh[qq],"")!=0;qq++){
// 						if(strcmp(word,hh[qq])==0){
// 							q=1;break;
// 						}
// 					}
// 					if(q==1){
// 						printf("(1,%d)\n",qq);
// 						q=0;
// 					}else{
// 						for(qq=0;qq<j;qq++){
// 							hh[h][qq]=word[qq];
// 						}
// 						printf("(1,%d)\n",h);
// 						h++;
// 					}
// 				/*	int qq,yy=0;
// 					if(h==0){
// 						for(qq=0;qq<j;qq++){
// 							hh[0][qq]=word[qq];
// 						}
// 						printf("(1,%d)\n",h);
// 						h++;
// 					}else{
// 						for(qq=0;qq<h;qq++){
// 							if(strcmp(hh[qq], word)==0){
// 								yy=1;
// 								break;
// 							}
// 						}
// 						if(yy==0){
// 							for(qq=0;qq<j;qq++){
// 								hh[h][qq]=word[qq];

// 							}
// 							printf("(1,%d)\n",h);
// 							h++;
// 							yy=0;
// 						}
// 						else{
// 							for(qq=0;qq<h;qq++){
// 								if(strcmp(hh[qq], word)==0)
// 									printf("(1,%d)\n",qq);
// 								yy=0;
// 							}
// 						}
// 					}*/
// 					//
// 				}
// 				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 			}
// 			else if (isDigit(Sourcecode[i]) == 1)//常数
// 			{int c = 0;
// 				while (isDigit(Sourcecode[i])){

// 					num[c] = Sourcecode[i];
// 					i += 1;
// 					c++;
// 				}
// 				printf("(2,%s)\n", num);
// 			}
// 			else if (isDelimiter(Sourcecode[i]) == 1){//界符

// 				printf("(5,%c)\n", Sourcecode[i]);
// 				i++;
// 			}
// 			else if (isOperators(Sourcecode[i]) == 1){//运算符

// 				if (Sourcecode[i] == ':' || Sourcecode[i] == '>' || Sourcecode[i] == '<'&&Sourcecode[i + 1] == '='){
// 					printf("(4,%c%c)\n", Sourcecode[i], Sourcecode[i + 1]);
// 					i += 2;
// 				}
// 				else{
// 					printf("(4,%c)\n", Sourcecode[i]);
// 					i+= 1;
// 				}
// 			}
// 			else{
// 				if(Sourcecode[i]!='\0')
// 					printf("(error,%c)\n",Sourcecode[i]);
// 				if(Sourcecode[i+1]!='\0'){

// 					i+=1;}
// 				else
// 					i=100;
// 			}
// 		}

// 	}

// 	goto back;

// }

#include<stdio.h>
#include<String.h>
int main(){
 FILE *p;
 int  falg = 0,len,i=0,j=0;//
 char str[1000],str1[1000],c;
 if((p=fopen("e:\\test.txt","rt"))==NULL){
  printf("无法打开");
  return  0;
 }
 else{
  //fgets(str,1000,p);
  while((c=getc(p))!=EOF){
   str[i++] = c;
  }
  fclose(p);
  str[i] = '\0';
  for(i=0;i<strlen(str);i++){
   if(str[i]=='/'&&str[i+1]=='/'){
    while(str[i++]!='\n'){}
   }//单行注释
   else if(str[i]=='/'&&str[i+1]=='*'){
    while(!(str[i]=='*'&&str[i+1]=='/')){i++;}
    i+=2;
   }//多行注释
   else if(str[i]==' '&&str[i+1]==' '){
    while(str[i]==' '){i++;}
    i--;
    if(str1[j-1]!=' ')
       str1[j++]=' ';
   }//多个空格，去除空格
   else if(str[i]=='\n') {
    if(str1[j-1]!=' ')
       str1[j++]=' ';
   }//换行处理，
   else if(str[i]==9){
    while(str[i]==9){
     i++;
    }
    if(str1[j-1]!=' ')
     str1[j++]=' ';
    i--;
   }//tab键处理
   else str1[j++] = str[i];//其他字符处理
  }
  str1[j] = '\0';
 // printf("%s\n",str);
 // printf("%s\n",str1);
  /*
  for(int k=0;k<strlen(str1);k++){
   if(str1[k]!=' ')
     printf("%c",str1[k]);
   else  printf("_");
  }
  */
  if((p = fopen("e:\\test1.txt","w"))==NULL){
   printf("can not find it!");
   return 0;
  }
  else{
   if(fputs(str1,p)!=0){
    printf("存储失败！");
   }
   else printf("存储成功！");
  }
  fclose(p);
 }
 return 0;
}

// ---------------------------------------------------------

// #include <stdio.h>
// #include <string.h>

// #define MAXWORD 1000
//   struct bibuffer
// {
//    char *buffer[2048];
//               //缓冲区空间
//  char *beginning, forward;
//        //前向和后向指针
//  int count;
//                      //前向指针记数
// } bbuf;

// void parse(char c)
// {
// if (c = ' ')
//  
//   {
//       memcpy(word[i], beginning, (size_t)(forward - beginning));
//       i++;
//      
//   }
//    else forward++;
// }

// int main(int argc, char *argv)
// {
//    File *fp;
//    char *word[MAXWORD];
//    int i = 0;
//    buffer = new char[2048];
//    fp = open("test.c", "r");
//    read(fp, buffer, 1023);
//    buffer[1023] = '#';
//    read(fp, buffer + 1024, 1023);
//    buffer[2047] = '#';
//    bbuf->buffer = buffer;
//    bbuf->beginning = bbuf->forward = bbuf->buffer;
//    bbuf->count = 0;
//    while (1)
//  
//   {
//       forward = forward + 1;
//       if (count == 1023)
//   
//     {
//          read(fp, buffer + 1024, 1023);
//          forward++;
//           //这个函数的具体代码就要和具体的词法分析规则而定,这里假设只识别空格分割的单词
//    parse(*forward);
//         
//     }
//       else if (count >= 2048)
//   
//     {
//          read(fp, buffer, 1023);
//          forward = bbuf->buffer;
//                               //这个函数的具体代码就要和具体的词法分析规则而定,这里假设只识别空格分割的单词
//    parse(*forward);
//         
//     }
//       else if (count != 1023 && count < 2048 && (*forward) = '#')
//   
//     {
//          break;
//                  //词法分析结束
//   
//     }
//      
//   }
//    
// }