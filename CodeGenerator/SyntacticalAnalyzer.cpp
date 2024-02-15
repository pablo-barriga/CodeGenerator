/*******************************************************************************
* Assignment: Project 2 - Syntactical Analyzer for Scheme to C++ Translator    *
* Author: Dr. Watts                                                            *
* Modified by:                                                                 *
* Date: Fall 2023                                                              *
* File: SyntaticalAnalyzer.h                                                   *
*                                                                              *
* Description: This file contains the implentation of the SyntaticalAnalyzer   *
*******************************************************************************/

#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <set>
#include "CodeGenerator.h"
#include "SyntacticalAnalyzer.h"

using namespace std;

static string token_lexemes[] = {
        "{}", "identifier", "numeric literal", "list operation", "+", "-",
        ">", "<", "#t", "#f", "/", "*", "=", ">=", "<=", "(", ")", "'",
        "identifier", "if", "cond", "display", "newline", "and", "or", "not",
        "define", "let", "cons or append", "number?", "list?", "zero?",
        "null?", "eof?", "modulo", "round", "read", "else", "string literal",
        "error", "end of file", "end of file",
};

SyntacticalAnalyzer::SyntacticalAnalyzer (const string & fileNamePrefix)
{
    ruleFile.open (fileNamePrefix + ".p2");
    lex = new LexicalAnalyzer (fileNamePrefix);
    cg = new CodeGenerator (fileNamePrefix, lex); // Added for Project 3
    token = lex->GetToken ();
    program ();
}

SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
    ruleFile.close ();
    delete cg; // Added for Project 3
    delete lex;
}

void SyntacticalAnalyzer::program ()
{
    int errors = 0;
    set <int> firsts {LPAREN_T, EOF_T};
    set <int> follows {EOF_T};

    char message [200];
    sprintf (message, "Entering Program function; current token is: %s, lexeme: %s", lex->GetTokenName(token).c_str(), lex->GetLexeme().c_str());
    lex->debugFile << message << endl;

    if (firsts.find (token) == firsts.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (firsts.find (token) == firsts.end())
            token = lex->GetToken ();
    }
    if (token == LPAREN_T)
    { // Rule 1
        lex->debugFile << "Using Rule 1\n";
        ruleFile << "Using Rule 1\n";
        token = lex->GetToken ();
        define ();
        if (token == LPAREN_T)
        {
            token = lex->GetToken ();
        }
        else
        {
            errors++;
            sprintf (message, "'%s' expected ", token_lexemes[LPAREN_T].c_str());
            lex->ReportError (message);
        }
        more_defines ();
        if (token == EOF_T)
        {
            token = lex->GetToken ();
        }
        else
        {
            errors++;
            sprintf (message, "'%s' expected ", token_lexemes[EOF_T].c_str());
            lex->ReportError (message);
        }
    }
    else
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
    }
    if (follows.find (token) == follows.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (follows.find (token) == follows.end())
            token = lex->GetToken ();
    }

    sprintf (message, "Exiting Program function; current token is: %s", lex->GetTokenName(token).c_str());
    lex->debugFile << message << endl;
    return;
}

void SyntacticalAnalyzer::more_defines ()
{
    int errors = 0;
    set <int> firsts {DEFINE_T, IDENT_T, EOF_T};
    set <int> follows {EOF_T, EOF_T};

    char message [200];
    sprintf (message, "Entering More_Defines function; current token is: %s, lexeme: %s", lex->GetTokenName(token).c_str(), lex->GetLexeme().c_str());
    lex->debugFile << message << endl;

    if (firsts.find (token) == firsts.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (firsts.find (token) == firsts.end())
            token = lex->GetToken ();
    }
    if (token == DEFINE_T)
    { // Rule 2
        lex->debugFile << "Using Rule 2\n";
        ruleFile << "Using Rule 2\n";
        define ();
        if (token == LPAREN_T)
        {
            token = lex->GetToken ();
        }
        else
        {
            errors++;
            sprintf (message, "'%s' expected ", token_lexemes[LPAREN_T].c_str());
            lex->ReportError (message);
        }
        more_defines ();
    }
    else if (token == IDENT_T)
    { // Rule 3
        lex->debugFile << "Using Rule 3\n";
        ruleFile << "Using Rule 3\n";
        token = lex->GetToken ();
        stmt_list ();
        
        if (token == RPAREN_T)
        {
            token = lex->GetToken ();
        }
        else
        {
            errors++;
            sprintf (message, "'%s' expected ", token_lexemes[RPAREN_T].c_str());
            lex->ReportError (message);
        }
    }
    else
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
    }
    if (follows.find (token) == follows.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (follows.find (token) == follows.end())
            token = lex->GetToken ();
    }

    sprintf (message, "Exiting More_Defines function; current token is: %s", lex->GetTokenName(token).c_str());
    lex->debugFile << message << endl;
    return;
}

void SyntacticalAnalyzer::define ()
{
    int errors = 0;
    set <int> firsts {DEFINE_T, EOF_T};
    set <int> follows {LPAREN_T, EOF_T};
    string ident_holder = "";
    char message [200];
    sprintf (message, "Entering Define function; current token is: %s, lexeme: %s", lex->GetTokenName(token).c_str(), lex->GetLexeme().c_str());
    lex->debugFile << message << endl;

    if (firsts.find (token) == firsts.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (firsts.find (token) == firsts.end())
            token = lex->GetToken ();
    }
    if (token == DEFINE_T)
    { // Rule 4
        lex->debugFile << "Using Rule 4\n";
        ruleFile << "Using Rule 4\n";
        token = lex->GetToken ();
        if (token == LPAREN_T)
        {
            token = lex->GetToken ();
        }
        else
        {
            errors++;
            sprintf (message, "'%s' expected ", token_lexemes[LPAREN_T].c_str());
            lex->ReportError (message);
        }
        if (token == IDENT_T)
        {
            if(lex->GetLexeme() == "main"){
                ident_holder = lex->GetLexeme();
                cg->WriteCode(0,"int main (" );

            }else{

                cg->WriteCode(0, "Object " + lex->GetLexeme() +" (");
            }
            token = lex->GetToken ();
        }
        else
        {
            errors++;
            sprintf (message, "'%s' expected ", token_lexemes[IDENT_T].c_str());
            lex->ReportError (message);
        }

        param_list ();
        if (token == RPAREN_T)
        {
            cg->WriteCode(0, ")\n");
            token = lex->GetToken ();

        }
        else
        {
            errors++;
            sprintf (message, "'%s' expected ", token_lexemes[RPAREN_T].c_str());
            lex->ReportError (message);
        }
        cg->WriteCode(0, "{\n");
        
        cg->WriteCode(0, "Object _RetVal;\n");
        
        
        stmt (0, "", true);
        stmt_list ();
        
        
        
        if (token == RPAREN_T)
        {

            token = lex->GetToken ();

            if(ident_holder == "main"){
                cg->WriteCode(0, "return 0;\n");
                cg->WriteCode(0, "}\n");
            }else{
                cg->WriteCode(0, "return _RetVal;\n");
                cg->WriteCode(0, "}\n");
            }
            

        }
        else
        {
            errors++;
            sprintf (message, "'%s' expected ", token_lexemes[RPAREN_T].c_str());
            lex->ReportError (message);
        }
    }
    else
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
    }
    if (follows.find (token) == follows.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (follows.find (token) == follows.end())
            token = lex->GetToken ();
    }

    sprintf (message, "Exiting Define function; current token is: %s", lex->GetTokenName(token).c_str());
    lex->debugFile << message << endl;
    return;
}

