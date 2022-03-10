%{
    #include "bF.hpp"
    ostringstream literal;
%}

%option yylineno noyywrap

%x string

%%
\.*             {}          // line comment

"'"         { literal.clear(); BEGIN(string); }
<string>"'" { BEGIN(INITIAL); yylval.o = new Str(literal.str()); return STR; }
<string>.   { literal << yytext; }

[+\-]?[0-9]+    TOKEN(Int,NUM)
[^ \t\r\n]+     TOKEN(Sym,SYM)

[ \t\r\n]+      {}              // drop spaces
.               {yyerror("");}  // any undetected char
