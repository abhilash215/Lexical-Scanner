///////////////////////////////////////////////////////////////////////
// TestExecutive.cpp - Test                                          //
// ver 3.2                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Author:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////

#include<iostream>
#include<fstream>
#include<string>
#include "../SemiExp/SemiExp.h"

using namespace Scanner;
class TestExec
{
public:

	void TestR2()
	{
		std::cout << "\n";
		std::cout << "Demonstrating Requirement 2\n";
		std::cout << "------------------------------------------------------------------------------------------\n";
		std::cout << "For this project Visual studio 2015 and \n C++ standard library's streams for all I/O are used.\n";
		std::cout << "\n";
	}

	void TestR3()
	{
		std::cout << "\n";
		std::cout << "Demonstrating Requirement 3\n";
		std::cout << "------------------------------------------------------------------------------------------\n";
		std::cout << " C++ packages for Tokenizing, collecting SemiExpressions, and a scanner interface, ITokCollection\n are provided.\n";
		std::cout << "\n";

	}

	void DisplayInputStream()
	{
		std::string line;
		std::cout << "----------Input data for Requirement 4 -------" << "\n\n";
		std::ifstream inputfile("../Tokenizer/Test.txt");
		if (inputfile.is_open()) {
			while (getline(inputfile, line)) {
				std::cout << line << "\n";
			}
			inputfile.close();
		}
		else
			std::cout << "Unable to open file";
		std::cout << "\n----------Input data for Requirement 4 -------" << "\n\n";
	}

	void TestR4()
	{
		std::cout << "\n";
		std::cout << "Demonstrating Requirement 4\n";
		std::cout << "------------------------------------------------------------------------------------------\n";
		std::cout << "A Tokenizer Package is provided that displays different tokens:\n";
		std::cout << " alphanumeric token\n punctuator tokens \nspecial one and two chathank youracteyr tokens \n c++ comments \n c comments \n quoted strings\n ";
		std::cout << "The different tokens are displayed below:\n\n";
		std::string fileSpec = "../Tokenizer/Test.txt";
		std::ifstream in(fileSpec);
		if (!in.good())
		{
			std::cout << "\n  can't open " << fileSpec << "\n\n";
		}
		Toker toker;
		toker.attach(&in);
		toker.Reset();
		toker.setvector();
		std::cout << "\n";
		toker.setpairvector();
		std::cout << "\n";
		do
		{
			std::string tok = toker.getTok();
			if (tok == "\n")
				tok = "newline";
			std::cout << "\n -- " << tok;
		} while (in.good());

		std::cout << "\n\n";
		std::cout << "The different tokens are diplayed above";
		std::cout << "\n\n";
	}


	void TestR5()
	{
		std::cout << "\n\n";
		std::cout << "Demonstrating Requirement 5\n";
		std::cout << "------------------------------------------------------------------------------------------\n";
		std::cout << " The Toker class in the Tokenizer Package returns one token for each call to a member function getTok().\n\n";
	}
	void DisplayInputStreamSemi()
	{
		std::string line;
		std::cout << "----------Input data for Requirement 6,7,8 -------\n\n";
		std::ifstream inputfile("../SemiExp/semitest.txt");
		if (inputfile.is_open()) {
			while (getline(inputfile, line)) {
				std::cout << line << "\n";
			}
			inputfile.close();
		}
		else
			std::cout << "Unable to open file";
		std::cout << "\n----------Input data for Requirement 6,7,8 -------\n\n";
	}

	void TestR678()
	{
		std::cout << "\n";
		std::cout << "Demonstrating Requirement 6, 7, 8\n\n";
		std::cout << "------------------------------------------------------------------------------------------\n";
	 	Toker toker;
		toker.Reset();
		std::string fileSpec = "../SemiExp/semitest.txt";
		std::fstream in(fileSpec);

		if (!in.good())	std::cout << "\n  can't open file " << fileSpec << "\n\n";
		
		toker.attach(&in);

		SemiExp semi(&toker); 
		
		while (semi.get())
		{
			std::cout << "\n  -- semiExpression --";
			semi.show(true);
		}
		if (semi.length() > 0)
		{
			std::cout << "\n  -- semiExpression --";
			semi.show(true);
			std::cout << "\n\n";
		}
	}

	void TestR9()	{
		std::cout << "\n";
		std::cout << "Demonstrating Requirement 9\n";
		std::cout << "------------------------------------------------------------------------------------------\n\n\n";
		Toker toker;
		std::string fileSpec = "../SemiExp/semitest.txt";
		std::fstream in(fileSpec);
		if (!in.good())	{
			std::cout << "\n  can't open file " << fileSpec << "\n\n";}
		toker.attach(&in);
		SemiExp semi(&toker);
		std::cout << "Demonstrating bool get(bool clear = true) function\n";
		semi.get(true);
		semi.show();
		std::cout << "Demonstrating length() function\n";
		semi.length();
		std::cout << semi.length()<<"\n";
		std::cout << "Demonstrating clear() function\n"; 
		std::cout << "before";
		semi.show();
		std::cout << "after";
		semi.clear();
		semi.show();
		std::cout << "Demonstrating toLower() function\n";
		semi.get(true);
		semi.show();
		semi.toLower();
		semi.show();



	}
};



void main() 
{
	TestExec test;
	test.TestR2();
	test.TestR3();
	test.DisplayInputStream();
	test.TestR4();   
	test.TestR5();
	test.DisplayInputStreamSemi();
	test.TestR678();
	test.TestR9();

	
}