void SyntacticalAnalyzer::stmt_list (int count, string seperator, bool printRet)
{
    int errors = 0;
    set <int> firsts {NUMLIT_T, TRUE_T, FALSE_T, LPAREN_T, SQUOTE_T, IDENT_T,
                      STRLIT_T, RPAREN_T, EOF_T};
    set <int> follows {RPAREN_T, EOF_T};

    char message [200];
    sprintf (message, "Entering Stmt_List function; current token is: %s, lexeme: %s", lex->GetTokenName(token).c_str(), lex->GetLexeme().c_str());
    lex->debugFile << message << endl;

    if (firsts.find (token) == firsts.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (firsts.find (token) == firsts.end())
            token = lex->GetToken ();
    }
    if (token == NUMLIT_T || token == TRUE_T || token == FALSE_T || token == LPAREN_T
        || token == SQUOTE_T || token == IDENT_T || token == STRLIT_T)
    { // Rule 5
       
        lex->debugFile << "Using Rule 5\n";
        ruleFile << "Using Rule 5\n";
        if(count > 0){
            
             cg->WriteCode(0, seperator);
        } 

        stmt (count,seperator,printRet);
        stmt_list (count + 1,seperator,printRet);
     

       
    }
    else if (token == RPAREN_T)
    { 
       
       
        lex->debugFile << "Using Rule 6\n";
        ruleFile << "Using Rule 6\n";
   
    }
    else
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
    }
    if (follows.find (token) == follows.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (follows.find (token) == follows.end())
            token = lex->GetToken ();
    }

    sprintf (message, "Exiting Stmt_List function; current token is: %s", lex->GetTokenName(token).c_str());
    lex->debugFile << message << endl;
    return;
}

void SyntacticalAnalyzer::stmt (int count, string seperator, bool printRet)
{
    int errors = 0;
    stmtCounter ++;
    set <int> firsts {NUMLIT_T, TRUE_T, FALSE_T, SQUOTE_T, STRLIT_T, IDENT_T,
                      LPAREN_T, EOF_T};
    set <int> follows {NUMLIT_T, TRUE_T, FALSE_T, LPAREN_T, RPAREN_T, SQUOTE_T,
                       IDENT_T, STRLIT_T, EOF_T};

    char message [200];
    sprintf (message, "Entering Stmt function; current token is: %s, lexeme: %s", lex->GetTokenName(token).c_str(), lex->GetLexeme().c_str());
    lex->debugFile << message << endl;

    if (firsts.find (token) == firsts.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (firsts.find (token) == firsts.end())
            token = lex->GetToken ();
    }
    if (token == NUMLIT_T || token == TRUE_T || token == FALSE_T || token == SQUOTE_T
        || token == STRLIT_T)
    { // Rule 7
        lex->debugFile << "Using Rule 7\n";
        ruleFile << "Using Rule 7\n";
        
        literal ();
    }
    else if (token == IDENT_T)
    { // Rule 8
        lex->debugFile << "Using Rule 8\n";
        ruleFile << "Using Rule 8\n";
        cg->WriteCode(0, lex->GetLexeme());
        token = lex->GetToken ();
    }
    else if (token == LPAREN_T)
    { // Rule 9
        lex->debugFile << "Using Rule 9\n";
        ruleFile << "Using Rule 9\n";
        token = lex->GetToken ();
        if(printRet && token != DISPLAY_T && token != NEWLINE_T 
            && token != IF_T && token != LET_T && token != COND_T){
                cg->WriteCode(0, "_RetVal = ");
        }
        action (count,seperator);
        if (token == RPAREN_T)
        {
            cg->WriteCode(0, "");
            token = lex->GetToken ();
        }
        else
        {
            errors++;
            sprintf (message, "'%s' expected ", token_lexemes[RPAREN_T].c_str());
            lex->ReportError (message);
        }

    }
    else
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
    }
    if (follows.find (token) == follows.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (follows.find (token) == follows.end())
            token = lex->GetToken ();
    }
    stmtCounter --;
    if(stmtCounter == 0){
        cg->WriteCode(0, ";\n");
    }
    
    sprintf (message, "Exiting Stmt function; current token is: %s", lex->GetTokenName(token).c_str());
    lex->debugFile << message << endl;
    return;
}

void SyntacticalAnalyzer::literal ()
{
    int errors = 0;
    set <int> firsts {NUMLIT_T, STRLIT_T, SQUOTE_T, TRUE_T, FALSE_T, EOF_T};
    set <int> follows {NUMLIT_T, TRUE_T, FALSE_T, LPAREN_T, RPAREN_T, SQUOTE_T,
                       IDENT_T, STRLIT_T, EOF_T};

    char message [200];
    sprintf (message, "Entering Literal function; current token is: %s, lexeme: %s", lex->GetTokenName(token).c_str(), lex->GetLexeme().c_str());
    lex->debugFile << message << endl;

    if (firsts.find (token) == firsts.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (firsts.find (token) == firsts.end())
            token = lex->GetToken ();
    }
    if (token == NUMLIT_T)
    { // Rule 10
        lex->debugFile << "Using Rule 10\n";
        ruleFile << "Using Rule 10\n";
        cg->WriteCode(0, "Object(\""+ lex->GetLexeme()+"\")");
        token = lex->GetToken ();
        
    }
    else if (token == STRLIT_T)
    { // Rule 11
        lex->debugFile << "Using Rule 11\n";
        ruleFile << "Using Rule 11\n";
        cg->WriteCode(0, "Object(" +lex->GetLexeme()+ ")");
        token = lex->GetToken ();
    }
    else if (token == SQUOTE_T)
    { // Rule 12
        lex->debugFile << "Using Rule 12\n";
        ruleFile << "Using Rule 12\n";
        cg->WriteCode(0, "Object(\"");
        token = lex->GetToken ();
        quoted_lit ();
        cg->WriteCode(0, "\")");
    }
    else if (token == TRUE_T || token == FALSE_T)
    { // Rule 13
        lex->debugFile << "Using Rule 13\n";
        ruleFile << "Using Rule 13\n";
        logical_lit ();
    }
    else
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
    }
    if (follows.find (token) == follows.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (follows.find (token) == follows.end())
            token = lex->GetToken ();
    }

    sprintf (message, "Exiting Literal function; current token is: %s", lex->GetTokenName(token).c_str());
    lex->debugFile << message << endl;
    return;
}

