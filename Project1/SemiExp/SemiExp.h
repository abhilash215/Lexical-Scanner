#ifndef SEMIEXPRESSION_H
#define SEMIEXPRESSION_H
///////////////////////////////////////////////////////////////////////
// SemiExpression.h - collect tokens for analysis                    //
// ver 3.3                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Lexical Scanner, CSE687 -Object Oriented Design,SP16 //
//Platform:     Dell Inspiron 15,6th gen Core i5, Windows 10         //
// Author:      Abhilash Udayashankar, SUID:778388557                //
//              audayash@syr.edu, (774)-540-1234                     // 
//Original author::Jim Fawcett, CST 4-187, Syracuse University       //
//              (315) 443-3948, jfawcett@twcny.rr.com                // 
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////

/*
* Package Operations:
* -------------------
* This package provides a public SemiExp class that collects and makes
* available sequences of tokens.  SemiExp uses the services of a Toker
* class to acquire tokens.  Each call to SemiExp::get() returns a
* sequence of tokens that ends in {.  This will be extended to use the
* full set of terminators: {, }, ;, and '\n' if the line begins with #.
*
* This is a new version for students in CSE687 - OOD, Spring 2016 to use
* for Project #1.
*
* Build Process:
* --------------
* Required Files:
*   SemiExpression.h, SemiExpression.cpp, Tokenizer.h, Tokenizer.cpp,ITokCollection.h
*
* Build Command: devenv Project1.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 3.3 : 08 Feb 2016
* - New comments and functions are added
* ver 3.2 : 02 Feb 2016
* - declared SemiExp copy constructor and assignment operator = delete
* - added default argument for Toker pointer to nullptr so SemiExp
*   can be used like a container of tokens.
* - if pToker is nullptr then get() will throw logic_error exception
* ver 3.1 : 30 Jan 2016
* - changed namespace to Scanner
* - fixed bug in termination due to continually trying to read
*   past end of stream if last tokens didn't have a semiExp termination
*   character
* ver 3.0 : 29 Jan 2016
* - built in help session, Friday afternoon
*
* Planned Additions and Changes:
* ------------------------------
* - add public :, protected :, private : as terminators
* - move creation of tokenizer into semiExp constructor so
*   client doesn't have to write that code.
*/

#include <vector>
#include "../Tokenizer/Tokenizer.h"
#include "ITokCollection.h"

namespace Scanner
{
  using Token = std::string;

  class SemiExp
	  :ITokCollection
  {
  public:
    SemiExp(Toker* pToker = nullptr);
    SemiExp(const SemiExp&) = delete;
	std::string& operator[](int n);
    bool get(bool = true);
    size_t length();
	std::string show(bool showNewLines = false);
	size_t find(const std::string& tok);
	void push_back(const std::string& tok);
	bool remove(size_t s);
	void trimFront();
	void toLower();
	void clear();
	bool merge(const std::string& firstTok, const std::string& secondTok) ;
	 bool remove(const std::string& tok) ;
  private:
    std::vector<Token> _tokens;
	bool hasFor = false;
    Toker* _pToker;
  };
}
#endif
