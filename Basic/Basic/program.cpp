/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program.h"
#include "statement.h"
#include "../StanfordCPPLib/tokenscanner.h"
using namespace std;

Program::Program() {
	firstline=NULL;
	checkfirst=false;
   // Replace this stub with your own code
}

Program::~Program() {
   // Replace this stub with your own code
}

void Program::clear1(){
	while(firstline!=NULL){
		Line* tmpline=firstline;
		firstline=firstline->next;
		delete tmpline;
	}
	if(firstline==NULL) return;
}

void Program::addSourceLine(int lineNumber, string line) {	
	checkfirst=false;
	if(firstline==NULL)
		firstline=new Line(lineNumber,line);
	else {
		if(findLineNumber(lineNumber)==true){
			if(firstline->linenum==lineNumber)
				firstline=firstline->next;
			removeSourceLine(lineNumber);
			}
		Line* strline=firstline;
		if(checkfirst==false){
			strline=new Line(lineNumber,line);
			strline->next=firstline;
			firstline=strline;
		}
		else{ 
			while(strline->next!=NULL&&strline->next->linenum<lineNumber) strline=strline->next;
			if(strline->next==NULL) strline->next=new Line(lineNumber,line);
			else{
				Line* nextline=strline->next;
			    strline->next=new Line(lineNumber,line);
			    strline=strline->next;
			    strline->next=nextline;
			}
		}
	}
	return;
}

void Program::removeSourceLine(int lineNumber) {
	Line* strline=firstline;
	Line* beforeLine=NULL;
	while(strline!=NULL&&strline->linenum<lineNumber) {
		beforeLine=strline;
		strline=strline->next;}
	if(strline->linenum!=lineNumber)  return;
	if(beforeLine==NULL){
		Line* tmpline=firstline;
		firstline=firstline->next;
		delete tmpline;
		return;
	}
	else{
	Line* tmpline=strline;
	strline=strline->next;
	beforeLine->next=strline;
	delete tmpline;
	return;
	}
}

string Program::getSourceLine(int lineNumber) {
	Line* strline=firstline;
	if(firstline==NULL) return"";
	if(findLineNumber(lineNumber)==false)
		return "";
	else{
		while(strline!=NULL&&strline->linenum<lineNumber) strline=strline->next;
		if(strline==NULL) return"";
		return strline->linestr;
	}
}

void Program::setParsedStatement(int lineNumber, Statement *statement) {
	Line* strline=firstline;
	if(firstline->linenum==lineNumber) {firstline->stmt=statement; return;}
	while(strline!=NULL&&strline->linenum<lineNumber)
	  strline=strline->next;
	if(strline->linenum!=lineNumber) {cout<<"error"<<endl;return;}
	strline->stmt=statement;
	return;
}

Statement *Program::getParsedStatement(int lineNumber) {
   Line* strline=firstline;
	while(strline->linenum!=lineNumber) 
	 strline=strline->next;
   if(strline->stmt==NULL)
	  return NULL;
   else return strline->stmt;
}

int Program::getFirstLineNumber() {
	if(firstline==NULL) return -1;
	else
	   	return firstline->linenum;     
}

int Program::getNextLineNumber(int lineNumber) {
	Line* strline=firstline;
	while(strline!=NULL&&strline->linenum<=lineNumber) strline=strline->next;
	if(strline==NULL) return -1;
	else {
		int num=strline->linenum;
		return num;}
}

bool Program::findLineNumber(int lineNumber) {
	Line* strline=firstline;
	if(firstline->linenum>lineNumber) return false;
	else if(firstline->linenum==lineNumber) return true;
	else{
		checkfirst=true; 
		while(strline!=NULL&&strline->linenum<lineNumber) 
		  strline=strline->next;
		if(strline==NULL) return false;
	if(strline->linenum==lineNumber) return true;
	else return false;
	}
}

void Program::run(EvalState & state){
	Line* strline=firstline;
	int num;
	while(strline!=NULL){
		Statement* tmpStmt=strline->stmt;
		if(tmpStmt->getType()==ENDstmt) break;
		else if(tmpStmt->getType()==REMstmt) {strline=strline->next;continue;}
		else if(tmpStmt->getType()==IFstmt||tmpStmt->getType()==GOTOstmt) {
			num=tmpStmt->execute3(state);
			if(num==-2) return;
			else if(num==-1) {strline=strline->next;continue;}
			else {
				strline=firstline;
				while(strline!=NULL&&strline->linenum<num) strline=strline->next;
				if(strline==NULL||strline->linenum!=num) {cout<<"LINE NUMBER ERROR"<<endl; return;}
				else continue;
			}
		}	
		else {tmpStmt->execute1(state);
		strline=strline->next;}
	}
	return;
}

void Program::list(){
	if(firstline==NULL) return;
	Line* strline=firstline;
	while(strline!=NULL){
		cout<<strline->linestr<<endl;
		strline=strline->next;
	}
	return;
}

void Program::clearOrder(Program& program,EvalState & state){
	program.clear1();
	state.clear();
	return;
}

void Program::help(){
	return;
}