void SyntacticalAnalyzer::quoted_lit ()
{
    int errors = 0;
    set <int> firsts {NUMLIT_T, LISTOP1_T, PLUS_T, MINUS_T, GT_T, LT_T,
                      TRUE_T, FALSE_T, DIV_T, MULT_T, EQUALTO_T, GTE_T,
                      LTE_T, LPAREN_T, SQUOTE_T, IDENT_T, IF_T, COND_T,
                      DISPLAY_T, NEWLINE_T, AND_T, OR_T, NOT_T, DEFINE_T,
                      LET_T, LISTOP2_T, NUMBERP_T, LISTP_T, ZEROP_T, NULLP_T,
                      EOFP_T, MODULO_T, ROUND_T, READ_T, ELSE_T, STRLIT_T,
                      EOF_T};
    set <int> follows {NUMLIT_T, TRUE_T, FALSE_T, LPAREN_T, RPAREN_T, SQUOTE_T,
                       IDENT_T, STRLIT_T, EOF_T};

    char message [200];
    sprintf (message, "Entering Quoted_Lit function; current token is: %s, lexeme: %s", lex->GetTokenName(token).c_str(), lex->GetLexeme().c_str());
    lex->debugFile << message << endl;

    if (firsts.find (token) == firsts.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (firsts.find (token) == firsts.end())
            token = lex->GetToken ();
    }
    if (token == NUMLIT_T || token == LISTOP1_T || token == PLUS_T || token == MINUS_T
        || token == GT_T || token == LT_T || token == TRUE_T || token == FALSE_T
        || token == DIV_T || token == MULT_T || token == EQUALTO_T || token == GTE_T
        || token == LTE_T || token == LPAREN_T || token == SQUOTE_T || token == IDENT_T
        || token == IF_T || token == COND_T || token == DISPLAY_T || token == NEWLINE_T
        || token == AND_T || token == OR_T || token == NOT_T || token == DEFINE_T
        || token == LET_T || token == LISTOP2_T || token == NUMBERP_T || token == LISTP_T
        || token == ZEROP_T || token == NULLP_T || token == EOFP_T || token == MODULO_T
        || token == ROUND_T || token == READ_T || token == ELSE_T || token == STRLIT_T)
    { // Rule 14
        lex->debugFile << "Using Rule 14\n";
        ruleFile << "Using Rule 14\n";
        any_other_token ();
    }
    else
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
    }
    if (follows.find (token) == follows.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (follows.find (token) == follows.end())
            token = lex->GetToken ();
    }

    sprintf (message, "Exiting Quoted_Lit function; current token is: %s", lex->GetTokenName(token).c_str());
    lex->debugFile << message << endl;
    return;
}

void SyntacticalAnalyzer::logical_lit ()
{
    int errors = 0;
    set <int> firsts {TRUE_T, FALSE_T, EOF_T};
    set <int> follows {NUMLIT_T, TRUE_T, FALSE_T, LPAREN_T, RPAREN_T, SQUOTE_T,
                       IDENT_T, STRLIT_T, EOF_T};

    char message [200];
    sprintf (message, "Entering Logical_Lit function; current token is: %s, lexeme: %s", lex->GetTokenName(token).c_str(), lex->GetLexeme().c_str());
    lex->debugFile << message << endl;

    if (firsts.find (token) == firsts.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (firsts.find (token) == firsts.end())
            token = lex->GetToken ();
    }
    if (token == TRUE_T)
    { // Rule 15
        lex->debugFile << "Using Rule 15\n";
        ruleFile << "Using Rule 15\n";
        cg->WriteCode(0, "Object(\"#t\")");
        token = lex->GetToken ();
    }
    else if (token == FALSE_T)
    { // Rule 16
        lex->debugFile << "Using Rule 16\n";
        ruleFile << "Using Rule 16\n";
        cg->WriteCode(0, "Object(\"#f\")");
        token = lex->GetToken ();
    }
    else
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
    }
    if (follows.find (token) == follows.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (follows.find (token) == follows.end())
            token = lex->GetToken ();
    }

    sprintf (message, "Exiting Logical_Lit function; current token is: %s", lex->GetTokenName(token).c_str());
    lex->debugFile << message << endl;
    return;
}

void SyntacticalAnalyzer::more_tokens ()
{
    int errors = 0;
    set <int> firsts {NUMLIT_T, LISTOP1_T, PLUS_T, MINUS_T, GT_T, LT_T,
                      TRUE_T, FALSE_T, DIV_T, MULT_T, EQUALTO_T, GTE_T,
                      LTE_T, LPAREN_T, SQUOTE_T, IDENT_T, IF_T, COND_T,
                      DISPLAY_T, NEWLINE_T, AND_T, OR_T, NOT_T, DEFINE_T,
                      LET_T, LISTOP2_T, NUMBERP_T, LISTP_T, ZEROP_T, NULLP_T,
                      EOFP_T, MODULO_T, ROUND_T, READ_T, ELSE_T, STRLIT_T,
                      RPAREN_T, EOF_T};
    set <int> follows {RPAREN_T, EOF_T};

    char message [200];
    sprintf (message, "Entering More_Tokens function; current token is: %s, lexeme: %s", lex->GetTokenName(token).c_str(), lex->GetLexeme().c_str());
    lex->debugFile << message << endl;

    if (firsts.find (token) == firsts.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (firsts.find (token) == firsts.end())
            token = lex->GetToken ();
    }
    if (token == NUMLIT_T || token == LISTOP1_T || token == PLUS_T || token == MINUS_T
        || token == GT_T || token == LT_T || token == TRUE_T || token == FALSE_T
        || token == DIV_T || token == MULT_T || token == EQUALTO_T || token == GTE_T
        || token == LTE_T || token == LPAREN_T || token == SQUOTE_T || token == IDENT_T
        || token == IF_T || token == COND_T || token == DISPLAY_T || token == NEWLINE_T
        || token == AND_T || token == OR_T || token == NOT_T || token == DEFINE_T
        || token == LET_T || token == LISTOP2_T || token == NUMBERP_T || token == LISTP_T
        || token == ZEROP_T || token == NULLP_T || token == EOFP_T || token == MODULO_T
        || token == ROUND_T || token == READ_T || token == ELSE_T || token == STRLIT_T)
    { // Rule 17
        lex->debugFile << "Using Rule 17\n";
        ruleFile << "Using Rule 17\n";
        any_other_token ();
        more_tokens ();
    }
    else if (token == RPAREN_T)
    { // Rule 18
        lex->debugFile << "Using Rule 18\n";
        ruleFile << "Using Rule 18\n";
        
    }
    else
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
    }
    if (follows.find (token) == follows.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (follows.find (token) == follows.end())
            token = lex->GetToken ();
    }

    sprintf (message, "Exiting More_Tokens function; current token is: %s", lex->GetTokenName(token).c_str());
    lex->debugFile << message << endl;
    return;
}

