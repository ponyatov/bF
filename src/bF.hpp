#ifndef _bF_H_
#define _bF_H_

#include <assert.h>

#include <iostream>
#include <sstream>
using namespace std;

/// @defgroup core Object Core / metaL /

/// @ingroup core
/// @brief object hypergraph node /root class/
class Object {
   private:
    /// @name garbage collector

    /// ref counter
    size_t ref;
    /// last in global object list
    static Object* last;
    /// next object in global list
    Object* next;

   protected:
    /// static value: object name, string/number,..
    string value;

   public:
    Object(string V);
    virtual ~Object();
    string dump(int depth = 0, string prefix = "");
    string pad(int depth);
    string head(string prefix);
    virtual string tag();
    virtual string val();
};

/// @defgroup primitive Primitive
/// @ingroup core

/// @ingroup primitive
class Primitive : public Object {
   public:
    Primitive(string V);
};

/// @ingroup primitive
/// @brief symbol
class Sym : public Primitive {
   public:
    Sym(string V);
};

/// @ingroup primitive
/// @brief number /floating point/
class Num : public Primitive {
   protected:
    float value;

   public:
    Num(string V);
};

/// @ingroup primitive
/// @brief integer
class Int : public Num {
   protected:
    int value;

   public:
    Int(string V);
};

/// @ingroup primitive
/// @brief string
class Str : public Primitive {
   public:
    Str(string V);
};

extern int yylex();
extern int yylineno;
extern char* yytext;
extern FILE* yyin;
extern int yyparse();
extern void yyerror(string);
#define TOKEN(C, X)               \
    {                             \
        yylval.o = new C(yytext); \
        return X;                 \
    }
#include "bF.parser.hpp"

#endif  // _bF_H_
