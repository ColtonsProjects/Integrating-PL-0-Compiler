//Colton Curtis

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compiler.h"

#define ARRAY_SIZE 500



int numTokens = 0;
lexeme *tokens;
int token_index = 0;
symbol *table;
int table_index = 0;
instruction *code;
int code_index = 0;

int error = 0;
int error2 = 0;
int level;
int inx = 0;

void emit(int op, int l, int m);
void add_symbol(int kind, char name[], int value, int level, int address);
void mark();
int multiple_declaration_check(char name[]);
int find_symbol(char name[], int kind);

void block();
int declarations();
void constants();
void variables(int numVars);
void procedure();
void statement();
void condition();
void factor();
void term();
void expression();

void print_parser_error(int error_code, int case_code);
void print_assembly_code();
void print_symbol_table();
void block();







instruction *parse(int code_flag, int table_flag, lexeme *list)
{

  if(list == NULL){
    return NULL;
  }
  
	// variable setup
	int i;
	tokens = list;
	table = calloc(ARRAY_SIZE, sizeof(symbol));
	code = calloc(ARRAY_SIZE, sizeof(instruction));


  //starts the token index
	token_index = 0;

  
  //  --------------------------------------------------------

  //Add the symbol main for our function 
  add_symbol(3, "main", 0, 0, 0);
  
  level = -1;

  //JMP to the location to start main
  emit(7, 0, 0);

  //creates block
	block();


  table[0].address = inx;

  for(int j = 0; j<code_index; j++){
    if(code[j].op == 5){
      code[j].m = table[code[j].m].address;
    }
  }

  code[0].m = inx;
  
  
  if(tokens[token_index].type != 17){
    error = 1;
    error2 = 0;
    print_parser_error(error , error2);
    return 0;
  }


  emit( 9, 0, 3);//HLT

  if(error == 0){
    if(code_flag == 1){
      print_assembly_code();
    }
    if(table_flag == 1){
      print_symbol_table();
    }
  }

  code[code_index].m = -1;
	return code;
}


void block(){
  level++;
  int m = declarations();
  inx = code_index*3;
  if(error != 0)
    return;

  if(error != 0)
    return;

  procedure();
  if(error != 0)
    return;


  emit(6, 0, m);

  while(tokens[token_index].type == 6 || tokens[token_index].type == 7 || tokens[token_index].type == 9 || tokens[token_index].type == 12 || tokens[token_index].type == 14 || tokens[token_index].type == 15){
    statement();
      if(error != 0)
        return;
  }
    
  if(error != 0)
    return;

  mark();
  level--;
}


int declarations(){
  int varscalled = 0;

  //check to see the type
  while(tokens[token_index].type == 3 || tokens[token_index].type == 4){
    if(tokens[token_index].type == 3){//constant
      constants();
      if(error != 0){//check for errors
        return 0;
      }
    }
    else{
      variables(varscalled);//variable
      if(error != 0){
        return 0;
      }
      varscalled++;//increment
    }
  }

  return varscalled+3;
}




void constants(){
  int minus_flag = 0;//check for negative
  token_index++;
  if(tokens[token_index].type != 1 ){
    error = 2;
    error2 = 1;
    print_parser_error(error , error2);
    return;
  }

  if(multiple_declaration_check(tokens[token_index].identifier_name) != -1){//check to see if alr declared
    error = 3;
    error2 = 0;
    print_parser_error(error , error2);
    return;
  }

  //now we know there are no errors so save the symbol 
  strcpy(table[table_index].name, tokens[token_index].identifier_name);

  
  token_index++;
  if(tokens[token_index].type != 18){
    error = 4;
    error2 = 1;
    print_parser_error(error , error2);
    return;
  }

  token_index++;//check for a negative number
  if(strcmp(tokens[token_index].identifier_name ,"-") == 0){
    minus_flag = 1;
    token_index++;
  }

  if(tokens[token_index].type != 2){
    error = 5;
    error2 = 0;
    print_parser_error(error , error2);
    return;
  }

  //now we know there are no errors so save the number
  table[table_index].value = tokens[token_index].number_value;
  table[table_index].kind = 2;
  table[table_index].level = level;
  // table[token_index].address = 

  token_index++;
  if(minus_flag == 1){
    table[token_index].value = (table[token_index].value*(-1));//make negative
  }

  
  add_symbol(1, table[table_index].name, table[table_index].value, level, 0);
  if(tokens[token_index].type != 20){
    error = 6;
    error2 = 1;
    print_parser_error(error , error2);
    return;
  }
  token_index++;
}