void SyntacticalAnalyzer::param_list (int count, string seperator)
{
    int errors = 0;
    set <int> firsts {IDENT_T, RPAREN_T, EOF_T};
    set <int> follows {RPAREN_T, EOF_T};

    char message [200];
    sprintf (message, "Entering Param_List function; current token is: %s, lexeme: %s", lex->GetTokenName(token).c_str(), lex->GetLexeme().c_str());
    lex->debugFile << message << endl;

    if (firsts.find (token) == firsts.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (firsts.find (token) == firsts.end())
            token = lex->GetToken ();
    }
    if (token == IDENT_T)
    { // Rule 19
        if(count > 0){
           
            cg->WriteCode(0, ", ");
        }
        lex->debugFile << "Using Rule 19\n";
        ruleFile << "Using Rule 19\n";
        cg->WriteCode(0, "Object " + lex->GetLexeme());
    
        token = lex->GetToken ();
      
        param_list (count + 1);
     
    }
    else if (token == RPAREN_T)
    { // Rule 20
        lex->debugFile << "Using Rule 20\n";
        ruleFile << "Using Rule 20\n";

    }
    else
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
    }
    if (follows.find (token) == follows.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (follows.find (token) == follows.end())
            token = lex->GetToken ();
    }

    sprintf (message, "Exiting Param_List function; current token is: %s", lex->GetTokenName(token).c_str());
    lex->debugFile << message << endl;
    return;
}

void SyntacticalAnalyzer::else_part (bool printRet)
{
    int errors = 0;
    set <int> firsts {NUMLIT_T, TRUE_T, FALSE_T, LPAREN_T, SQUOTE_T, IDENT_T,
                      STRLIT_T, RPAREN_T, EOF_T};
    set <int> follows {RPAREN_T, EOF_T};

    char message [200];
    sprintf (message, "Entering Else_Part function; current token is: %s, lexeme: %s", lex->GetTokenName(token).c_str(), lex->GetLexeme().c_str());
    lex->debugFile << message << endl;

    if (firsts.find (token) == firsts.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (firsts.find (token) == firsts.end())
            token = lex->GetToken ();
    }
    if (token == NUMLIT_T || token == TRUE_T || token == FALSE_T || token == LPAREN_T
        || token == SQUOTE_T || token == IDENT_T || token == STRLIT_T)
    { // Rule 21
        lex->debugFile << "Using Rule 21\n";
        ruleFile << "Using Rule 21\n";
        cg->WriteCode(0, "else\n");
        cg->WriteCode(0, "{\n");
        if(token == STRLIT_T || token == NUMLIT_T){
            cg->WriteCode(0, "_RetVal = "); 
        }
        stmt (0, "",true);
        cg->WriteCode(0,";\n");
        cg->WriteCode(0, "}\n");
    }
    else if (token == RPAREN_T)
    { // Rule 22
        lex->debugFile << "Using Rule 22\n";
        ruleFile << "Using Rule 22\n";

    }
    else
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
    }
    if (follows.find (token) == follows.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (follows.find (token) == follows.end())
            token = lex->GetToken ();
    }

    sprintf (message, "Exiting Else_Part function; current token is: %s", lex->GetTokenName(token).c_str());
    lex->debugFile << message << endl;
    return;
}

void SyntacticalAnalyzer::stmt_pair (int count)
{
    int errors = 0;
    set <int> firsts {LPAREN_T, RPAREN_T, EOF_T};
    set <int> follows {RPAREN_T, EOF_T};

    char message [200];
    sprintf (message, "Entering Stmt_Pair function; current token is: %s, lexeme: %s", lex->GetTokenName(token).c_str(), lex->GetLexeme().c_str());
    lex->debugFile << message << endl;

    if (firsts.find (token) == firsts.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (firsts.find (token) == firsts.end())
            token = lex->GetToken ();
    }
    if (token == LPAREN_T)
    { // Rule 23
        lex->debugFile << "Using Rule 23\n";
        ruleFile << "Using Rule 23\n";
        token = lex->GetToken ();
        stmt_pair_body (count);
    }
    else if (token == RPAREN_T)
    { // Rule 24
        lex->debugFile << "Using Rule 24\n";
        ruleFile << "Using Rule 24\n";
        ;
    }
    else
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
    }
    if (follows.find (token) == follows.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (follows.find (token) == follows.end())
            token = lex->GetToken ();
    }

    sprintf (message, "Exiting Stmt_Pair function; current token is: %s", lex->GetTokenName(token).c_str());
    lex->debugFile << message << endl;
    return;
}

void SyntacticalAnalyzer::stmt_pair_body (int count)
{
    int errors = 0;
    set <int> firsts {NUMLIT_T, TRUE_T, FALSE_T, LPAREN_T, SQUOTE_T, IDENT_T,
                      STRLIT_T, ELSE_T, EOF_T};
    set <int> follows {RPAREN_T, EOF_T};

    char message [200];
    sprintf (message, "Entering Stmt_Pair_Body function; current token is: %s, lexeme: %s", lex->GetTokenName(token).c_str(), lex->GetLexeme().c_str());
    lex->debugFile << message << endl;

    if (firsts.find (token) == firsts.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (firsts.find (token) == firsts.end())
            token = lex->GetToken ();
    }
    if (token == NUMLIT_T || token == TRUE_T || token == FALSE_T || token == LPAREN_T
        || token == SQUOTE_T || token == IDENT_T || token == STRLIT_T)
    { // Rule 25
        lex->debugFile << "Using Rule 25\n";
        ruleFile << "Using Rule 25\n";
        if(count > 0){
            cg->WriteCode(0, "else if(");
        }else{
            cg->WriteCode(0, "if(");
        }
        stmt ();
        cg->WriteCode(0, ")\n");
        cg->WriteCode(0, "{\n");
        stmt ();
        if (token == RPAREN_T)
        {
            token = lex->GetToken ();
        }
        else
        {
            errors++;
            sprintf (message, "'%s' expected ", token_lexemes[RPAREN_T].c_str());
            lex->ReportError (message);
        }
        cg->WriteCode(0, "}\n");
        stmt_pair (count + 1);
    }
    else if (token == ELSE_T)
    { // Rule 26
        lex->debugFile << "Using Rule 26\n";
        ruleFile << "Using Rule 26\n";
        token = lex->GetToken ();
        cg->WriteCode(0,"else\n");
        cg->WriteCode(0,"{\n");
        stmt ();
        if (token == RPAREN_T)
        {
            cg->WriteCode(0,"}\n");
            token = lex->GetToken ();
        }
        else
        {
            errors++;
            sprintf (message, "'%s' expected ", token_lexemes[RPAREN_T].c_str());
            lex->ReportError (message);
        }
    }
    else
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
    }
    if (follows.find (token) == follows.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (follows.find (token) == follows.end())
            token = lex->GetToken ();
    }

    sprintf (message, "Exiting Stmt_Pair_Body function; current token is: %s", lex->GetTokenName(token).c_str());
    lex->debugFile << message << endl;
    return;
}

