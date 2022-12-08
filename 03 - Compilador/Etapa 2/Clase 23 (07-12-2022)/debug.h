#ifndef DEBUG_H
#define DEBUG_H

#include "lexer.h"
#include "parser.h"
#include <iostream>

std::ostream& operator<<(std::ostream& os, const token_anotado& t) {
   return os << t.vista << " " << t.tipo;
}

std::ostream& operator<<(std::ostream&, const expresion&);

std::ostream& operator<<(std::ostream& os, const std::vector<std::unique_ptr<expresion>>& lista) {
   for (int i = 0; i < lista.size( ); ++i) {
      os << *lista[i];
      if (i + 1 < lista.size( )) {
          os << ",";
      }
   }
   return os;
}

std::ostream& operator<<(std::ostream& os, const expresion& ex) {
   if (auto p = dynamic_cast<const expresion_termino*>(&ex); p != nullptr) {
      return os << p->token.vista;
   } else if (auto p = dynamic_cast<const expresion_binaria*>(&ex); p != nullptr) {
      return os << "(" << *p->izq << p->op.vista << *p->der << ")";
   } else if (auto p = dynamic_cast<const expresion_prefija*>(&ex); p != nullptr) {
      return os << p->op.vista << "(" << *p->ex << ")";
   } else if (auto p = dynamic_cast<const expresion_posfija*>(&ex); p != nullptr) {
      return os << "(" << *p->ex << ")" << p->op.vista;
   } else if (auto p = dynamic_cast<const expresion_llamada*>(&ex); p != nullptr) {
      return os << p->funcion.vista << "(" << p->parametros << ")";
   } else if (auto p = dynamic_cast<const expresion_indexacion*>(&ex); p != nullptr) {
      return os << *p->ex << "[" << *p->indice << "]";
   } else if (auto p = dynamic_cast<const expresion_secuencia*>(&ex); p != nullptr) {
      return os << "[" << p->valores << "]";
   } else {
      return os;
   }
}

#endif

/*
//Ejemplo de entrada:
a = 1 < 2 + 3 * arr[1 + 2] - (f( ) * g( ) / 2 + 1) + z

a = 1 + 2 + 3

a = b = c = d = 5

1 + 2 * 3 < arr[1 + 2 * 3]
*/
