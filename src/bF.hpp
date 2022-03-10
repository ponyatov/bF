#ifndef _bF_H_
#define _bF_H_

#include <cxxabi.h>
#include <assert.h>

#include <iostream>
#include <sstream>
#include <locale>
#include <map>
#include <vector>
#include <stack>
#include <queue>
using namespace std;

/// @defgroup core Object Core / metaL /
/// @brief object (hyper)graph elements

/// @defgroup gc memory manager
/// @brief garbage collector etc
/// @ingroup core

/// @ingroup core
/// @brief object hypergraph node /root class/
class Object {
   private:
    /// @name garbage collector

    /// @brief ref counter
    /// @ingroup gc
    size_t ref;

    /// @brief last in global objects list
    /// @ingroup gc
    static Object* last;

    /// @brief next object in global list
    /// @ingroup gc
    Object* next;

   protected:
    /// @name attribute grammar fields

    /// static value: object name, string/number,..
    string value;

   public:
    /// @name constructor
    Object(string V);
    virtual ~Object();

    /// @name text dump & stringify

    /// full text tree dump
    string dump(int depth = 0, string prefix = "");
    /// tree padding
    string pad(int depth);
    /// `<T:V>` header
    string head(string prefix);
    /// class/type tag
    string tag();
    /// object @ref value as string
    virtual string val();
};

/// @defgroup primitive Primitive
/// @brief numbers, string,..
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
    string val();
};

/// @ingroup primitive
/// @brief integer
class Int : public Num {
   protected:
    int value;

   public:
    Int(string V);
    string val();
};

/// @ingroup primitive
/// @brief string
class Str : public Primitive {
   public:
    Str(string V);
};

/// @defgroup container
/// @brief generic data containers
/// @ingroup core

/// @ingroup container
class Container : public Object {};

/// @brief associative array
/// @ingroup container
class Map : public Container {
    map<string, Object*> slot;
};

/// @brief ordered container
/// @ingroup container
class Vector : public Container {
    vector<Object*> nest;
};

/// @brief LIFO
/// @ingroup container
class Stack : public Container {
    stack<Object*> nest;
};

/// @brief FIFO
/// @ingroup container
class Queue : public Container {
    queue<Object*> nest;
};

/// @defgroup active
/// @brief executable elements (operator, function etc)
/// @ingroup core

/// @brief
/// @ingroup active
class Active : public Object {};

/// @brief Virtual Machine (process)
/// @ingroup active
class VM : public Active {};

/// @brief VM command
/// @ingroup active
class Cmd : public Active {};

/// @brief function
/// @ingroup active
class Fn : public Active {};

/// @brief operator
/// @ingroup active
class Op : public Active {};

/// @defgroup io IO
/// @brief file input/output
/// @ingroup core

/// @defgroup net Net
/// @brief base networking
/// @ingroup io

/// @defgroup forth FORTH
/// @brief WASD-machine
/// @{

/// Words = Vocabulary
extern map<string, Object*> W;
/// Async queue
extern queue<Object*> A;
/// return Stack (call/ret)
extern stack<Object*> S;
/// Data stack
extern stack<Object*> D;

/// @}

/// @defgroup skelex Lexical Skeleton
/// @brief Syntax Lexer/Parser

/// @name lexer

/// @brief lexer (get single token)
/// @ingroup skelex
extern int yylex();

/// @brief current line
/// @ingroup skelex
extern int yylineno;

/// @brief parsed lexeme
/// @ingroup skelex
extern char* yytext;

/// @brief current file handler
/// @ingroup skelex
extern FILE* yyin;

/// @name parser

/// @brief parser
/// @ingroup skelex
extern int yyparse();

/// @brief syntax error callback
/// @ingroup skelex
extern void yyerror(string);

#define TOKEN(C, X)               \
    {                             \
        yylval.o = new C(yytext); \
        return X;                 \
    }
#include "bF.parser.hpp"

#endif  // _bF_H_
