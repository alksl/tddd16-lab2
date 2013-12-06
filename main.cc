#include <iostream.h>
#include <iomanip.h>
#include <stdio.h>
#include <stdlib.h>

#include "scanner.h"

typedef struct
{
    int      token;
    char    *name;
} tTokenName;


tTokenName tokens[] = {{ FUNCTION,   "FUNCTION" },
                       { ID,         "ID" },
                       { DECLARE,    "DECLARE" },
                       { ARRAY,      "ARRAY" },
                       { INTEGER,    "INTEGER" },
                       { OF,         "OF" },
                       { REAL,       "REAL" },
                       { XBEGIN,     "XBEGIN" },
                       { XEND,       "XEND" },
                       { IF,         "IF" },
                       { THEN,       "THEN" },
                       { ELSE,       "ELSE" },
                       { WHILE,      "WHILE" },
                       { DO,         "DO" },
                       { ASSIGN,     "ASSIGN" },
                       { RETURN,     "RETURN" },
                       { GE,         "GE" },
                       { LE,         "LE" },
                       { EQ,         "EQ" },
                       { NE,         "NE" },
                       { TRUE,       "TRUE" },
                       { FALSE,      "FALSE" },
                       { PROGRAM,    "PROGRAM" },
                       { ELSEIF,     "ELSEIF" },
                       { NOT,        "NOT" },
                       { AND,        "AND" },
                       { OR,         "OR" },
                       { UMINUS,     "UMINUS" }};

int numTokens = sizeof(tokens)/sizeof(*tokens);


ostream& PrintToken(ostream& o, int token)
{
    int i;
    extern char *yytext;

    for (i = 0; i < numTokens; i++)
    {
        if (token == tokens[i].token)
        {
            o << tokens[i].name << " \'" << yytext << "\'";
            return o;
        }
    }

    o << '\'' << (unsigned char)token << '\'';

    return o;
}

//  omanip<int> Token(int t)
//  {
//      return omanip<int>(&PrintToken, t);
//  }

int main(int argc, char **argv)
{
    int     token;
    extern  FILE *yyin;
    extern  int yylex();

    /*
     * Open the input file, if any
     */

    switch(argc)
    {
    case 1:
        yyin = stdin;
        break;
    case 2:
        yyin = fopen(argv[1], "r");
        if (yyin == NULL)
        {
            perror(argv[1]);
            exit(1);
        }
        break;
    default:
        cerr << "Usage: " << argv[0] << " [ filename ]\n";
        exit(1);
    }


    /*
     * Loop for as long as there are tokens
     */

    while ((token = yylex()) != 0)
    {
      cout << "Scanned "; PrintToken(cout, token); cout << '\n' << flush;
    }

    cout << "End of file\n";
    exit(0);
}

void yyerror(char *msg)
{
    extern int yylineno;

    cerr << "Error at line " << yylineno << ": " << msg << '\n' << flush;
}
