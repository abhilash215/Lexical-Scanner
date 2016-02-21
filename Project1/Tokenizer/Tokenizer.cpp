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
* ver 3.5 : 08 Feb 2016
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
*/

#include "Tokenizer.h"
#include <iostream>
#include <cctype>
#include <string>
#include <vector>


namespace Scanner {
	class ConsumeState {
	public:
		ConsumeState();
		ConsumeState(const ConsumeState&) = delete;
		ConsumeState& operator=(const ConsumeState&) = delete;
		virtual ~ConsumeState();
		void attach(std::istream* pIn) { _pIn = pIn; }
		virtual void eatChars() = 0;
		void consumeChars() {
			_pState->eatChars();
			_pState = nextState();
		}
		bool canRead() { return _pIn->good(); }
		std::string getTok() { return token; }
		bool hasTok() { return token.size() > 0; }
		ConsumeState* nextState();
		void setSpecialSingleChars(std::string ssc) {
			std::cout << "\nBefore adding the new character to vector\n";
			for (unsigned int i = 0; i < _sscTokens.size(); i++)
				std::cout << _sscTokens[i];
			_sscTokens.push_back(ssc);
			std::cout << "\nAfter adding the new character to vector\n";
			for (unsigned int i = 0; i < _sscTokens.size(); i++)
				std::cout << _sscTokens[i];
		}
		void setSpecialCharPairs(std::string scp) {
			std::cout << "\nBefore adding the new character pair to vector\n";
			for (unsigned int i = 0; i < _scpTokens.size(); i++)
				std::cout << _scpTokens[i];
			_scpTokens.push_back(scp);
			std::cout << "\nAfter adding the new character  pair to vector\n";
			for (unsigned int i = 0; i < _scpTokens.size(); i++)
				std::cout << _scpTokens[i];
		}
		static void reset();

	protected:
		static std::vector<std::string> _sscTokens;
		static std::vector<std::string> _scpTokens;
		static std::string token;
		static std::istream* _pIn;
		static int prevChar;
		static int currChar;
		static ConsumeState* _pState;
		static ConsumeState* _pEatCppComment;
		static ConsumeState* _pEatCComment;
		static ConsumeState* _pEatWhitespace;
		static ConsumeState* _pEatPunctuator;
		static ConsumeState* _pEatAlphanum;
		static ConsumeState* _pEatNewline;
		static ConsumeState* _pEatquotedStrings;
		static ConsumeState* _pEatsinglequotedStrings;
		static ConsumeState* _pEatSpecialCharacterPair;
		static ConsumeState* _pEatSpecialSingleCharacter;

	};
}


using namespace Scanner;

std::string ConsumeState::token;
std::istream* ConsumeState::_pIn = nullptr;
int ConsumeState::prevChar;
int ConsumeState::currChar;
static bool first_ctor = true;
static bool first_dtor = true;
ConsumeState* ConsumeState::_pState = nullptr;
ConsumeState* ConsumeState::_pEatCppComment = nullptr;
ConsumeState* ConsumeState::_pEatCComment = nullptr;
ConsumeState* ConsumeState::_pEatWhitespace = nullptr;
ConsumeState* ConsumeState::_pEatPunctuator = nullptr;
ConsumeState* ConsumeState::_pEatAlphanum = nullptr;
ConsumeState* ConsumeState::_pEatquotedStrings = nullptr;
ConsumeState* ConsumeState::_pEatSpecialSingleCharacter = nullptr;
ConsumeState* ConsumeState::_pEatsinglequotedStrings = nullptr;
ConsumeState* ConsumeState::_pEatSpecialCharacterPair = nullptr;
std::vector<std::string> ConsumeState::_sscTokens = { "<", ">", "[", "]", "(", ")", "{", "}", ":", "=", "+", "-", "*", "\n",";","#" };
std::vector<std::string> ConsumeState::_scpTokens = { "<<", ">>", "::", "++", "==", "+=", "-=", "*=", "/=","--" };

