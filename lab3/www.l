%{
  
#include <stdio.h>
int yyparse (void);
int yyerror(char* s);
%}

proto ^[a-z]+
sep :\/\/
tok [a-zA-Z0-9]+(\-[a-zA-Z0-9]+)*
dottok \.[a-zA-Z0-9]+(\-[a-zA-Z0-9]+)*

%%
{proto}  { printf("PROTO: %s\n",	yytext);}
{sep}    { printf("SEP: %s\n",  	yytext);}
{dottok} { printf("dottok: %s\n",	yytext);}
{tok}	 { printf("tok: %s\n",  	yytext);}
<<EOF>> return EOF;
.		return 256;
%%
int  yyerror(char *s) {
  return fprintf (stderr, "%s\n", s);
}

int main(int argc, char const * argv[]) {
    YY_BUFFER_STATE buf;
	yy_flex_debug = 1;
	for (int i = 1; i < argc; i++)
	{
		printf("str: '%s'\n", argv[i]);
		buf = yy_scan_string(argv[i]);
		yy_switch_to_buffer (buf);
		int ret = yyparse();
		printf("ret: %d\n\n", ret);
		yy_delete_buffer(buf);
	}

    return 0;
}