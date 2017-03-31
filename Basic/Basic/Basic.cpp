/*
 * File: Basic.cpp
 * ---------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the BASIC interpreter from
 * Assignment #6.
 * [TODO: extend and correct the documentation]
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "statement.h"
#include <stdlib.h>
#include "evalstate.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"

#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"
using namespace std;

/* Function prototypes */

void processLine(string line, Program & program, EvalState & state);
void executedDirect(string line,EvalState & state,string token);
bool isDigit(string str);
/* Main program */

int main() {
   EvalState state;
   Program program;
   while (true) {
      try {
         processLine(getLine(), program, state);
      } catch (ErrorException & ex) {
         cerr << "Error: " << ex.getMessage() << endl;
      }
   }
   return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version,
 * the implementation does exactly what the interpreter program
 * does in Chapter 19: read a line, parse it as an expression,
 * and then print the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(string line, Program & program, EvalState & state) {
	TokenScanner scanner;
	scanner.ignoreWhitespace();
	scanner.scanNumbers();
	scanner.setInput(line);
        string first_token= scanner.nextToken();
	if(first_token=="LET"||first_token=="PRINT"||first_token=="INPUT")
	{executedDirect(line,state,first_token); return;}
	int linenum=0;
	bool digitFlag=isDigit(first_token);
	if(digitFlag){
		linenum=atoi(first_token.c_str());
		if(linenum<0) {cout<<"SYNTAX ERROR"<<endl; return;}
	}
	if(!scanner.hasMoreTokens()){ 
				if(first_token=="RUN") program.run(state);
	            else if(first_token=="LIST") program.list();
				else if(first_token=="CLEAR") program.clearOrder(program,state);
				else if(first_token=="QUIT") exit(0);
				else if(first_token=="HELP") program.help();
	else if(digitFlag) {if(program.findLineNumber(linenum)) program.removeSourceLine(linenum); }
	else {cout<<"SYNTAX ERROR"<<endl;return;}
	}
		else{
        if(digitFlag) {
		string second_token=scanner.nextToken();
		bool flag=false;
		Statement *stmt=NULL;
		if(second_token=="REM") {
			stmt=new REMSTMT();}
		else if(second_token=="INPUT"){
			stmt=new INPUTSTMT(line);
			flag=stmt->checkError();
		}
		else if(second_token=="LET"){
			stmt=new LETSTMT(line);
			flag=stmt->checkError();
		}
		else if(second_token=="PRINT"){
			stmt=new PRINTSTMT(line);
			flag=stmt->checkError();
		}
		else if(second_token=="END"){
			stmt=new ENDSTMT(line);
			flag=stmt->checkError();
		}
		else if(second_token=="IF"){
			stmt=new IFSTMT(line);
			flag=stmt->checkError();
		}
		else if(second_token=="GOTO"){
			stmt=new GOTOSTMT(line);
			flag=stmt->checkError();
		}
		else {delete stmt; cout<<"SYNTAX ERROR"<<endl; return;}
		if(flag=false)  return;
		program.addSourceLine(linenum,line);
		program.setParsedStatement(linenum,stmt);
	}	
	else {cout<<"SYNTAX ERROR"<<endl;return;}
	}
	return;
}

bool isDigit(string str){
	bool floatflag=false;
	for(int i=0;i<str.size();i++){
		if(!floatflag&&str[i]=='.') {floatflag=true;continue;}
		if(str[i]<'0'||str[i]>'9') return false;	
		else continue;
	}
	return true;
}

void executedDirect(string line, EvalState & state,string token){
	if(token=="LET"){
		Statement *stmt=new LETSTMT(line);
		bool flag=stmt->checkError();
		if(flag==false) return;
		else stmt->executeDirect(state);
	}
	else if(token=="PRINT"){
		Statement *stmt=new PRINTSTMT(line);
		bool flag=stmt->checkError();
		if(flag==false) return;
		else stmt->execute1(state);
	}
	else if(token=="INPUT"){
		Statement *stmt=new INPUTSTMT(line);
		bool flag=stmt->checkError();
		if(flag==false) return;
		else stmt->execute1(state);
	} 
  return;	
}
