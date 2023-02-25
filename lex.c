//Colton Curtis HW2 SCanner

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "compiler.h"

int is_symbol(char in);

lexeme *lex_analyze(int list_flag, char *input) {  
  if(input == NULL){
    return NULL;
  }
  
  int lindex = 0;

  if(list_flag == 1)
    printf("Lexeme List:\n\nlexeme\t\ttoken type\n-----------------------\n");//Initial print

  lexeme *l = malloc(sizeof(lexeme)*500);
  char tokens[33][11] =  {"w", "n", "const", "var", "procedure", "call", "begin", "end", "if", "then", "else", "while", "do", "read", "write", "def", ".", ":=", "-", ";", "{", "}", "==", "<>", "<", "<=", ">", ">=", "+", "*", "/", "(", ")"};  
  // Represents the different tokens that signify instructions
  // 2D character array


  char array[500];
  strcpy(array,input);//Array to store instructions
  int index = 0;
  int action = 0;

  // SETUP ------------------------------------------------------------


  //Start to cycle through the array
  while(array[index] != '\0'){
    int found = 0;
    char scan = array[index];


    

    //ALPHA--------------------------------------------
    if(isalpha(scan)){//character is in the alphabet
      //map out every possibility by characters
      char word[12];
      strcpy(word, "");
      int s = 0;
      
      while(isalpha(array[index]) || isdigit(array[index])){
        word[s] = array[index];//copies the word over to a char array
        index++;
        s++;
        if(s == 11){
          break;
        }
      }
      
      //Check for too much information
      if(isalpha(array[index]) || isdigit(array[index])){
        printf("Lexical Analyzer Error: maximum identifier length is 11\n");
        while(isalpha(array[index]) || isdigit(array[index])){
          index++;
          }
        index++;
        strcpy(word,"");
        action = 0;
      }
      else{//end the word that is copied over
        word[s] = '\0';
      }
      int gang = 0;
      if(action == 0)
        gang =1;

      if(action == 1 && strcmp(word, "") != 0){//Identifier
        found = 1;
        if(isdigit(word[0])){//error
          printf("Lexical Analyzer Error: identifiers cannot begin with digits");
        }
        else{
          if(list_flag == 1)
            printf("%s\t\t\t%d\n", word, 1);
          strcpy(l[lindex].identifier_name, word);
          l[lindex].type = 1;
          lindex++;
        }
        action = 0;
      }

      for(int i=0;i<33;i++){
        if(strcmp(word, tokens[i]) == 0){//compare the word to our list
          found = 1;
          if(i+1 == 4 || i+1 == 9 || i+1 == 3 || i+1 == 5 || i+1 == 14 || i+1 == 6|| i+1 == 16){
            action = 1;
          }
          if(list_flag == 1)
            printf("%s\t\t\t%d\n", word, i+1);//prints the word and the token
          strcpy(l[lindex].identifier_name, word);
          l[lindex].type = 1+i;
          lindex++;
        }
      }

      if(gang == 1 && found == 0){//starting out as a name in the file
        if(strcmp("main",word)==0 || strcmp("null",word)==0){
          printf("Lexical Analyzer Error: identifiers cannot be named 'null' or 'main'\n");
        }
        else if(strcmp(word, "") != 0 ){//word scanned in
          if(list_flag == 1)
            printf("%s\t\t\t%d\n", word, 1);
          strcpy(l[lindex].identifier_name, word);
          l[lindex].type = 1;
          lindex++;
        }
      }

    }
    //---------------------------------------------
      


      
    //DIGIT-----------------------------------------
    else if(isdigit(scan)){
      char *num = malloc(sizeof(char)*7);
      int d = 0;
      while(isdigit(array[index])){//scans the digit into 'num'
        num[d] = array[index];
        d++;
        index++;
      }
      if(isalpha(array[index])){//reached the max length
        while(isalpha(array[index]) || isdigit(array[index])){
          d+=5;
          index++;
        }
      }
      if(d>5){//prints an error
        printf("Lexical Analyzer Error: maximum number length is 5\n");
        d = 0;
      }
      else{//regular print number
        if(list_flag == 1)
          printf("%s\t\t\t%d\n", num, 2);
        l[lindex].number_value = atoi(num);
        l[lindex].type = 2;
        lindex++;
        found = 1;
        }
      
      action = 0;
    }
    //-----------------------------------------------


      

    //SYMBOLS----------------------------------------
    else{
      switch(scan){//each one of these is a case that recognizes a symbol and prints it
        char temp[4];
        case ';':{
          if(list_flag == 1)
            printf("%c\t\t\t%d\n", array[index], 20);
          l[lindex].identifier_name[0] = ';';
          l[lindex].type = 20;
          lindex++;
          index+=2;
          break;
        }

        case ':':{
          temp[0] = array[index];
          if(array[index+1] == '='){
            temp[1] = array[index+1];
            temp[2] = '\0';
            if(list_flag == 1)
              printf("%s\t\t\t%d\n", temp, 18);
            strcpy(l[lindex].identifier_name,temp);
            l[lindex].type = 18;
            lindex++;
            index+=2;
          }
          
        }

        case '=':{
          temp[0] = array[index];
          if(array[index+1] == '='){
            temp[1] = array[index+1];
            temp[2] = '\0';
            if(list_flag == 1)
              printf("%s\t\t\t%d\n", temp, 23);
            strcpy(l[lindex].identifier_name,temp);
            l[lindex].type = 23;
            lindex++;
            index+=2;
          }
          
        }

        case ' ':{
          index++;
          break;
        }
        
        case '\n':{
          index++;
          break;
        }

        case '{':{
          if(list_flag == 1)
            printf("%c\t\t\t%d\n", scan, 21);
          l[lindex].identifier_name[0] = '{';
          l[lindex].type = 21;
          lindex++;
          index++;
          break;
        }

        case '<':{//checks the different variations of the symbol
          temp[0] = array[index];
          temp[1] = '\0';
          if(array[index+1] == '>'){
            temp[1] = array[index+1];
            temp[2] = '\0';
            if(list_flag == 1)
              printf("%s\t\t\t%d\n", temp, 24);
            strcpy(l[lindex].identifier_name,temp);
            l[lindex].type = 24;
            lindex++;
            index+=2;
            action = 1;
            break;
          }
          if(array[index+1] == '='){
            temp[1] = array[index+1];
            temp[2] = '\0';
            if(list_flag == 1)
              printf("%s\t\t\t%d\n", temp, 26);
            strcpy(l[lindex].identifier_name,temp);
            l[lindex].type = 26;
            lindex++;
            index+=2;
            action = 1;
            break;
          }
          else{
            if(list_flag == 1)
              printf("%s\t\t\t%d\n", temp, 25);
            strcpy(l[lindex].identifier_name,temp);
            l[lindex].type = 25;
            lindex++;
            index++;
            break;
          }
          break;
        }

        case '>':{//checks the different variations of the symbol
          temp[0] = array[index];
          temp[1] = '\0';
          if(array[index+1] == '='){
            temp[1] = array[index+1];
            temp[2] = '\0';
            if(list_flag == 1)
              printf("%s\t\t\t%d\n", temp, 28);
            strcpy(l[lindex].identifier_name,temp);
            l[lindex].type = 28;
            lindex++;
            index+=2;
            action = 1;
            break;
          }
          else{
            if(list_flag == 1)
              printf("%s\t\t\t%d\n", temp, 27);
            strcpy(l[lindex].identifier_name,temp);
            l[lindex].type = 27;
            lindex++;
            index++;
            break;
          }
          break;
        }

        case '+':{
          if(list_flag == 1)
            printf("%c\t\t\t%d\n", scan, 29);
          l[lindex].identifier_name[0] = '+';
          l[lindex].type = 29;
          lindex++;
          index++;
          break;
        }

        case '-':{
          if(list_flag == 1)
            printf("%c\t\t\t%d\n", scan, 19);
          l[lindex].identifier_name[0] = '-';
          l[lindex].type = 19;
          lindex++;
          index++;
          break;
        }

        case '}':{
          if(list_flag == 1)
            printf("%c\t\t\t%d\n", scan, 22);
          l[lindex].identifier_name[0] = '}';
          l[lindex].type = 22;
          lindex++;
          index++;
          break;
        }

        case '.':{
          if(list_flag == 1)
            printf("%c\t\t\t%d\n", scan, 17);
          l[lindex].identifier_name[0] = '.';
          l[lindex].type = 17;
          lindex++;
          index++;
          break;
        }

        case '#':{
          while(array[index] != '\n'){
            index++;
          }
          index++;
          break;
        }

        case '*':{
          if(list_flag == 1)
            printf("%c\t\t\t%d\n", scan, 30);
          l[lindex].identifier_name[0] = '*';
          l[lindex].type = 30;
          lindex++;
          index++;
          break;
        }

        case '/':{
          if(list_flag == 1)
            printf("%c\t\t\t%d\n", scan, 31);
          l[lindex].identifier_name[0] = '/';
          l[lindex].type = 31;
          lindex++;
          index++;
          break;
        }

        case '(':{
          if(list_flag == 1)
            printf("%c\t\t\t%d\n", scan, 32);
          l[lindex].identifier_name[0] = '(';
          l[lindex].type = 32;
          lindex++;
          index++;
          break;
        }

        case ')':{
          if(list_flag == 1)
            printf("%c\t\t\t%d\n", scan, 33);
          l[lindex].identifier_name[0] = ')';
          l[lindex].type = 33;
          lindex++;
          index++;
          break;
        }

        default:{//The symbol was not found
          if(found == 0){
            printf("Lexical Analyzer Error: invalid symbol\n");
            index++;
            break;
          }
        }
        
      }
    }
    //-----------------------------------------------

  }
  return l;
}




