#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

/*******************************************************************************
* Assignment: Project 3 - Code Generator for Scheme to C++ Translator          *
* Author: Dr. Watts                                                            *
* Modified by:                                                                 *
* Date: Fall 2023                                                              *
* File: CodeGenerator.h                                                        *
*                                                                              *
* Description: This file contains the description of the CodeGenerator         *
*******************************************************************************/

#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.h"

using namespace std;

/*******************************************************************************
* Class: CodeGenerator                                                         *
*                                                                              *
* Description: This class is designed to generator C++ code for an input PL460 *
*              program.                                                        *
*              The functions in this class will be called by the Syntactic     *
*              analyzer.                                                       *
*******************************************************************************/

class CodeGenerator 
{
    public:
	CodeGenerator (string fileNamePrefix, LexicalAnalyzer * L);
	~CodeGenerator ();
	void WriteCode (int tabs, string code);
    private:
	LexicalAnalyzer * lex;
	ofstream cpp;
};
	
#endif
