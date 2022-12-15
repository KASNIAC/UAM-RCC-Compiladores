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

std::ostream& operator<<(std::ostream& os, const std::unique_ptr<expresion>& ex) {
   if (auto p = dynamic_cast<const expresion_termino*>(ex.get( )); p != nullptr) {
      return os << p->token.vista;
   } else if (auto p = dynamic_cast<const expresion_binaria*>(ex.get( )); p != nullptr) {
      return os << "(" << p->izq << p->op.vista << p->der << ")";
   } else if (auto p = dynamic_cast<const expresion_prefija*>(ex.get( )); p != nullptr) {
      return os << p->op.vista << "(" << p->ex << ")";
   } else if (auto p = dynamic_cast<const expresion_posfija*>(ex.get( )); p != nullptr) {
      return os << "(" << p->ex << ")" << p->op.vista;
   } else if (auto p = dynamic_cast<const expresion_llamada*>(ex.get( )); p != nullptr) {
      return os << p->funcion.vista << "(" << p->parametros << ")";
   } else if (auto p = dynamic_cast<const expresion_indexacion*>(ex.get( )); p != nullptr) {
      return os << p->ex << "[" << p->indice << "]";
   } else if (auto p = dynamic_cast<const expresion_secuencia*>(ex.get( )); p != nullptr) {
      return os << "[" << p->valores << "]";
   } else {
      return os;
   }
}

#endif
