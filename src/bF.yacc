%{
    #include "bF.hpp"
%}

%defines %union { Object *o; }

%type  <o> ex
%token <o> SYM NUM STR

%%

REPL :
REPL : REPL ex  { cout << $2->dump(); }

ex : SYM
ex : NUM
ex : STR
