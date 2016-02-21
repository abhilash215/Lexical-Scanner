///////////////////////////////////////////////////////////////////////
// SemiExpression.h - collect tokens for analysis                    //
// ver 3.3                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Lexical Scanner,CSE687 - Object Oriented Design,SP16 //
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
* Maintenance History:
* --------------------
* ver 3.3 : 10 Feb 2016
* - New comments and functions are added
* ver 3.2 : 02 Feb 2016
* - declared SemiExp copy structor and assignment operator = delete
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
* Build Process:
* --------------
* Required Files:
*   SemiExpression.h, SemiExpression.cpp, Tokenizer.h, Tokenizer.cpp,ITokCollection.h
*
* Build Command: devenv Project1.sln /rebuild debug
*
*Public Interface
* ------------------
* find(const std::string& tok)
* -function to find the position of token
*remove(const std::string& tok)
* -function to remove the token from particular position
* remove(size_t n)
* - function to remove the token
* push_back(const std::string& tok)
* -function to load the vector with tokens
* length()
*function to find the length of the vector
*/

/*
** Planned Additions and Changes:
* ------------------------------
* - add public :, protected :, private : as terminators
* - move creation of tokenizer into semiExp constructor so
*   client doesn't have to write that code.
*
*/

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <exception>
#include "SemiExp.h"
#include "ITokCollection.h"
#include "../Tokenizer/Tokenizer.h"

using namespace Scanner;

SemiExp::SemiExp(Toker* pToker) : _pToker(pToker) {}

//<---- finds the character---->
size_t SemiExp::find(const std::string& tok) {
	int pos = 0;
	for (size_t i = 0;i < length();++i)
	{
		if (_tokens.at(i) == tok)
		{
			pos = i;
			break;
		}
	}
	return pos;
}

//<--removes that many tokens from vector
bool SemiExp::remove(size_t n)
{
	if (n<0 || n>length()) return false;
	std::vector<Token>::iterator it = _tokens.begin() + n;
	_tokens.erase(it);
	return true;
}

//<--removes that particular token vector
bool SemiExp::remove(const std::string& tok)
{	
	int pos = this->find(tok);
	_tokens.erase(_tokens.begin()+pos);
	return true;
}

//<--- removes leading new line chars
void SemiExp::trimFront()
{
	while (length() > 1)
	{
		if (_tokens[0] == "\n") this->remove(0);
		else break;
	}
	
}

bool SemiExp::get(bool clear)
{

  if (_pToker == nullptr)
    throw(std::logic_error("no Toker reference"));
  if(clear)
  _tokens.clear();
  while (true)
  {
    std::string token = _pToker->getTok();
	   if (token == "")
      break;
    _tokens.push_back(token);

	if (token == "{" || token == ";" || token == "}")
		return true;
	if (token == ":")
		return true;

	if (token == "for")
	{
		int c = 0;
		do {
			token = _pToker->getTok();
			_tokens.push_back(token);
			if (token == "(") c++;
			if (token == ")") c--;
		} while (c != 0);
		token = _pToker->getTok();
		_tokens.push_back(token);
		return true;
	}

	if (token == "#")
	{
		token = _pToker->getTok();
		while (token.size()>0)
		{
			if (isspace(token.at(0)))
				break;
			_tokens.push_back(token);
			token = _pToker->getTok();
		}
		return true;
	}

  }
  return false;
}

std::string& SemiExp::operator[](int n)
{
	int size = _tokens.size();
  if (n < 0 || n >= size)
    throw(std::invalid_argument("index out of range"));
  return _tokens[n];
}

size_t SemiExp::length()
{
  return _tokens.size();
}

std::string SemiExp::show(bool showNewLines)
{
  std::cout << "\n  ";
  std::string str;
  for (auto token : _tokens)
  {
	  str.append(token);
	  if (token != "\n")
		  std::cout << token << " ";
}
    std::cout << "\n";
  return str;
}

//<---to lower case
 void SemiExp::toLower() {
	 std::string& token = _pToker->getTok();
	 int size = token.size();
	 for (int i = 0;i < size;i++)
	 {
		 if (isalpha(token.at(i)))
		token.at(i)=tolower(token.at(i));
	 }
}

 //<---clears starting tokens
 void SemiExp::clear() { _tokens.clear(); }

 //<----pushes back---
 void SemiExp::push_back(const std::string& tok)
 {
	 _tokens.push_back(tok);
 }

 //<---mergers-----------------
 bool SemiExp::merge(const std::string& firstTok, const std::string& secondTok)
 {
	
	 return true;
 }


#ifndef TEST_SemiExp
#include<fstream>
 int submain()//submain
 {
	 Toker toker;
	 std::string fileSpec = "../SemiExp/semitest.txt";
	 std::fstream in(fileSpec);
	 if (!in.good())
	 {
		 std::cout << "\n  can't open file " << fileSpec << "\n\n";
		 return 1;
	 }
	 toker.attach(&in);

	 SemiExp semi(&toker);

	 while (semi.get())
	 {
		 std::cout << "\n  -- semiExpression --";
		 semi.show(true);
	 }
	 /*
		May have collected tokens, but reached end of stream
		before finding SemiExp terminator.
	  */
	 if (semi.length() > 0)
	 {
		 std::cout << "\n  -- semiExpression --";
		 semi.show(true);
		 std::cout << "\n\n";
	 }
	 return 0;
 }
#endif
 