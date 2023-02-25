# Integrating-PL-0-Compiler
This code is a PL/0 compiler

The driver.c file is the main file that initiates the compilation process. It reads the input program from the in.txt file and calls the parser and lexer functions to analyze the program. The parser.c file contains the code to parse the input program and generate a parse tree. The lexer analyzes the input program character by character and generates a token stream, which is consumed by the parser.

The lex.c file contains the code for the lexer. It uses regular expressions to recognize PL/0 tokens and returns the corresponding token type. The compiler.h file includes the necessary headers for the compiler to function correctly.

The output of the PL/0 compiler is saved in the out.txt file. The output includes a Lexeme List and Assembly Code. The Lexeme List displays the lexeme and its corresponding token type. The Assembly Code displays the generated code in assembly language.
