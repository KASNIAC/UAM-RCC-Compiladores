#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "parser_aux.h"
#include "parser_expresion.h"
#include "parser_sentencia.h"
#include <memory>
#include <vector>

struct declaracion_funcion {
   std::unique_ptr<expresion> tipo;
   const token_anotado& nombre;
   std::vector<std::unique_ptr<sentencia_declaracion>> parametros;
   std::vector<std::unique_ptr<sentencia>> cuerpo;
};

struct arbol_sintactico {
   std::vector<declaracion_funcion> funciones;
};

declaracion_funcion parser_funcion(const token_anotado*& p) {
   //...
}

arbol_sintactico parser(const std::vector<token_anotado>& tokens) {
   auto p = &tokens[0];
   arbol_sintactico arbol;
   while (p->tipo != FIN_ARCHIVO) {
      arbol.push_back(parser_funcion(p));
   }
   return arbol;
}

#endif
