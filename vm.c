//Colton Curtis VM HW1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"

#define ARRAY_SIZE 500

void print_stack(int PC, int BP, int SP, int GP, int *pas, int *bars);
void print_instruction(int flag, int PC, int* IR);
int base(int *pas, int BP, int L);

void execute(int trace_flag, instruction *code){

  if(code == NULL){
    return NULL;
  }
  
  int pas[ARRAY_SIZE];//array to hold the stack
  int bars[ARRAY_SIZE];
  int OP, L, M, PC, IR, BP, SP;//create the variables
  
  
  for(int i = 0; i<ARRAY_SIZE; i++){//Initallize the array
    pas[i] = 0;
    bars[i] = 0;
  }

  int x = 0;
  int i = 0;
  while(code[i].m != -1){ //fetch cycle
    pas[x] = code[i].op;
    pas[x+1] = code[i].l;
    pas[x+2] = code[i].m;
    i++;
    x += 3;//Move the index holder
  }



  //Set all the variables to begin the execute cycle
  BP = x;
  SP = BP-1;
  PC = 0;

  //Halt variable
  int halt = 1;
  int z = 0;//test variable
  int y = x;//Holds the origional start of the struct 
  while(halt != 0){
    switch(pas[PC]){
      case 1 :{//LIT
        print_instruction(trace_flag, PC, pas);//print at the top of every case
        SP++;
        pas[SP] = pas[PC+2];
        PC = PC+3;//Always increment to the next instruction set if PC is not altered
        break;
      }
      case 2 :{//OPR
        switch(pas[PC+2]){
          case 0:{//RTN
            bars[BP] = 0;
            print_instruction(trace_flag, PC, pas);
            SP = BP-1;
            BP = pas[SP+2];
            PC = pas[SP+3];
            break;
          }
          case 1:{//ADD (adds the two top numbers and the result is put in the lower index)
            print_instruction(trace_flag, PC, pas);
            pas[SP-1] = pas[SP-1] + pas[SP];
            SP--;
            PC = PC+3;
            break;
            }
          case 2:{//SUB
            print_instruction(trace_flag, PC, pas);
            pas[SP-1] = pas[SP-1] - pas[SP];//subtraction
            SP--;
            PC = PC+3;
            break;
            }
          case 3:{//MUL
            print_instruction(trace_flag, PC, pas);
            pas[SP-1] = pas[SP-1] * pas[SP];//multiplication
            SP--;
            PC = PC+3;
            break;
            }
          case 4:{//DIV
            print_instruction(trace_flag, PC, pas);
            pas[SP-1] = pas[SP-1] / pas[SP];//divided
            SP--;
            PC = PC+3;
            break;
            }
          case 5:{//EQL
            print_instruction(trace_flag, PC, pas);
            if(pas[SP-1]==pas[SP]){//check to see if they are equal
              pas[SP-1] = 1;
            }
            else
              pas[SP-1] = 0;
            SP--;
            PC = PC+3;
            break;
            }
            
          case 6:{//NEQ
            print_instruction(trace_flag, PC, pas);
            if(pas[SP-1]==pas[SP]){//Check to see if they are equal
              pas[SP-1] = 0;
            }
            else
              pas[SP-1] = 1;
            SP--;
            PC = PC+3;
            break;
            }
          case 7:{//LSS
            print_instruction(trace_flag, PC, pas);//check to see if less than
            if(pas[SP-1]<pas[SP]){
              pas[SP-1] = 1;
            }
            else
              pas[SP-1] = 0;
            SP--;
            PC = PC+3;
            break;
            }
          case 8:{//LEQ
            print_instruction(trace_flag, PC, pas);//Less than or equal to
            if(pas[SP-1]<=pas[SP]){
              pas[SP-1] = 1;
            }
            else
              pas[SP-1] = 0; 
            SP--;
            PC = PC+3;
            break;
            }
          case 9:{//GTR
            print_instruction(trace_flag, PC, pas);//Greater than
            if(pas[SP-1]>pas[SP]){
              pas[SP-1] = 1;
            }
            else
              pas[SP-1] = 0;
            SP--;
            PC = PC+3;
            break;
            }
          case 10:{//GEQ
            print_instruction(trace_flag, PC, pas);//Check to see if greater than or equal
            if(pas[SP-1]>=pas[SP]){
              pas[SP-1] = 1;
            }
            else
              pas[SP-1] = 0;
            SP--;
            PC = PC+3;
            break;
            printf("%d", x);
            }
          }
      
          break;
        }
      case 3 :{//LOD
        print_instruction(trace_flag, PC, pas);//Loads the top value from the stack
        SP++;
        pas[SP] = pas[base(pas,BP,pas[PC+1])+pas[PC+2]];
        PC = PC+3;
        break;
        }
      case 4 :{//STO
        print_instruction(trace_flag, PC, pas);//Stores the value at the top of the stack
        pas[base(pas, BP, pas[PC+1])+pas[PC+2]] = pas[SP];
        SP--;
        PC = PC+3;
        break;
        }
      case 5 :{//CAL
        //New Registar
        print_instruction(trace_flag, PC, pas);
        pas[SP+1] = base(pas, BP, pas[PC+1]);
        pas[SP+2] = BP;
        pas[SP+3] = PC+3;
        BP = SP+1;
        PC = pas[PC+2];
        bars[BP] = 1;
        break;
        }
      case 6 :{//INC
        print_instruction(trace_flag, PC, pas);
        SP = SP+pas[PC+2];//Increment the SP from the instructions
        PC = PC+3;
        break;
        }
      case 7 :{//JMP
        print_instruction(trace_flag, PC, pas);
        PC = pas[PC+2];//Jump to the input from instruction
        break;
        }
      case 8 :{//JPC
        print_instruction(trace_flag, PC, pas);
        if(pas[SP]==0){
          PC = pas[PC+2];
          //printf("-----%d-----", pas[PC+2]);
        }
        else{
          PC = PC+3;
        }
        SP--;
        break;
        }
      case 9 :{
        switch(pas[PC+2]){
          case 1:{//WRT
            if(trace_flag == 1)
              printf("Output : %d\n", pas[SP]);
            SP--;
            print_instruction(trace_flag, PC, pas);//Prints out the SP and the instruction
            PC = PC+3;
            break;
            }
          case 2:{//RED
            SP++;
            if(trace_flag == 1)
              scanf("Output : %d\n", &pas[SP]);//Gives input to the SP
            print_instruction(trace_flag, PC, pas);
            PC = PC+3;
            break;
            }
          case 3:{//HLT
            print_instruction(trace_flag, PC, pas);
            halt = 0;//Ends the code by changing the halt
            break;
            }
        }
      }
  }
  if(trace_flag == 1)
    print_stack(PC, BP, SP, y, pas, bars);
    
  z++;
    // test variable 
    /* if(z==20){
      return 0;
    } */
  }
}