ConsumeState* ConsumeState::_pEatNewline;

void testLog(const std::string& msg);

void ConsumeState::reset()
{
	first_ctor = true;
	first_dtor = true;
}

ConsumeState* ConsumeState::nextState()
{
	if (!(_pIn->good()))
		return nullptr;
	int chNext = _pIn->peek();
	std::string temp = "";
	temp += currChar;
	temp += chNext;
	if (chNext == EOF)
		_pIn->clear();
	// if peek() reads end of file character, EOF, then eofbit is set and
	// _pIn->good() will return false.  clear() restores state to good
	if (std::isspace(currChar) && currChar != '\n')
	{
		testLog("state: eatWhitespace");
		return _pEatWhitespace;
	}
	if (currChar == '/' && chNext == '/')
	{
		testLog("state: eatCppComment");
		return _pEatCppComment;
	}
	if (currChar == '/' && chNext == '*')
	{
		testLog("state: eatCComment");
		return _pEatCComment;
	}
	if (currChar == '\n')
	{
		testLog("state: eatNewLine");
		return _pEatNewline;
	}
	if (std::isalnum(currChar))
	{
		testLog("state: eatAlphanum");
		return _pEatAlphanum;
	}
	if (currChar == '\'')
	{
		testLog("state:singlequotedStrings");
		return _pEatsinglequotedStrings;
	}
	if (currChar == '"')
	{
		testLog("state:quotedStrings");
		return _pEatquotedStrings;
	}
	for (auto str : _scpTokens) {
		if (str == temp) {
			testLog("state: eatSpecialCharacterPair");
			return _pEatSpecialCharacterPair;
		}
	}
	for (auto str : _sscTokens) {
		if (str[0] == currChar) {
			testLog("state: eatSpecialSingleCharacter");
			return _pEatSpecialSingleCharacter;
		}
	}
	if (ispunct(currChar))
	{
		testLog("state: eatPunctuator");
		return _pEatPunctuator;
	}
	if (!_pIn->good())
		return _pEatWhitespace;

	throw(std::logic_error("invalid type"));
}