void SyntacticalAnalyzer::assign_pair ()
{
    int errors = 0;
    set <int> firsts {LPAREN_T, EOF_T};
    set <int> follows {LPAREN_T, RPAREN_T, EOF_T};

    char message [200];
    sprintf (message, "Entering Assign_Pair function; current token is: %s, lexeme: %s", lex->GetTokenName(token).c_str(), lex->GetLexeme().c_str());
    lex->debugFile << message << endl;

    if (firsts.find (token) == firsts.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (firsts.find (token) == firsts.end())
            token = lex->GetToken ();
    }
    if (token == LPAREN_T)
    { // Rule 27
        lex->debugFile << "Using Rule 27\n";
        ruleFile << "Using Rule 27\n";
         
        token = lex->GetToken ();
        
        if (token == IDENT_T)
        {
            cg->WriteCode(0, "Object " + lex->GetLexeme() + " = ");
    
            token = lex->GetToken ();
        }
        else
        {
            errors++;
            sprintf (message, "'%s' expected ", token_lexemes[IDENT_T].c_str());
            lex->ReportError (message);
        }
        
        
        stmt ();
        cg->WriteCode(0,";\n");
        
        if (token == RPAREN_T)
        {
            token = lex->GetToken ();
        }
        else
        {
            errors++;
            sprintf (message, "'%s' expected ", token_lexemes[RPAREN_T].c_str());
            lex->ReportError (message);
        }
    }
    else
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
    }
    if (follows.find (token) == follows.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (follows.find (token) == follows.end())
            token = lex->GetToken ();
    }

    sprintf (message, "Exiting Assign_Pair function; current token is: %s", lex->GetTokenName(token).c_str());
    lex->debugFile << message << endl;
    return;
}

void SyntacticalAnalyzer::more_assigns ()
{
    int errors = 0;
    set <int> firsts {LPAREN_T, RPAREN_T, EOF_T};
    set <int> follows {RPAREN_T, EOF_T};

    char message [200];
    sprintf (message, "Entering More_Assigns function; current token is: %s, lexeme: %s", lex->GetTokenName(token).c_str(), lex->GetLexeme().c_str());
    lex->debugFile << message << endl;

    if (firsts.find (token) == firsts.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (firsts.find (token) == firsts.end())
            token = lex->GetToken ();
    }
    if (token == LPAREN_T)
    { // Rule 28
        lex->debugFile << "Using Rule 28\n";
        ruleFile << "Using Rule 28\n";
        assign_pair ();
        more_assigns ();
    }
    else if (token == RPAREN_T)
    { // Rule 29
        lex->debugFile << "Using Rule 29\n";
        ruleFile << "Using Rule 29\n";

    }
    else
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
    }
    if (follows.find (token) == follows.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (follows.find (token) == follows.end())
            token = lex->GetToken ();
    }

    sprintf (message, "Exiting More_Assigns function; current token is: %s", lex->GetTokenName(token).c_str());
    lex->debugFile << message << endl;
    return;
}

