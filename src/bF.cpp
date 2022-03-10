#include "bF.hpp"

int main(int argc, char* argv[]) {
    printf("\n%s:\n", argv[0]);
    for (int i = 1; i < argc; i++) {
        printf("argv[%i] = <%s>\n", i, argv[i]);
        assert(yyin = fopen(argv[i], "r"));
        yyparse();
        fclose(yyin);
    }
    printf("\n");
    return 0;
}

#define YYERR "\n\n" << yylineno << ":" << msg << "[" << yytext << "]\n\n"
void yyerror(string msg) {
    cerr << YYERR;
    exit(-1);
}

Object* Object::last = nullptr;

Object::Object(string V) {
    value = V;
    next = last;
    last = this;
}

Object::~Object() {
    if (last == this) last = next;
}

string Object::dump(int depth, string prefix) {
    ostringstream ret;
    ret << pad(depth) << head(prefix);
    return ret.str();
}

string Object::pad(int depth) {
    ostringstream ret;
    ret << '\n';
    for (uint i = 0; i < depth; i++) ret << '\t';
    return ret.str();
}

string Object::head(string prefix) {
    ostringstream ret;
    ostringstream gid;
    gid << " @" << this;
    ret << prefix << '<' << tag() << ':' << val() << '>' << gid.str();
    return ret.str();
}

string Object::tag() {
    string name(
        abi::__cxa_demangle(typeid(*this).name(), nullptr, nullptr, nullptr));
    ostringstream ret;
    static locale loc;
    for (auto c : name) ret << tolower(c, loc);
    return ret.str();
}

string Object::val() { return value; }

Primitive::Primitive(string V) : Object(V) {}
Sym::Sym(string V) : Primitive(V) {}
Num::Num(string V) : Primitive("") { value = atof(V.c_str()); }
Int::Int(string V) : Num("") { value = atoi(V.c_str()); }
Str::Str(string V) : Primitive(V) {}

string Num::val() { return to_string(value); }
string Int::val() { return to_string(value); }

map<string, Object*> W;
queue<Object*> A;
stack<Object*> S;
stack<Object*> D;