class EatWhitespace : public ConsumeState{
public:
	virtual void eatChars() {
		token.clear();
		do {
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (std::isspace(currChar) && currChar != '\n'); }
};

class EatCppComment : public ConsumeState{
public:
	virtual void eatChars() {
		token.clear();
		do {
			if (!_pIn->good())  // end of stream
				return;
			token += currChar;
			currChar = _pIn->get();
		} while (currChar != '\n');	}
};

class EatCComment : public ConsumeState{
public:
	virtual void eatChars() {
		token.clear();
		do {
			if (!_pIn->good())  // end of stream
				return;
			token += currChar;
			currChar = _pIn->get();
		} while (currChar != '*' || _pIn->peek() != '/');
		token += currChar;
		currChar = _pIn->get();
		token += currChar;
		currChar = _pIn->get();	}
};

class EatPunctuator : public ConsumeState {
public:
	virtual void eatChars() {
		token.clear();
		do {
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (!ispunct(currChar));	}
};

class EatAlphanum : public ConsumeState {
public:
	virtual void eatChars() {
		token.clear();
		do {
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (isalnum(currChar));
	}
};

class EatquotedStrings : public ConsumeState {
public:
	virtual void eatChars() {
		token.clear();
		do {
			if (!_pIn->good())  // end of stream
				return;
			if (currChar == '\\'&& _pIn->peek() == '"') {
				prevChar = currChar;
				currChar = _pIn->get();
				token += currChar;	}
			if (currChar != '"')
				token += currChar;
			currChar = _pIn->get();
		} while (currChar != '"');
		currChar = _pIn->get();
	}
};

class EatsinglequotedStrings : public ConsumeState {
public:
	virtual void eatChars() {
		token.clear();
		do {
			if (!_pIn->good())  // end of stream
				return;
			if (currChar == '\\'&& _pIn->peek() == '\'') {
				prevChar = currChar;
				currChar = _pIn->get();
				token += currChar;	}
			if (currChar != '\'')
				token += currChar;
			currChar = _pIn->get();
		} while (currChar != '\'');
		currChar = _pIn->get();
	}
};

class EatSpecialCharacterPair : public ConsumeState {
public:
	virtual void eatChars() {
		token.clear();
		token += currChar;
		if (!_pIn->good())	return;
		currChar = _pIn->get();
		token += currChar;
		if (!_pIn->good())	return;
		currChar = _pIn->get();
	}
};

class EatSpecialSingleCharacter : public ConsumeState {
public:
	virtual void eatChars() {
		token.clear();
		token += currChar;
		if (!_pIn->good())
			return;
		currChar = _pIn->get();	}
};

class EatNewline : public ConsumeState{
public:
	virtual void eatChars()	{
		token.clear();
		//std::cout << "\n  eating alphanum";
		token += currChar;
		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();	}
};

ConsumeState::ConsumeState()
{
	if (first_ctor)
	{
		first_ctor = false;
		_pEatAlphanum = new EatAlphanum();
		_pEatCComment = new EatCComment();
		_pEatCppComment = new EatCppComment();
		_pEatPunctuator = new EatPunctuator();
		_pEatWhitespace = new EatWhitespace();
		_pEatNewline = new EatNewline();
		_pEatquotedStrings = new EatquotedStrings();
		_pEatsinglequotedStrings = new EatsinglequotedStrings();
		_pEatSpecialCharacterPair = new EatSpecialCharacterPair();
		_pEatSpecialSingleCharacter = new EatSpecialSingleCharacter();
		_pState = _pEatWhitespace;
	}
}

ConsumeState::~ConsumeState()
{
	if (first_dtor)
	{
		first_dtor = false;
		delete _pEatAlphanum;
		delete _pEatCComment;
		delete _pEatCppComment;
		delete _pEatPunctuator;
		delete _pEatWhitespace;
		delete _pEatNewline;
		delete _pEatquotedStrings;
		delete _pEatsinglequotedStrings;
		delete _pEatSpecialCharacterPair;
		delete _pEatSpecialSingleCharacter;
	}
}

Toker::Toker() : pConsumer(new EatWhitespace()) {}

Toker::~Toker() { delete pConsumer; }

// setting new char to vector
void  Toker::setvector()
{
	pConsumer->setSpecialSingleChars("^");
}

// setting new pairchar to vector
void  Toker::setpairvector()
{
	pConsumer->setSpecialCharPairs("^^");
}

void Scanner::Toker::Reset()
{
	ConsumeState::reset();
}

bool Toker::attach(std::istream* pIn)
{
	if (pIn != nullptr && pIn->good())
	{
		pConsumer->attach(pIn);
		return true;
	}
	return false;
}

std::string Toker::getTok(){
	while (true)	{
		if (!pConsumer->canRead())
			return "";
		pConsumer->consumeChars();
		if (pConsumer->hasTok())
			break;	}
	return pConsumer->getTok();
}

bool Toker::canRead() { return pConsumer->canRead(); }

void testLog(const std::string& msg)
{
#ifdef TEST_LOG
	std::cout << "\n  " << msg;
#endif
}

//----< test stub >--------------------------------------------------

#ifdef TEST_TOKENIZER

#include <fstream>

int main()
{
	//std::string fileSpec = "../Tokenizer/Tokenizer.cpp";
	//std::string fileSpec = "../Tokenizer/Tokenizer.h";
	std::string fileSpec = "../Tokenizer/Test.txt";

	std::ifstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  can't open " << fileSpec << "\n\n";
		return 1;
	}
	Toker toker;
	toker.attach(&in);
	do
	{
		std::string tok = toker.getTok();
		if (tok == "\n")
			tok = "newline";
		std::cout << "\n -- " << tok;
	} while (in.good());

	std::cout << "\n\n";

	return 0;
}
#endif
