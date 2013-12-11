%{

#include <scanner.h>

/* --- Your code here ---
 *
 * This is where C declarations go. If you need to include
 * files for the actions below, this is where you do it.
 */

/* --- End your code --- */

extern void yyerror(char *);
%}

%option yylineno
%option noyywrap
%option 8bit
%option caseless
%option nodefault

digit                             [0-9]
integer                           {digit}+
sign                              [+-]
exponent                          [Ee]({sign})?{integer}
real                              (\.{integer}|{integer}\.({integer})?)({exponent})?
integer_with_exponent             {integer}{exponent}

identifier                        [a-zA-Z][a-zA-Z0-9_]*

cpp_comment                       \/\/.*\n

whitespace                        [ \t]*

%%

if                                  return IF;
then                                return THEN;
elseif                              return ELSEIF;
else                                return ELSE;
begin                               return XBEGIN;
end                                 return XEND;
while                               return WHILE;
function                            return FUNCTION;
program                             return PROGRAM;
return                              return RETURN;
declare                             return DECLARE;
do                                  return DO;
and                                 return AND;
or                                  return OR;
not                                 return NOT;
:=                                  return ASSIGN;
">="                                return GE;
"<="                                return LE;
==                                  return EQ;
"<>"                                return NE;
array                               return ARRAY;
of                                  return OF;

{identifier}                        return ID;

{real}                              return REAL;
{integer_with_exponent}             return REAL;
{integer}                           return INTEGER;

{cpp_comment}                       { }

{whitespace}                        { }

(.|\n)                              return yytext[0];
<<EOF>>                             yyterminate();
%%
