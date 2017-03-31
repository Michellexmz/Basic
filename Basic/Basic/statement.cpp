/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"
#include "evalstate.h"
#include "exp.h"
#include "parser.h"
#include <stdlib.h>
#include "../StanfordCPPLib/tokenscanner.h"
#include "../StanfordCPPLib/simpio.h"
using namespace std;

/* Implementation of the Statement class */

Statement::Statement() {
   /* Empty */
}

Statement::~Statement() {
   /* Empty */
}

bool Statement::checkKeyWord(string line){
	TokenScanner scanner;
	scanner.ignoreWhitespace();
	scanner.scanNumbers();
	scanner.setInput(line);
	while(scanner.hasMoreTokens()){
		string tmpToken=scanner.nextToken();
		if(tmpToken=="REM"||tmpToken=="LET"||tmpToken=="PRINT"||tmpToken=="INPUT"||tmpToken=="END"||tmpToken=="IF"||tmpToken=="THEN"||tmpToken=="GOTO"||tmpToken=="LIST"||tmpToken=="CLEAR"||tmpToken=="QUIT"||tmpToken=="HELP"||tmpToken=="RUN") {cout<<"SYNTAX ERROR"<<endl;return false;}
		else continue;
	}
	return true;
}
bool Statement::checkIfParsed(string thestring,EvalState & state){
	TokenScanner scanner;
	scanner.ignoreWhitespace();
	scanner.scanNumbers();
	scanner.setInput(thestring);
	while(scanner.hasMoreTokens()){
		string tmpToken=scanner.nextToken();
		TokenType tokenType=scanner.getTokenType(tmpToken);
		 if(tokenType==WORD&&!state.isDefined(tmpToken)){
				cout<<"VARIABLE NOT DEFINED"<<endl;
				return false;
			}
		else continue;
	}
	return true;
}

REMSTMT::REMSTMT() {
   /* Empty */
}

void REMSTMT::execute2() {
   return;
}

StatementType REMSTMT::getType(){
	return REMstmt;
}

bool LETSTMT::checkError() {
   	TokenScanner scanner;
     scanner.ignoreWhitespace();
     scanner.scanNumbers();
     scanner.setInput(strline);
     string first=scanner.nextToken();
	 if(scanner.getTokenType(first)==NUMBER)
        string next=scanner.nextToken();
   if(!scanner.hasMoreTokens()) {cout<<"SYNTAX ERROR"<<endl;return false;}
   string firstToken=scanner.nextToken(); 
     if(!scanner.hasMoreTokens()) {cout<<"SYNTAX ERROR"<<endl;return false;}
	 TokenScanner newScanner;
	 string op=scanner.nextToken();
     if(op!="="){
		 cout<<"SYNTAX ERROR"<<endl;
		 return false;
	 }
     if(!scanner.hasMoreTokens()){cout<<"SYNTAX ERROR"<<endl; return false;}
	 while(scanner.hasMoreTokens()){
		 string nexttoken=scanner.nextToken();
		 if(scanner.getTokenType(nexttoken)!=NUMBER&&scanner.getTokenType(nexttoken)!=WORD&&scanner.getTokenType(nexttoken)!=OPERATOR)
		 {cout<<"SYNTAX ERROR"<<endl; return false;}
		 else {expStr.append(nexttoken);
			 expStr.append(" ");
			 newScanner.saveToken(nexttoken);
		 }
	 }
	 bool keyflag=Statement::checkKeyWord(firstToken);
	 keyflag=Statement::checkKeyWord(expStr);
	 if(!keyflag) return false;
    TokenScanner rightScanner;
   while(newScanner.hasMoreTokens()) rightScanner.saveToken(newScanner.nextToken());
	 exp=parseExp(rightScanner);
	 if(exp==NULL) {cout<<"SYNTAX ERROR"<<endl; return false;}
	 else
	 { var=firstToken; return true;}
}

void LETSTMT::execute1(EvalState & state){
	bool letFlag=Statement::checkIfParsed(expStr,state);
	if(!letFlag) return;
	else{
		int value=exp->eval(state);
		state.setValue(var,value);
	    return;
	}
}

