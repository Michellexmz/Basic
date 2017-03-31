/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface specified in Chapter 17, which is an
 * excellent model for the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include "evalstate.h"
#include "exp.h"
#include "../StanfordCPPLib/tokenscanner.h"
enum StatementType {REMstmt,INPUTstmt,LETstmt,PRINTstmt,ENDstmt,IFstmt,GOTOstmt};
/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

class Statement {

public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

   Statement();

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

   virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */

   virtual void execute1(EvalState & state){};
   virtual void execute2(){};
   virtual int execute3(EvalState & state){};
   virtual bool checkIfParsed(string thestring,EvalState & state);
   virtual StatementType getType()=0;
   virtual bool checkError(){};
   bool checkKeyWord(string line);
   virtual void executeDirect(EvalState & state){};
};

/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.  If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */

class REMSTMT:public Statement {
public:
   REMSTMT();
   virtual void execute2();
   virtual StatementType getType();
};

class LETSTMT:public Statement {
public:
	LETSTMT(string line):strline(line),exp(NULL),var(""),expStr(""){};
   virtual void execute1(EvalState & state);
   virtual bool checkError();
   virtual StatementType getType();
   virtual void executeDirect(EvalState & state);
private:
   string strline;
   string var;
   Expression *exp;
   string expStr;
};

class PRINTSTMT:public Statement {
public:
 	PRINTSTMT(string line):strline(line),exp(NULL),expStr(""){};
	virtual bool checkError();
   virtual void execute1(EvalState & state);
   virtual StatementType getType();
private:
   string strline;
   Expression *exp;
   string expStr;
};

class INPUTSTMT:public Statement {
public:
   INPUTSTMT(string line):strline(line),var(""){};
   virtual bool checkError();
   virtual void execute1(EvalState & state);
   virtual StatementType getType();
   bool checkNum(string str);
private:
   string strline;
   string var;
};

class ENDSTMT:public Statement {
public:
 	ENDSTMT(string line):strline(line){};
	virtual bool checkError();
   virtual void execute2();
   virtual StatementType getType();
private:
   string strline;
};

class GOTOSTMT:public Statement {
public:
 	GOTOSTMT(string line):strline(line),linenumber(-1){};
	virtual bool checkError();
    virtual int execute3(EvalState & state);
   virtual StatementType getType();
private:
   string strline;
   int linenumber;
};

class IFSTMT:public Statement {
public:
	IFSTMT(string line):strline(line),exp1(NULL),exp2(NULL),op(""),exp1Str(""),exp2Str(""){};
   virtual bool checkError();
   virtual int execute3(EvalState & state);
   virtual StatementType getType();
private:
   string strline;
   string exp1Str;
   string exp2Str;
   Expression *exp1;
   Expression *exp2;
   int num;
   string op;
};
#endif
