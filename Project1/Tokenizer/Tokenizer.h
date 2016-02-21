#ifndef TOKENIZER_H
#define TOKENIZER_H
///////////////////////////////////////////////////////////////////////
// Tokenizer.cpp - implements the functions for getting the tokens   //
// ver 3.5                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Lexical Scanner,CSE687 - Object Oriented Design,SP16 //
//Platform:     Dell Inspiron,6th gen Core i5, Windows 10            //
// Author:      Abhilash Udayashankar, SUID:778388557                //
//              audayash@syr.edu, (774)-540-1234                     // 
//Original author::Jim Fawcett, CST 4-187, Syracuse University       //
//              (315) 443-3948, jfawcett@twcny.rr.com                // 
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////

/*
* Package Operations:
* -------------------
* This package provides a public Toker class and private ConsumeState class.
* Toker reads words from a std::stream, throws away whitespace and comments
* and returns words from the stream in the order encountered.  Quoted
* strings and certain punctuators and newlines are returned as single tokens.
*/

/*
** Build Process :
*--------------
* Required Files : Tokenizer.h
* Build Command : devenv Tokenizer.sln / rebuild debug
*/
/*
* Maintenance History :
*--------------------
* ver 3.5 : 10 Feb 2016
* -new functions are added to implement the toker class
* -Added comments to the file
* ver 3.4 : 03 Feb 2016
* -fixed bug that prevented reading of last character in source by
*   clearing stream errors at beginning of ConsumeState::nextState()
* ver 3.3 : 02 Feb 2016
* -declared ConsumeState copy constructor and assignment operator = delete
* ver 3.2 : 28 Jan 2016
* -fixed bug in ConsumeState::nextState() by returning a valid state
*   pointer if all tests fail due to reaching end of file instead of
*   throwing logic_error exception.
* ver 3.1 : 27 Jan 2016
* -fixed bug in EatCComment::eatChars()
* -removed redundant statements assigning _pState in derived eatChars()
*   functions
* -removed calls to nextState() in each derived eatChars() and fixed
*   call to nextState() in ConsumeState::consumeChars()
* ver 3.0 : 11 Jun 2014
* -first release of new design
*/
/*
*Public Interface
*----------------
* ConsumeState()
* -constructor
* ~Consumestate()
* -Deconstructor
* nextState()
* -
* attach()
* -attaches the file to the pointer
* getTok()
* - gets the tokens
* canRead()
* - tells whether if there are more tokens that can be read
* eatChars()
* - eats the characters one by one
* setvector()
* - sets a new single character to the vector
* setpairvector()
*- sets a pair of new characterts the vector
*/

#include <iosfwd>
#include <string>

namespace Scanner
{
	class ConsumeState;

	class Toker
	{
	public:
		Toker();
		Toker(const Toker&) = delete;
		~Toker();
		Toker& operator=(const Toker&) = delete;
		bool attach(std::istream* pIn);
		std::string getTok();                           //get tokens
		bool canRead();                                 //read tokens if possible
		void setvector();                               //sets a new single character to the vector
		void setpairvector();                           //sets a pair of new characterts the vector
		static void Reset();
	private:
		ConsumeState* pConsumer;
	};
}
#endif