void print_stack(int PC, int BP, int SP, int GP, int *pas, int *bars)
{
  int i;
  printf("%d\t%d\t%d\t", PC, BP, SP);//Prints out the instruction set
  for (i = GP; i <= SP; i++)
  {
    if (bars[i] == 1)//Add the bar if needed (new registar)
      printf("| %d ", pas[i]);
    else
      printf("%d ", pas[i]);
  }

  
  printf("\n");
}

void print_instruction(int flag, int PC, int* IR)
{
  if(flag == 0)
    return;
  
  // Takes in the input from the stack and converts it to the appropriate english operation name 
  char opname[4];
  switch (IR[PC])
  {
    case 1 : strcpy(opname, "LIT"); break;
    case 2 :
    switch (IR[PC+2])
  {
    case 0 : strcpy(opname, "RTN"); break;
    case 1 : strcpy(opname, "ADD"); break;
    case 2 : strcpy(opname, "SUB"); break;
    case 3 : strcpy(opname, "MUL"); break;
    case 4 : strcpy(opname, "DIV"); break;
    case 5 : strcpy(opname, "EQL"); break;
    case 6 : strcpy(opname, "NEQ"); break;
    case 7 : strcpy(opname, "LSS"); break;
    case 8 : strcpy(opname, "LEQ"); break;
    case 9 : strcpy(opname, "GTR"); break;
    case 10 : strcpy(opname, "GEQ"); break;
    default : strcpy(opname, "err"); break;
  }
  break;
    case 3 : strcpy(opname, "LOD"); break;
    case 4 : strcpy(opname, "STO"); break;
    case 5 : strcpy(opname, "CAL"); break;
    case 6 : strcpy(opname, "INC"); break;
    case 7 : strcpy(opname, "JMP"); break;
    case 8 : strcpy(opname, "JPC"); break;
    case 9 :
    switch (IR[PC+2])
  {
    case 1 : strcpy(opname, "WRT"); break;
    case 2 : strcpy(opname, "RED"); break;
    case 3 : strcpy(opname, "HLT"); break;
    default : strcpy(opname, "err"); break;
  }
    break;
    default : strcpy(opname, "err"); break;
  }
  printf("%d\t%s\t%d\t%d\t",PC/3, opname, IR[PC+1], IR[PC+2]);
}

int base(int *pas, int BP, int L)
{
  int arb = BP; // arb = activation record base
  while ( L > 0)     //find base L levels down
  {
    arb = pas[arb];
    L--;
  }
  return arb;
}