void SyntacticalAnalyzer::action (int count, string seperator, bool printRet)
{
    int errors = 0;
    set <int> firsts {IF_T, COND_T, LET_T, LISTOP1_T, LISTOP2_T, AND_T,
                      OR_T, NOT_T, NUMBERP_T, LISTP_T, ZEROP_T, NULLP_T,
                      EOFP_T, PLUS_T, MINUS_T, DIV_T, MULT_T, MODULO_T,
                      ROUND_T, EQUALTO_T, GT_T, LT_T, GTE_T, LTE_T,
                      IDENT_T, DISPLAY_T, NEWLINE_T, READ_T, EOF_T};
    set <int> follows {RPAREN_T, EOF_T};

    char message [200];
    sprintf (message, "Entering Action function; current token is: %s, lexeme: %s", lex->GetTokenName(token).c_str(), lex->GetLexeme().c_str());
    lex->debugFile << message << endl;

    if (firsts.find (token) == firsts.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (firsts.find (token) == firsts.end())
            token = lex->GetToken ();
    }
    if (token == IF_T)
    { // Rule 30
        lex->debugFile << "Using Rule 30\n";
        ruleFile << "Using Rule 30\n";
        cg->WriteCode(0,"if(");
        token = lex->GetToken ();
        stmt (count = 0, seperator, false);
        cg->WriteCode(0,")\n");
        cg->WriteCode(0,"{\n");
        stmt ();
        cg->WriteCode(0,";\n");
        cg->WriteCode(0,"}\n");
        else_part ();
    }
    else if (token == COND_T)
    { // Rule 31
        lex->debugFile << "Using Rule 31\n";
        ruleFile << "Using Rule 31\n";
        token = lex->GetToken ();
        if (token == LPAREN_T)
        {
            token = lex->GetToken ();
        }
        else
        {
            errors++;
            sprintf (message, "'%s' expected ", token_lexemes[LPAREN_T].c_str());
            lex->ReportError (message);
        }
        stmt_pair_body (count = 0);
    }
    else if (token == LET_T)
    { // Rule 32
        lex->debugFile << "Using Rule 32\n";
        ruleFile << "Using Rule 32\n";
        cg->WriteCode(0, "{\n");
       
        token = lex->GetToken ();
        if (token == LPAREN_T)
        {
            token = lex->GetToken ();
        }
        else
        {
            errors++;
            sprintf (message, "'%s' expected ", token_lexemes[LPAREN_T].c_str());
            lex->ReportError (message);
        }

        more_assigns ();
        
        if (token == RPAREN_T)
        {
            token = lex->GetToken ();
        }
        else
        {
            errors++;
            sprintf (message, "'%s' expected ", token_lexemes[RPAREN_T].c_str());
            lex->ReportError (message);
        }
        stmt ();
        stmt_list ();
        cg->WriteCode(0, "}\n");
    }
    else if (token == LISTOP1_T)
    { // Rule 33
        lex->debugFile << "Using Rule 33\n";
        ruleFile << "Using Rule 33\n";
        cg->WriteCode(0, "listop (\"" + lex->GetLexeme() + "\", ");
        token = lex->GetToken ();
        stmt (count = 0, seperator, false);
        cg->WriteCode(0, ")");
    }
    else if (token == LISTOP2_T)
    { // Rule 34
        lex->debugFile << "Using Rule 34\n";
        ruleFile << "Using Rule 34\n";
        cg->WriteCode(0, "listop (\"" + lex->GetLexeme() + "\", ");
        token = lex->GetToken ();       
        stmt (count = 0, seperator, false);
         cg->WriteCode(0, ", ");
        stmt (count = 0, seperator, false);
         cg->WriteCode(0, ")");
    }
    else if (token == AND_T)
    { // Rule 35
        lex->debugFile << "Using Rule 35\n";
        ruleFile << "Using Rule 35\n";
        seperator = " && ";
        token = lex->GetToken ();
        stmt_list (count = 0, seperator);
        seperator = "";
    }
    else if (token == OR_T)
    { // Rule 36
        lex->debugFile << "Using Rule 36\n";
        ruleFile << "Using Rule 36\n";
        
        seperator = " || ";
        token = lex->GetToken ();
        stmt_list (count = 0, seperator);
        seperator = ""; 
    }
    else if (token == NOT_T)
    { // Rule 37
        lex->debugFile << "Using Rule 37\n";
        ruleFile << "Using Rule 37\n";
         cg->WriteCode(0, "!(" );
        token = lex->GetToken ();
        stmt (count = 0, seperator, false);
        cg->WriteCode(0, ")");
    }
    else if (token == NUMBERP_T)
    { // Rule 38
        lex->debugFile << "Using Rule 38\n";
        ruleFile << "Using Rule 38\n";
        cg->WriteCode(0, "numberp(" );
        token = lex->GetToken ();
        stmt (count = 0, seperator, false);
         cg->WriteCode(0, ")");
    }
    else if (token == LISTP_T)
    { // Rule 39
        lex->debugFile << "Using Rule 39\n";
        ruleFile << "Using Rule 39\n";
        cg->WriteCode(0, "listp(" );
        token = lex->GetToken ();
        stmt (count = 0, seperator,false);
         cg->WriteCode(0, ")");
    }
    else if (token == ZEROP_T)
    { // Rule 40
        lex->debugFile << "Using Rule 40\n";
        ruleFile << "Using Rule 40\n";
        cg->WriteCode(0, "zerop(" );
        token = lex->GetToken ();
        stmt (count = 0, seperator,false);
         cg->WriteCode(0, ")");
    }
    else if (token == NULLP_T)
    { // Rule 41
        lex->debugFile << "Using Rule 41\n";
        ruleFile << "Using Rule 41\n";
        cg->WriteCode(0, "nullp(" );
        token = lex->GetToken ();
        stmt ();
        cg->WriteCode(0, ")");
    }
    else if (token == EOFP_T)
    { // Rule 42
        lex->debugFile << "Using Rule 42\n";
        ruleFile << "Using Rule 42\n";
        cg->WriteCode(0, "(Object(\"#<eof>\") == ");
        token = lex->GetToken ();
        stmt ();
        cg->WriteCode(0, ")");
    }
    else if (token == PLUS_T)
    { // Rule 43
        lex->debugFile << "Using Rule 43\n";
        ruleFile << "Using Rule 43\n";
        token = lex->GetToken ();
        cg->WriteCode(0, "(");
        seperator = " + ";
        stmt_list (count = 0, seperator);
        cg->WriteCode(0, ")");
        seperator = "";
    }
    else if (token == MINUS_T)
    { // Rule 44
        lex->debugFile << "Using Rule 44\n";
        ruleFile << "Using Rule 44\n";
        cg->WriteCode(0, "(");
        seperator = " - ";
        token = lex->GetToken ();
        stmt (count = 0, seperator);
        cg->WriteCode(0, " - ");
        stmt_list (count = 0, seperator);
        cg->WriteCode(0, ")");
        seperator = "";
    }
    else if (token == DIV_T)
    { // Rule 45
        lex->debugFile << "Using Rule 45\n";
        ruleFile << "Using Rule 45\n";
        cg->WriteCode(0, "(");
        seperator = " / ";
        token = lex->GetToken ();
        stmt (count = 0, seperator);
        cg->WriteCode(0, " / ");
        stmt_list (count = 0, seperator);
        cg->WriteCode(0, ")");
        seperator = "";
    }
    else if (token == MULT_T)
    { // Rule 46
        lex->debugFile << "Using Rule 46\n";
        ruleFile << "Using Rule 46\n";
        cg->WriteCode(0, "(");
        seperator = " * ";
        token = lex->GetToken ();
        stmt_list (count = 0, seperator);
        cg->WriteCode(0, ")");
        seperator = "";
    }
    else if (token == MODULO_T)
    { // Rule 47
        lex->debugFile << "Using Rule 47\n";
        ruleFile << "Using Rule 47\n";
        cg->WriteCode(0, "(");
        token = lex->GetToken ();
        stmt ();
        cg->WriteCode(0, " % ");
        stmt ();
        cg->WriteCode(0, ")");
        seperator = "";
    }
    else if (token == ROUND_T)
    { // Rule 48
        lex->debugFile << "Using Rule 48\n";
        ruleFile << "Using Rule 48\n";
        cg->WriteCode(0, "round( ");
        token = lex->GetToken ();
        stmt ();
        cg->WriteCode(0, ") ");
    }
    else if (token == EQUALTO_T)
    { // Rule 49
        lex->debugFile << "Using Rule 49\n";
        ruleFile << "Using Rule 49\n";
        token = lex->GetToken ();
        cg->WriteCode(0, "(");
        seperator = " == ";
        stmt_list (count, seperator);
        cg->WriteCode(0, ")");
        seperator = "";
    }
    else if (token == GT_T)
    { // Rule 50
        lex->debugFile << "Using Rule 50\n";
        ruleFile << "Using Rule 50\n";
        token = lex->GetToken ();
        cg->WriteCode(0, "(");
        seperator = " > ";
        stmt_list (count = 0, seperator);
        cg->WriteCode(0, ")");
        seperator = "";
    }
    else if (token == LT_T)
    { // Rule 51
        lex->debugFile << "Using Rule 51\n";
        ruleFile << "Using Rule 51\n";
        token = lex->GetToken ();
        cg->WriteCode(0, "(");
        seperator = " < ";
        stmt_list (count = 0, seperator);
        cg->WriteCode(0, ")");
        seperator = "";
    }
    else if (token == GTE_T)
    { // Rule 52
        lex->debugFile << "Using Rule 52\n";
        ruleFile << "Using Rule 52\n";
        token = lex->GetToken ();
        cg->WriteCode(0, "(");
        seperator = " >= ";
        stmt_list (count = 0, seperator);
        cg->WriteCode(0, ")");
        seperator = "";
    }
    else if (token == LTE_T)
    { // Rule 53
        lex->debugFile << "Using Rule 53\n";
        ruleFile << "Using Rule 53\n";
        token = lex->GetToken ();
        cg->WriteCode(0, "(");
        seperator = " <= ";
        stmt_list (count = 0, seperator);
        cg->WriteCode(0, ")");
        seperator = "";
    }
    else if (token == IDENT_T)
    { // Rule 54
        lex->debugFile << "Using Rule 54\n";
        ruleFile << "Using Rule 54\n";
        cg->WriteCode(0,lex->GetLexeme() + "(");
        token = lex->GetToken ();
        seperator = ",";
        stmt_list (count = 0, seperator, false);
        seperator = "";
        cg->WriteCode(0,")");
    }
    else if (token == DISPLAY_T)
    { // Rule 55
        lex->debugFile << "Using Rule 55\n";
        ruleFile << "Using Rule 55\n";
        cg->WriteCode(0,"cout << ");
        token = lex->GetToken ();
        stmt (count = 0, seperator, false);
        
    }
    else if (token == NEWLINE_T)
    { // Rule 56
        lex->debugFile << "Using Rule 56\n";
        ruleFile << "Using Rule 56\n";
        cg->WriteCode(0,"cout << endl");
        token = lex->GetToken ();
    }
    else if (token == READ_T)
    { // Rule 57
        lex->debugFile << "Using Rule 57\n";
        ruleFile << "Using Rule 57\n";
        cg->WriteCode(0, "read(std::cin)");
        
        token = lex->GetToken ();
        
    }
    else
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
    }
    if (follows.find (token) == follows.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (follows.find (token) == follows.end())
            token = lex->GetToken ();
    }

    sprintf (message, "Exiting Action function; current token is: %s", lex->GetTokenName(token).c_str());
    lex->debugFile << message << endl;
    return;
}