void LETSTMT::executeDirect(EvalState & state){
	bool letFlag=Statement::checkIfParsed(expStr,state);
	if(!letFlag) return;
	bool flag=false;
	TokenScanner scanner;
	scanner.ignoreWhitespace();
	scanner.scanNumbers();
	scanner.setInput(expStr);
	string token=scanner.nextToken();
	scanner.saveToken(token);
	TokenType type=scanner.getTokenType(token);
	if(type!=WORD&&type!=NUMBER){cout<<"SYNTAX ERROR"<<endl; return;}
	else{
		int value=exp->eval(state);
		state.setValue(var,value);
		return;
	}
}

StatementType LETSTMT::getType(){
	return LETstmt;
}

bool PRINTSTMT::checkError(){
   	TokenScanner scanner;
     scanner.ignoreWhitespace();
     scanner.scanNumbers();
     scanner.setInput(strline);
     string first=scanner.nextToken();
	 if(scanner.getTokenType(first)==NUMBER)
        string next=scanner.nextToken();
	if(!scanner.hasMoreTokens()) {cout<<"SYNTAX ERROR"<<endl;return false;}
   TokenScanner newScanner;
   while(scanner.hasMoreTokens()){
   string tmpToken=scanner.nextToken();
   expStr.append(tmpToken);
   expStr.append(" ");
   newScanner.saveToken(tmpToken);
   }
 bool keyflag=Statement::checkKeyWord(expStr);
if(!keyflag) return false; 
   TokenScanner rightScanner;
  while(newScanner.hasMoreTokens()) rightScanner.saveToken(newScanner.nextToken()); 
	exp=parseExp(rightScanner);
      if(exp==NULL) {cout<<"SYNTAX ERROR"<<endl; return false;}
        else return true;   	
}

void PRINTSTMT::execute1(EvalState & state){
	bool printflag=Statement::checkIfParsed(expStr,state);
	if(!printflag) return;
	else{
		int val=exp->eval(state);
                if(val==-999999) return;
		cout<<val<<endl;
		return;
	}
}

StatementType PRINTSTMT::getType(){
	return PRINTstmt;
}

bool INPUTSTMT::checkError()
{   	
	TokenScanner scanner;
     scanner.ignoreWhitespace();
     scanner.scanNumbers();
     scanner.setInput(strline);
     string first=scanner.nextToken();
	 if(scanner.getTokenType(first)==NUMBER)
        string next=scanner.nextToken();
	if(!scanner.hasMoreTokens()) {cout<<"SYNTAX ERROR"<<endl;return false;}
         string token=scanner.nextToken();
		 bool keyflag=Statement::checkKeyWord(token);
         if(!keyflag) return false;
		 if(scanner.hasMoreTokens()) {cout<<"SYNTAX ERROR"<<endl;return false;}
		 TokenType type=scanner.getTokenType(token);
		 if(type==WORD) var=token;
		 else {cout<<"SYNTAX ERROR"<<endl; return false;}
		 return true;
}

void INPUTSTMT::execute1(EvalState & state){
	bool numberchecker=false;
	string str;
	while(!numberchecker){
		cout<<" ? ";
	str=getLine();
	numberchecker=checkNum(str);
	}
	int val=atoi(str.c_str()); 
	state.setValue(var,val);
	return;
}

bool INPUTSTMT::checkNum(string str){
     bool flag=false;
     for(int i=0;i<str.size();i++){
         if(str[i]<='9'&&str[i]>='0') continue;
	else if(!flag&&str[i]=='-'){flag=true;continue;}
	else {cout<<"INVALID NUMBER"<<endl;return false;}}
	 return true;
}

StatementType INPUTSTMT::getType(){
	return INPUTstmt;
}

bool ENDSTMT::checkError(){
	TokenScanner scanner;
     scanner.ignoreWhitespace();
     scanner.scanNumbers();
     scanner.setInput(strline);
     string first=scanner.nextToken();
	 if(scanner.getTokenType(first)==NUMBER)
        string next=scanner.nextToken();
	if(!scanner.hasMoreTokens()) return true;
	else {cout<<"SYNTAX ERROR"<<endl;return false;}
}

void ENDSTMT::execute2(){
	return;
}

StatementType ENDSTMT::getType(){
	return ENDstmt;
}

