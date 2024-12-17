%{
#include <stdio.h>
int yylex(void);
int yyerror(const char* s);
int yyparse (void);
#define YYDEBUG 1
%}

%token PROTO
%token SEP
%token TOK
%token DOTTOK

%start s
%define parse.trace
%define parse.error detailed

%%
s:
 | PROTO SEP body
 ;
body: TOK inner_body
 ;
inner_body: DOTTOK inner_body
 | %empty
 ;

%%

yydebug = 0;