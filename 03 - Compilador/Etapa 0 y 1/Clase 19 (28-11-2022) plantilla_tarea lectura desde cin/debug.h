#ifndef DEBUG_H
#define DEBUG_H

#include "lexer.h"
#include <iostream>

std::ostream& operator<<(std::ostream& os, const token_anotado& t) {
   return os << t.vista << " " << t.tipo;
}

#endif