void variables(int numVars){
  token_index++;
  if(tokens[token_index].type != 1){
    error = 2;
    error2 = 2;
    print_parser_error(error , error2);
    return;
  }

  if(multiple_declaration_check(tokens[token_index].identifier_name) != -1){
    error = 3;
    error2 = 0;
    print_parser_error(error , error2);
    return;
  }

  strcpy(table[table_index].name , tokens[token_index].identifier_name); 

  token_index++;
  add_symbol(2, table[table_index].name, 0, level, numVars+3);
  //printf("Added by vars: --->  %4d | %11s | %5d | %5d | %5d | %5d\n", table[table_index-1].kind, table[table_index-1].name, table[table_index-1].value, table[table_index-1].level, table[table_index-1].address, table[table_index-1].mark);  
  
  if(tokens[token_index].type != 20){
    error = 6;
    error2 = 2;
    print_parser_error(error , error2);
    return;
  }

  token_index++;
}


void statement(){
  if(tokens[token_index].type == 16){
    token_index++;
    if(tokens[token_index].type != 1){
      error = 2;
      error2 = 6;
      print_parser_error(error , error2);
      return;
    }

    //double check the solution
    int ind = find_symbol(tokens[token_index].identifier_name, 2);
    
    if(ind == -1){
    
      if(find_symbol(tokens[token_index].identifier_name, 1) == find_symbol(tokens[token_index].identifier_name, 3)){
        error = 8;
        error2 = 1;
        print_parser_error(error , error2);
        return;
      }
      else{
        error = 9;
        error2 = 0;
        print_parser_error(error , error2);
        return;
      }
    }

    token_index++;
    if(tokens[token_index].type != 18){
      error = 4;
      error2 = 2;
      print_parser_error(error , error2);
      return;
    }

    token_index++;
    expression();
    if(error != 0){
      return;
    }

    emit(4,table[ind].level, table[ind].address); 
  }

  else if(tokens[token_index].type == 6){
    token_index++;
    if(tokens[token_index].type != 1){
      error = 2;
      error2 = 4;
      print_parser_error(error , error2);
      return;
    }

    int indx = find_symbol(tokens[token_index].identifier_name, 3);//get the index for the emit
    if(indx == -1){//check which error
      if(find_symbol(tokens[token_index].identifier_name, 1) == find_symbol(tokens[token_index].identifier_name, 2)){
        error = 8;
        error2 = 2;
        print_parser_error(error , error2);
        return;
      }
      else{
        error = 9;
        error2 = 0;
        print_parser_error(error , error2);
        return;
      }
    }
    token_index++;//increment
    emit(5, table[indx].level, indx);
  }

    
  else if(tokens[token_index].type == 7){//begin
    do{
      token_index++;
      statement();
      if(error != 0){
        return;
      }
    } while(tokens[token_index].type == 20);//semicolon

    //printf("%d %d\n", token_index, tokens[token_index-1].type);
    
    if(tokens[token_index].type != 8){//Until the end
      if(tokens[token_index].type == 1 || tokens[token_index].type == 6 || tokens[token_index].type == 7 || tokens[token_index].type == 14 || tokens[token_index].type == 16){
        //printf("___ %d ___",token_index);
        error = 6;
        error2 = 3;
        print_parser_error(error , error2);
        return;
      }
      else{
        error = 10;
        error2 = 0;
        print_parser_error(error , error2);
        return;
      }
    }
    token_index++;
  }
      

  else if(tokens[token_index].type == 14){//read
    token_index++;
    if(tokens[token_index].type != 1){
      error = 2;
      error2 = 5;
      print_parser_error(error , error2);
      return;
    }
    int in = find_symbol(tokens[token_index].identifier_name, 2);

    if(in == -1){
      if(find_symbol(tokens[token_index].identifier_name, 1) == find_symbol(tokens[token_index].identifier_name, 2)){
        error = 8;
        error2 = 3;
        print_parser_error(error , error2);
        return;
      }
      else{
        error =13;
        error2 = 0;
        print_parser_error(error , error2);
      }
    }

    token_index++;
    emit(9, 0, 2);//RED
    emit(4, table[in].level, table[in].address);//STO
  }

  else if(tokens[token_index].type == 9){//if statement
    token_index++;
    condition();
    if(error != 0){
      return;
    }

    int codei = code_index;
    emit(8, 0, 0);

    if(tokens[token_index].type != 10){//then
      error = 11;
      error2 = 0;
      return;
    }
    token_index++;
    statement();
    if(error != 0)
      return;


    int indxx = code_index;
    if(tokens[token_index].type == 11){//else
      token_index++;
      emit(7, 0 , 0);//jump
      code[codei].m = code_index*3;//store the code index
      statement();

      if(error != 0)
        return;

      code[indxx].m = code_index*3;//set m to the correct value
    }

    else{
      code[codei].m = code_index*3;//set mto the correct value
    }
  }

    
  else if(tokens[token_index].type == 12){//while loop
    token_index++;
    int ci = code_index;
    
    condition();
    if(error != 0)
      return;

    if(tokens[token_index].type != 13){//is not "do" function
      error = 12;
      error2 = 0;
      return;
    }
    token_index++;

    int cii = code_index;//store the code index
    emit(8, 0, 0);
    
    statement();
    if(error != 0)
      return;

    emit(7, 0, ci*3);//jump to check the condition again
    code[cii].m = code_index*3;
  }

  else if(tokens[token_index].type == 15){//write
    token_index++;
    expression();
    if(error != 0)
      return;

    emit(9, 0, 1);
  }

  
}

