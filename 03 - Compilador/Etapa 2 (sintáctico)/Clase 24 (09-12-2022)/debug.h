#ifndef DEBUG_H
#define DEBUG_H

#include "lexer.h"
#include "parser.h"
#include <iostream>

std::ostream& operator<<(std::ostream& os, const token_anotado& t) {
   return os << t.vista << " " << t.tipo;
}

std::ostream& operator<<(std::ostream& os, const std::unique_ptr<expresion>& ex);

std::ostream& operator<<(std::ostream& os, const std::vector<std::unique_ptr<expresion>>& lista) {
   for (int i = 0; i < lista.size( ); ++i) {
      os << lista[i] << (i + 1 < lista.size( ) ? "," : "");
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
      return os << p->ex << p->op.vista;
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

std::ostream& operator<<(std::ostream& os, const std::unique_ptr<sentencia>& ex);

std::ostream& operator<<(std::ostream& os, const std::vector<std::unique_ptr<sentencia>>& lista) {
   for (int i = 0; i < lista.size( ); ++i) {
      os << lista[i] << "\n";
   }
   return os;
}

std::ostream& operator<<(std::ostream& os, const std::unique_ptr<sentencia_declaracion>& s) {
   if (s != nullptr) {
      os << s->tipo << " " << s->nombre.vista << (s->inicializador != nullptr ? " = " : "") << s->inicializador;
   }
   return os;
};
std::ostream& operator<<(std::ostream& os, const std::unique_ptr<sentencia>& s) {
   if (auto p = dynamic_cast<const sentencia_expresion*>(s.get( )); p != nullptr) {
      return os << p->ex << ";";
   } else if (auto p = dynamic_cast<const sentencia_declaracion*>(s.get( )); p != nullptr) {
      return os << reinterpret_cast<const std::unique_ptr<sentencia_declaracion>&>(s) << ";";
   } else if (auto p = dynamic_cast<const sentencia_if*>(s.get( )); p != nullptr) {
      return os << "if " << p->condicion << "{\n" << p->parte_si << "} else {\n" << p->parte_no << "}";
   } else if (auto p = dynamic_cast<const sentencia_while*>(s.get( )); p != nullptr) {
      return os << "while " << p->condicion << "{\n" << p->cuerpo << "}";
   } else if (auto p = dynamic_cast<const sentencia_do*>(s.get( )); p != nullptr) {
      return os << "do " << "{\n" << p->cuerpo << "} while " << p->condicion << ";";
   } else if (auto p = dynamic_cast<const sentencia_for*>(s.get( )); p != nullptr) {
      return os << "for " << p->inicializacion << ";" << p->condicion << ";" << p->actualizacion << "{\n" << p->cuerpo << "}";
   } else if (auto p = dynamic_cast<const sentencia_return*>(s.get( )); p != nullptr) {
      return os << "return " << p->ex << ";";
   } else if (auto p = dynamic_cast<const sentencia_break*>(s.get( )); p != nullptr) {
      return os << "break;";
   } else if (auto p = dynamic_cast<const sentencia_continue*>(s.get( )); p != nullptr) {
      return os << "continue;";
   } else if (auto p = dynamic_cast<const sentencia_exit*>(s.get( )); p != nullptr) {
      return os << "exit;";
   } else {
      return os;
   }
}

std::ostream& operator<<(std::ostream& os, const declaracion_funcion& f) {
   os << f.tipo << " " << f.nombre.vista << "(";
   for (int i = 0; i < f.parametros.size( ); ++i) {
      os << f.parametros[i]->tipo << " " << f.parametros[i]->nombre.vista << (i + 1 < f.parametros.size( ) ? "," : "");
   }
   os << ") {\n";
   for (const auto& s : f.cuerpo) {
      os << s << "\n";
   }
   os << "}";
   return os;
}

#endif
