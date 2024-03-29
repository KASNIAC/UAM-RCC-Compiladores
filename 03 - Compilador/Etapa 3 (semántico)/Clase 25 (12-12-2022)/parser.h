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
   auto tipo = expr(p);
   auto nombre = espera(p, IDENTIFICADOR);
   espera(p, PARENTESIS_IZQ);
   std::vector<std::unique_ptr<sentencia_declaracion>> params;
   espera(p, PARENTESIS_DER);
   espera(p, LLAVE_IZQ);
   auto cuerpo = lista_stmt(p);
   espera(p, LLAVE_DER);
   return { std::move(tipo), *nombre, std::move(params), std::move(cuerpo) };

   // por hacer: obtengan el tipo de retorno, el apuntador al nombre, los par�metros y el cuerpo para despu�s escribir
   //    return { std::move(tipo), *nombre, std::move(params), std::move(cuerpo) };
}

arbol_sintactico parser(const std::vector<token_anotado>& tokens) {
   auto p = &tokens[0];
   arbol_sintactico arbol;
   while (p->tipo != FIN_ARCHIVO) {
      arbol.funciones.push_back(parser_funcion(p));
   }
   return arbol;
}

#endif