void condition(){
  expression();
  if(error != 0){
    return;
  }
  if(tokens[token_index].type < 23 || tokens[token_index].type > 28){
    error = 16;
    error2 = 0;
    return;
  }
  int hold = tokens[token_index].type;//store the typr
  token_index++;
  
  expression();
  
  if(error != 0)
    return;
  
  emit(2, 0, hold-18); //Lines up with the other struct
  
}



void expression(){
  term();
  if(error != 0){
    return;
  }

  int tindex = 0;
  while(tokens[token_index].type == 19 || tokens[token_index].type == 29){
    if(tokens[token_index].type == 19)
      tindex = 1;
    token_index++;
    term();
    if(error != 0)
      return;

    if(tindex != 1){
      emit(2, 0, 1);//minus
    }
    else{
      emit(2, 0, 2);//plus
    }
    tindex = 0;//reset
  }
}



void term(){
  factor();
  if(error != 0)
    return;

  int type = 0;
  while(tokens[token_index].type == 30 || tokens[token_index].type == 31 ){
    if(tokens[token_index].type == 30){//times
      type = 1;
    }
    token_index++;
    factor();
    if(error != 0)
      return;
    if(type == 1){//times
      emit(2, 0, 3);
    }
    else{
      emit(2, 0, 4);//division
    }
    type = 0;
  }
  
}


void procedure(){
  while(tokens[token_index].type == 5){//procedure
    int ti = token_index+1;
    int ci = code_index;
    token_index++;
    
    if(tokens[token_index].type != 1){
      //not an identifier
      error = 2;
      error2 = 3;
      print_parser_error(error , error2);
      return;
    }
    
    if(multiple_declaration_check(tokens[token_index].identifier_name) != -1){
      //identifier was already used
      error = 3;
      error2 = 0;
      print_parser_error(error , error2);
      return;
    }

    strcpy(table[table_index].name, tokens[token_index].identifier_name);
    token_index++;

    add_symbol(3, table[table_index].name, 0, level, 0);//add to the table

    if(tokens[token_index].type != 21){//Left curley brace
      error = 14;
      error2 = 0;
      print_parser_error(error , error2);
      return;
    }

    token_index++;

    block();

    if(error != 0)
      return;

    emit(2, 0, 0);

    if(tokens[token_index].type != 22){//right curley brace
      error = 15;
      error2 = 0;
      print_parser_error(error , error2);
      return;
    }

    token_index++;
    inx = code_index*3;
    table[find_symbol(tokens[ti].identifier_name, 3)].address = ci*3;//set address
  }
}




void factor(){
  if(tokens[token_index].type == 1){
    int c = find_symbol(tokens[token_index].identifier_name, 1);
    int v = find_symbol(tokens[token_index].identifier_name, 2);

    if(c == v){
      if(find_symbol(tokens[token_index].identifier_name, 3) != -1){
        error = 17;
        error2 = 0;
        print_parser_error(error , error2);
        return;
      }
      else{
        error = 8;
        error2 = 4;
        print_parser_error(error , error2);
        return;
      }
    }

    if(c == -1){
      emit(3, table[v].level, table[v].address);//LOD
    }

    else if(v == -1){
      emit(1, 0, table[c].value);//LIT
    }

    else if(table[c].level > table[v].level){
      emit(1, 0, table[c].value);//LIT
    }

    else{
      emit(3, table[v].level, table[v].address);//LOD
    }

    token_index++;
  }

  else if(tokens[token_index].type == 2){//number 
    emit(1, 0, tokens[token_index].number_value);
    token_index++;
  }

  else if(tokens[token_index].type == 32){//left parenth
    token_index++;
    expression();
    if(error != 0)
      return;

    if(tokens[token_index].type != 33){//right parenth
      error = 18;
      error2 = 0;
      return;
    }
    token_index++;
  }

  else{
    error = 19;
    error2 = 0;
    print_parser_error(error , error2);
    return;
  }


}





