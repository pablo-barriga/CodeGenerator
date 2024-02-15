#pragma once

#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.h"
#include "CodeGenerator.h"

using namespace std;

class SyntacticalAnalyzer
{
public:
    SyntacticalAnalyzer (const string & fileNamePrefix);
    ~SyntacticalAnalyzer ();
private:
    LexicalAnalyzer * lex;
    ofstream ruleFile;	// .p2
    token_type token;
    void program ();
    CodeGenerator *cg;
    void more_defines();
    void define();
    void stmt_list(int count = 0, string seperator = "",bool printRet = true );
    void stmt(int count = 0, string seperator = "",bool printRet = true);
    void literal();
    void quoted_lit();
    void logical_lit();
    void more_tokens();
    void param_list(int count = 0,string seperator = "");
    void else_part(bool printRet = true);
    void stmt_pair(int count = 0);
    void stmt_pair_body(int count = 0);
    void assign_pair();
    void more_assigns();
    void action(int count = 0, string seperator = "",bool printRet = true);
    void any_other_token();
    unsigned tabCounter = 0;
    unsigned stmtCounter = 0;
    unsigned arithmeticCounter = 0;
    

};