void SyntacticalAnalyzer::any_other_token ()
{
    int errors = 0;
    set <int> firsts {LPAREN_T, IDENT_T, NUMLIT_T, STRLIT_T, IF_T, DISPLAY_T,
                      NEWLINE_T, READ_T, LISTOP1_T, LISTOP2_T, AND_T, OR_T,
                      NOT_T, DEFINE_T, LET_T, NUMBERP_T, LISTP_T, ZEROP_T,
                      NULLP_T, EOFP_T, PLUS_T, MINUS_T, DIV_T, MULT_T,
                      MODULO_T, ROUND_T, EQUALTO_T, GT_T, LT_T, GTE_T,
                      LTE_T, SQUOTE_T, COND_T, ELSE_T, TRUE_T, FALSE_T,
                      EOF_T};
    set <int> follows {NUMLIT_T, LISTOP1_T, PLUS_T, MINUS_T, GT_T, LT_T,
                       TRUE_T, FALSE_T, DIV_T, MULT_T, EQUALTO_T, GTE_T,
                       LTE_T, LPAREN_T, RPAREN_T, SQUOTE_T, IDENT_T, IF_T,
                       COND_T, DISPLAY_T, NEWLINE_T, AND_T, OR_T, NOT_T,
                       DEFINE_T, LET_T, LISTOP2_T, NUMBERP_T, LISTP_T, ZEROP_T,
                       NULLP_T, EOFP_T, MODULO_T, ROUND_T, READ_T, ELSE_T,
                       STRLIT_T, EOF_T};

    char message [200];
    sprintf (message, "Entering Any_Other_Token function; current token is: %s, lexeme: %s", lex->GetTokenName(token).c_str(), lex->GetLexeme().c_str());
    lex->debugFile << message << endl;

    if (firsts.find (token) == firsts.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (firsts.find (token) == firsts.end())
            token = lex->GetToken ();
    }
    
    if (token == LPAREN_T)
    { // Rule 58
        lex->debugFile << "Using Rule 58\n";
        ruleFile << "Using Rule 58\n";
        cg->WriteCode(0, "(");
        token = lex->GetToken ();
        more_tokens ();
        if (token == RPAREN_T) 
        {
            cg->WriteCode(0, ")");
            token = lex->GetToken ();
        }
        else
        {
            errors++;
            sprintf (message, "'%s' expected ", token_lexemes[RPAREN_T].c_str());
            lex->ReportError (message);
        }
    }
    else if (token == IDENT_T)
    { // Rule 59
        lex->debugFile << "Using Rule 59\n";
        ruleFile << "Using Rule 59\n";
        cg->WriteCode(0, "" + lex->GetLexeme() + " ");
        token = lex->GetToken ();
    }
    else if (token == NUMLIT_T)
    { // Rule 60
        lex->debugFile << "Using Rule 60\n";
        ruleFile << "Using Rule 60\n";
        cg->WriteCode(0, "" + lex->GetLexeme() + " ");
        token = lex->GetToken ();
    }
    else if (token == STRLIT_T)
    { // Rule 61
        lex->debugFile << "Using Rule 61\n";
        ruleFile << "Using Rule 61\n";
        string temp = lex->GetLexeme().substr(1, lex->GetLexeme().length() - 2);
        cg->WriteCode(0, "" + temp + " ");
        token = lex->GetToken ();
    }
    else if (token == IF_T)
    { // Rule 62
        lex->debugFile << "Using Rule 62\n";
        ruleFile << "Using Rule 62\n";
        cg->WriteCode(0, "" + lex->GetLexeme() + " ");
        token = lex->GetToken ();
    }
    else if (token == DISPLAY_T)
    { // Rule 63
        lex->debugFile << "Using Rule 63\n";
        ruleFile << "Using Rule 63\n";
        cg->WriteCode(0, "" + lex->GetLexeme() + " ");
        token = lex->GetToken ();
    }
    else if (token == NEWLINE_T)
    { // Rule 64
        lex->debugFile << "Using Rule 64\n";
        ruleFile << "Using Rule 64\n";
        cg->WriteCode(0, "" + lex->GetLexeme() + " ");
        token = lex->GetToken ();
    }
    else if (token == READ_T)
    { // Rule 65
        lex->debugFile << "Using Rule 65\n";
        ruleFile << "Using Rule 65\n";
        cg->WriteCode(0, "" + lex->GetLexeme() + " ");
        token = lex->GetToken ();
    }
    else if (token == LISTOP1_T)
    { // Rule 66
        lex->debugFile << "Using Rule 66\n";
        ruleFile << "Using Rule 66\n";
        cg->WriteCode(0, "" + lex->GetLexeme() + " ");
        token = lex->GetToken ();
    }
    else if (token == LISTOP2_T)
    { // Rule 67
        lex->debugFile << "Using Rule 67\n";
        ruleFile << "Using Rule 67\n";
        cg->WriteCode(0, "" + lex->GetLexeme() + " ");
        token = lex->GetToken ();
    }
    else if (token == AND_T)
    { // Rule 68
        lex->debugFile << "Using Rule 68\n";
        ruleFile << "Using Rule 68\n";
        cg->WriteCode(0, "" + lex->GetLexeme() + " ");
        token = lex->GetToken ();
    }
    else if (token == OR_T)
    { // Rule 69
        lex->debugFile << "Using Rule 69\n";
        ruleFile << "Using Rule 69\n";
        cg->WriteCode(0, "" + lex->GetLexeme() + " ");
        token = lex->GetToken ();
    }
    else if (token == NOT_T)
    { // Rule 70
        lex->debugFile << "Using Rule 70\n";
        ruleFile << "Using Rule 70\n";
        cg->WriteCode(0, "" + lex->GetLexeme() + " ");
        token = lex->GetToken ();
    }
    else if (token == DEFINE_T)
    { // Rule 71
        lex->debugFile << "Using Rule 71\n";
        ruleFile << "Using Rule 71\n";
        cg->WriteCode(0, "" + lex->GetLexeme() + " ");
        token = lex->GetToken ();
    }
    else if (token == LET_T)
    { // Rule 72
        lex->debugFile << "Using Rule 72\n";
        ruleFile << "Using Rule 72\n";
        cg->WriteCode(0, "" + lex->GetLexeme() + " ");
        token = lex->GetToken ();
    }
    else if (token == NUMBERP_T)
    { // Rule 73
        lex->debugFile << "Using Rule 73\n";
        ruleFile << "Using Rule 73\n";
        cg->WriteCode(0, "" + lex->GetLexeme() + " ");
        token = lex->GetToken ();
    }
    else if (token == LISTP_T)
    { // Rule 74
        lex->debugFile << "Using Rule 74\n";
        ruleFile << "Using Rule 74\n";
        cg->WriteCode(0, "" + lex->GetLexeme() + " ");
        token = lex->GetToken ();
    }
    else if (token == ZEROP_T)
    { // Rule 75
        lex->debugFile << "Using Rule 75\n";
        ruleFile << "Using Rule 75\n";
        cg->WriteCode(0, "" + lex->GetLexeme() + " ");
        token = lex->GetToken ();
    }
    else if (token == NULLP_T)
    { // Rule 76
        lex->debugFile << "Using Rule 76\n";
        ruleFile << "Using Rule 76\n";
        cg->WriteCode(0, "" + lex->GetLexeme() + " ");
        token = lex->GetToken ();
    }
    else if (token == EOFP_T)
    { // Rule 77
        lex->debugFile << "Using Rule 77\n";
        ruleFile << "Using Rule 77\n";
        cg->WriteCode(0, "" + lex->GetLexeme() + " ");
        token = lex->GetToken ();
    }
    else if (token == PLUS_T)
    { // Rule 78
        lex->debugFile << "Using Rule 78\n";
        ruleFile << "Using Rule 78\n";
        cg->WriteCode(0, "" + lex->GetLexeme() + " ");
        token = lex->GetToken ();
    }
    else if (token == MINUS_T)
    { // Rule 79
        lex->debugFile << "Using Rule 79\n";
        ruleFile << "Using Rule 79\n";
        cg->WriteCode(0, "" + lex->GetLexeme() + " ");
        token = lex->GetToken ();
    }
    else if (token == DIV_T)
    { // Rule 80
        lex->debugFile << "Using Rule 80\n";
        ruleFile << "Using Rule 80\n";
        cg->WriteCode(0, "" + lex->GetLexeme() + " ");
        token = lex->GetToken ();
    }
    else if (token == MULT_T)
    { // Rule 81
        lex->debugFile << "Using Rule 81\n";
        ruleFile << "Using Rule 81\n";
        cg->WriteCode(0, "" + lex->GetLexeme() + " ");
        token = lex->GetToken ();
    }
    else if (token == MODULO_T)
    { // Rule 82
        lex->debugFile << "Using Rule 82\n";
        ruleFile << "Using Rule 82\n";
        cg->WriteCode(0, "" + lex->GetLexeme() + " ");
        token = lex->GetToken ();
    }
    else if (token == ROUND_T)
    { // Rule 83
        lex->debugFile << "Using Rule 83\n";
        ruleFile << "Using Rule 83\n";
        cg->WriteCode(0, "" + lex->GetLexeme() + " ");
        token = lex->GetToken ();
    }
    else if (token == EQUALTO_T)
    { // Rule 84
        lex->debugFile << "Using Rule 84\n";
        ruleFile << "Using Rule 84\n";
        cg->WriteCode(0, "" + lex->GetLexeme() + " ");
        token = lex->GetToken ();
    }
    else if (token == GT_T)
    { // Rule 85
        lex->debugFile << "Using Rule 85\n";
        ruleFile << "Using Rule 85\n";
        cg->WriteCode(0, "" + lex->GetLexeme() + " ");
        token = lex->GetToken ();
    }
    else if (token == LT_T)
    { // Rule 86
        lex->debugFile << "Using Rule 86\n";
        ruleFile << "Using Rule 86\n";
        cg->WriteCode(0, "" + lex->GetLexeme() + " ");
        token = lex->GetToken ();
    }
    else if (token == GTE_T)
    { // Rule 87
        lex->debugFile << "Using Rule 87\n";
        ruleFile << "Using Rule 87\n";
        cg->WriteCode(0, "" + lex->GetLexeme() + " ");
        token = lex->GetToken ();
    }
    else if (token == LTE_T)
    { // Rule 88
        lex->debugFile << "Using Rule 88\n";
        ruleFile << "Using Rule 88\n";
        cg->WriteCode(0, "" + lex->GetLexeme() + " ");
        token = lex->GetToken ();
    }
    else if (token == SQUOTE_T)
    { // Rule 89
        lex->debugFile << "Using Rule 89\n";
        ruleFile << "Using Rule 89\n";
        cg->WriteCode(0,"(quote ");
        token = lex->GetToken ();
        
        any_other_token ();
        cg->WriteCode(0,") ");
    }
    else if (token == COND_T)
    { // Rule 90
        lex->debugFile << "Using Rule 90\n";
        ruleFile << "Using Rule 90\n";
        cg->WriteCode(0, "" + lex->GetLexeme() + " ");
        token = lex->GetToken ();
    }
    else if (token == ELSE_T)
    { // Rule 91
        lex->debugFile << "Using Rule 91\n";
        ruleFile << "Using Rule 91\n";
        cg->WriteCode(0, "" + lex->GetLexeme() + " ");
        token = lex->GetToken ();
    }
    else if (token == TRUE_T)
    { // Rule 92
        lex->debugFile << "Using Rule 92\n";
        ruleFile << "Using Rule 92\n";
        cg->WriteCode(0, "" + lex->GetLexeme() + " ");
        token = lex->GetToken ();
    }
    else if (token == FALSE_T)
    { // Rule 93
        lex->debugFile << "Using Rule 93\n";
        ruleFile << "Using Rule 93\n";
        cg->WriteCode(0, "" + lex->GetLexeme() + " ");
        token = lex->GetToken ();
    }
    else
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
    }
    if (follows.find (token) == follows.end())
    {
        errors++;
        sprintf (message, "'%s' unexpected ", lex->GetLexeme().c_str());
        lex->ReportError (message);
        while (follows.find (token) == follows.end())
            token = lex->GetToken ();
    }
   
    sprintf (message, "Exiting Any_Other_Token function; current token is: %s", lex->GetTokenName(token).c_str());
    return;
}