// adds a new instruction to the end of the code
void emit(int op, int l, int m)
{
	code[code_index].op = op;
	code[code_index].l = l;
	code[code_index].m = m;
	code_index++;
}





// adds a new symbol to the end of the table
void add_symbol(int kind, char name[], int value, int level, int address)
{
	table[table_index].kind = kind;
	strcpy(table[table_index].name, name);
	table[table_index].value = value;
	table[table_index].level = level;
	table[table_index].address = address;
	table[table_index].mark = 0;
	table_index++;
}




// marks all of the current procedure's symbols
void mark()
{
	int i;
	for (i = table_index - 1; i >= 0; i--)
	{
		if (table[i].mark == 1)
			continue;
		if (table[i].level < level)
			return;
		table[i].mark = 1;
	}
}




// returns -1 if there are no other symbols with the same name within this procedure
int multiple_declaration_check(char name[])
{
	int i;
	for (i = 0; i < table_index; i++)
		if (table[i].mark == 0 && table[i].level == level && strcmp(name, table[i].name) == 0)
			return i;
	return -1;
}





// returns the index of the symbol with the desired name and kind, prioritizing 
// 		symbols with level closer to the current level
int find_symbol(char name[], int kind)
{
	int i;
	int max_idx = -1;
	int max_lvl = -1;
	for (i = 0; i < table_index; i++)
	{
		if (table[i].mark == 0 && table[i].kind == kind && strcmp(name, table[i].name) == 0)
		{
			if (max_idx == -1 || table[i].level > max_lvl)
			{
				max_idx = i;
				max_lvl = table[i].level;
			}
		}
	}
	return max_idx;
}




void print_parser_error(int error_code, int case_code)
{
	switch (error_code)
	{
		case 1 :
			printf("Parser Error 1: missing . \n");
			break;
		case 2 :
			switch (case_code)
			{
				case 1 :
					printf("Parser Error 2: missing identifier after keyword const\n");
					break;
				case 2 :
					printf("Parser Error 2: missing identifier after keyword var\n");
					break;
				case 3 :
					printf("Parser Error 2: missing identifier after keyword procedure\n");
					break;
				case 4 :
					printf("Parser Error 2: missing identifier after keyword call\n");
					break;
				case 5 :
					printf("Parser Error 2: missing identifier after keyword read\n");
					break;
				case 6 :
					printf("Parser Error 2: missing identifier after keyword def\n");
					break;
				default :
					printf("Implementation Error: unrecognized error code\n");
			}
			break;
		case 3 :
			printf("Parser Error 3: identifier is declared multiple times by a procedure\n");
			break;
		case 4 :
			switch (case_code)
			{
				case 1 :
					printf("Parser Error 4: missing := in constant declaration\n");
					break;
				case 2 :
					printf("Parser Error 4: missing := in assignment statement\n");
					break;
				default :				
					printf("Implementation Error: unrecognized error code\n");
			}
			break;
		case 5 :
			printf("Parser Error 5: missing number in constant declaration\n");
			break;
		case 6 :
			switch (case_code)
			{
				case 1 :
					printf("Parser Error 6: missing ; after constant declaration\n");
					break;
				case 2 :
					printf("Parser Error 6: missing ; after variable declaration\n");
					break;
				case 3 :
					printf("Parser Error 6: missing ; after statement in begin-end\n");
					break;
				default :				
					printf("Implementation Error: unrecognized error code\n");
			}
			break;
		case 7 :
			printf("Parser Error 7: procedures and constants cannot be assigned to\n");
			break;
		case 8 :
			switch (case_code)
			{
				case 1 :
					printf("Parser Error 8: undeclared identifier used in assignment statement\n");
					break;
				case 2 :
					printf("Parser Error 8: undeclared identifier used in call statement\n");
					break;
				case 3 :
					printf("Parser Error 8: undeclared identifier used in read statement\n");
					break;
				case 4 :
					printf("Parser Error 8: undeclared identifier used in arithmetic expression\n");
					break;
				default :				
					printf("Implementation Error: unrecognized error code\n");
			}
			break;
		case 9 :
			printf("Parser Error 9: variables and constants cannot be called\n");
			break;
		case 10 :
			printf("Parser Error 10: begin must be followed by end\n");
			break;
		case 11 :
			printf("Parser Error 11: if must be followed by then\n");
			break;
		case 12 :
			printf("Parser Error 12: while must be followed by do\n");
			break;
		case 13 :
			printf("Parser Error 13: procedures and constants cannot be read\n");
			break;
		case 14 :
			printf("Parser Error 14: missing {\n");
			break;
		case 15 :
			printf("Parser Error 15: { must be followed by }\n");
			break;
		case 16 :
			printf("Parser Error 16: missing relational operator\n");
			break;
		case 17 :
			printf("Parser Error 17: procedures cannot be used in arithmetic\n");
			break;
		case 18 :
			printf("Parser Error 18: ( must be followed by )\n");
			break;
		case 19 :
			printf("Parser Error 19: invalid expression\n");
			break;
		default:
			printf("Implementation Error: unrecognized error code\n");

	}
}