bool GOTOSTMT::checkError(){
	   	TokenScanner scanner;
     scanner.ignoreWhitespace();
     scanner.scanNumbers();
     scanner.setInput(strline);
     string first=scanner.nextToken();
	 if(scanner.getTokenType(first)==NUMBER)
        string next=scanner.nextToken();
	if(!scanner.hasMoreTokens()) {cout<<"SYNTAX ERROR"<<endl;return false;}
	string token=scanner.nextToken();
	if(scanner.hasMoreTokens()) {cout<<"SYNTAX ERROR"<<endl;return false;}
	scanner.saveToken(token);
	if(scanner.getTokenType(token)==NUMBER){
		linenumber=atoi(token.c_str());
		if(linenumber<0) {cout<<"SYNTAX ERROR"<<endl; return false;}
	    else return true;
	}
	else 
	return false;
}

int GOTOSTMT::execute3(EvalState & state){
	return linenumber;
}

StatementType GOTOSTMT::getType(){
	return GOTOstmt;
}

bool IFSTMT::checkError(){
	TokenScanner scanner1,scanner2;
	   	TokenScanner scanner;
     scanner.ignoreWhitespace();
     scanner.scanNumbers();
     scanner.setInput(strline);
     string first=scanner.nextToken();
	 if(scanner.getTokenType(first)==NUMBER)
        string next=scanner.nextToken();
	if(!scanner.hasMoreTokens()) {cout<<"SYNTAX ERROR"<<endl;return false;}
	bool scanner1flag=false,scanner2flag=false;
     while(scanner.hasMoreTokens()){
	    string token=scanner.nextToken(); 
		if(!scanner1flag&&(token=="="||token==">"||token=="<")) {op=token;scanner1flag=true;continue;}
        if(scanner1flag&&token=="THEN") {scanner2flag=true;continue;}
		if(!scanner1flag) {
			exp1Str.append(token);
			exp1Str.append(" ");
			scanner1.saveToken(token);
		continue;}
	    if(!scanner2flag) {
			exp2Str.append(token);
			exp2Str.append(" ");
			scanner2.saveToken(token);
			continue;
		}
		if(scanner2flag){
		if(scanner.hasMoreTokens()){cout<<"SYNTAX ERROR"<<endl;return false;}
		else if(scanner.getTokenType(token)!=NUMBER) {cout<<"SYNTAX ERROR"<<endl; return false;}
		num=atoi(token.c_str());
		if(num<0) {cout<<"SYNTAX ERROR"<<endl;return false;}
		}
	 }
	 if(!scanner1flag||!scanner2flag) {cout<<"SYNTAX ERROR"<<endl; return false;}
	 bool keyflag1=Statement::checkKeyWord(exp1Str);
	 if(!keyflag1) return false;
	 bool keyflag2=Statement::checkKeyWord(exp2Str);
      if(!keyflag2) return false;

	 TokenScanner right1Scanner;
	    while(scanner1.hasMoreTokens()) right1Scanner.saveToken(scanner1.nextToken());
	 exp1=parseExp(right1Scanner); 
	 if(exp1==NULL) {cout<<"SYNTAX ERROR"<<endl; return false;}
	     TokenScanner right2Scanner;
	    while(scanner2.hasMoreTokens()) right2Scanner.saveToken(scanner2.nextToken());
	 exp2=parseExp(right2Scanner);
	 if(exp2==NULL) {cout<<"SYNTAX ERROR"<<endl; return false;}
	 return true; 
}

int IFSTMT::execute3(EvalState & state){
	bool ifflag1=Statement::checkIfParsed(exp1Str,state);
	if(!ifflag1) return -2;
	bool ifflag2=Statement::checkIfParsed(exp2Str,state);
	if(!ifflag2) return -2;
	int val1=exp1->eval(state);
if(val1==-999999) return -2;
	int val2=exp2->eval(state);
if(val2==-999999) return -2;
	if(op=="="){
		if(val1==val2) return num;
		else return -1;}
    else if(op=="<"){
	   if(val1<val2) return num;
       else return -1;}
    else if(op==">"){
	   if(val1>val2) return num;
       else return -1;}	   
}

StatementType IFSTMT::getType(){
	return IFstmt;
}