void print_assembly_code()
{
  
	int i;
	printf("Assembly Code:\n");
	printf("Line\tOP Code\tOP Name\tL\tM\n");
	for (i = 0; i < code_index; i++)
	{
		printf("%d\t%d\t", i, code[i].op);
		switch(code[i].op)
		{
			case LIT :
				printf("LIT\t");
				break;
			case OPR :
				switch (code[i].m)
				{
					case RTN :
						printf("RTN\t");
						break;
					case ADD : // DO NOT ATTEMPT TO IMPLEMENT THIS, YOU WILL GET A ZERO IF YOU DO
						printf("ADD\t");
						break;
					case SUB : // DO NOT ATTEMPT TO IMPLEMENT THIS, YOU WILL GET A ZERO IF YOU DO
						printf("SUB\t");
						break;
					case MUL : // DO NOT ATTEMPT TO IMPLEMENT THIS, YOU WILL GET A ZERO IF YOU DO
						printf("MUL\t");
						break;
					case DIV : // DO NOT ATTEMPT TO IMPLEMENT THIS, YOU WILL GET A ZERO IF YOU DO
						printf("DIV\t");
						break;
					case EQL : // DO NOT ATTEMPT TO IMPLEMENT THIS, YOU WILL GET A ZERO IF YOU DO
						printf("EQL\t");
						break;
					case NEQ : // DO NOT ATTEMPT TO IMPLEMENT THIS, YOU WILL GET A ZERO IF YOU DO
						printf("NEQ\t");
						break;
					case LSS : // DO NOT ATTEMPT TO IMPLEMENT THIS, YOU WILL GET A ZERO IF YOU DO
						printf("LSS\t");
						break;
					case LEQ : // DO NOT ATTEMPT TO IMPLEMENT THIS, YOU WILL GET A ZERO IF YOU DO
						printf("LEQ\t");
						break;
					case GTR : // DO NOT ATTEMPT TO IMPLEMENT THIS, YOU WILL GET A ZERO IF YOU DO
						printf("GTR\t");
						break;
					case GEQ : // DO NOT ATTEMPT TO IMPLEMENT THIS, YOU WILL GET A ZERO IF YOU DO
						printf("GEQ\t");
						break;
					default :
						printf("err\t");
						break;
				}
				break;
			case LOD :
				printf("LOD\t");
				break;
			case STO :
				printf("STO\t");
				break;
			case CAL :
				printf("CAL\t");
				break;
			case INC :
				printf("INC\t");
				break;
			case JMP :
				printf("JMP\t");
				break;
			case JPC : // DO NOT ATTEMPT TO IMPLEMENT THIS, YOU WILL GET A ZERO IF YOU DO
				printf("JPC\t");
				break;
			case SYS :
				switch (code[i].m)
				{
					case WRT : // DO NOT ATTEMPT TO IMPLEMENT THIS, YOU WILL GET A ZERO IF YOU DO
						printf("WRT\t");
						break;
					case RED :
						printf("RED\t");
						break;
					case HLT :
						printf("HLT\t");
						break;
					default :
						printf("err\t");
						break;
				}
				break;
			default :
				printf("err\t");
				break;
		}
		printf("%d\t%d\n", code[i].l, code[i].m);
	}
	printf("\n");
}



void print_symbol_table()
{
	int i;
	printf("Symbol Table:\n");
	printf("Kind | Name        | Value | Level | Address | Mark\n");
	printf("---------------------------------------------------\n");
	for (i = 0; i < table_index; i++)
		printf("%4d | %11s | %5d | %5d | %5d | %5d\n", table[i].kind, table[i].name, table[i].value, table[i].level, table[i].address, table[i].mark); 
	printf("\n");
}