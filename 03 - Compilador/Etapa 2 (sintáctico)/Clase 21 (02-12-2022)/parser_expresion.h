#ifndef PARSER_EXPRESION_H
#define PARSER_EXPRESION_H

#include "error.h"
#include "lexer.h"
#include "parser_aux.h"
#include <memory>
#include <utility>
#include <vector>

struct expresion {
   virtual ~expresion( ) = default;
};

struct expresion_termino : expresion {
   const token_anotado& token;

   expresion_termino(const token_anotado& t)
   : token(t) {
   }
};

struct expresion_binaria : expresion {
   std::unique_ptr<expresion> izq;
   const token_anotado& op;
   std::unique_ptr<expresion> der;

   expresion_binaria(std::unique_ptr<expresion> i, const token_anotado& p, std::unique_ptr<expresion> d)
   : izq(std::move(i)), op(p), der(std::move(d)) {
   }
};

struct expresion_prefija : expresion {
   const token_anotado& op;
   std::unique_ptr<expresion> ex;

   expresion_prefija(const token_anotado& p, std::unique_ptr<expresion> e)
   : op(p), ex(std::move(e)) {
   }
};

struct expresion_posfija : expresion {
   std::unique_ptr<expresion> ex;
   const token_anotado& op;

   expresion_posfija(std::unique_ptr<expresion> e, const token_anotado& p)
   : ex(std::move(e)), op(p) {
   }
};

struct expresion_llamada : expresion {
   const token_anotado& funcion;
   std::vector<std::unique_ptr<expresion>> parametros;

   expresion_llamada(const token_anotado& f, std::vector<std::unique_ptr<expresion>> p)
   : funcion(f), parametros(std::move(p)) {
   }
};

struct expresion_indexacion : expresion {
   std::unique_ptr<expresion> ex;
   std::unique_ptr<expresion> indice;

   expresion_indexacion(std::unique_ptr<expresion> e, std::unique_ptr<expresion> i)
   : ex(std::move(e)), indice(std::move(i)) {
   }
};

struct expresion_secuencia : expresion {
   std::vector<std::unique_ptr<expresion>> valores;

   expresion_secuencia(std::vector<std::unique_ptr<expresion>> v)
   : valores(std::move(v)) {
   }
};

std::unique_ptr<expresion> expr(const token_anotado*&);
std::unique_ptr<expresion> expr_primaria(const token_anotado*&);
std::unique_ptr<expresion> expr_unaria(const token_anotado*&);
std::unique_ptr<expresion> expr_binaria(const token_anotado*&, int);

std::vector<std::unique_ptr<expresion>> lista_expr(const token_anotado*& p) {
   // por hacer
   return { };
}

std::unique_ptr<expresion> expr_primaria(const token_anotado*& p) {
   if ((p->tipo == IDENTIFICADOR || p->tipo == PRINT || p->tipo == SCAN) && (p + 1)->tipo == PARENTESIS_IZQ) {
      auto funcion = p;
      p += 2;
      auto params = lista_expr(p);
      espera(p, PARENTESIS_DER);
      return new expresion_llamada(*funcion, std::move(params));
   } else if (p->tipo == IDENTIFICADOR || p->tipo == LITERAL_ENTERA || p->tipo == LITERAL_CADENA) {
      return new expresion_termino(*p++);
   } else if (p->tipo == CORCHETE_IZQ) {
      auto elems = lista_expr(++p);
      espera(p, CORCHETE_DER);
      return new expresion_secuencia(std::move(elems));
   } else if (p->tipo == PARENTESIS_IZQ) {
      auto ex = expr(++p);
      espera(p, PARENTESIS_DER);
      return std::move(ex);
   } else {
      throw error("Expresion esperada", p->vista);
   }
}

std::unique_ptr<expresion> expr_unaria(const token_anotado*& p) {
   if (es_operador_prefijo(p->tipo)) {
      auto temp = p;
      return new expresion_prefija(temp, expr_unaria(++p));
   }

   auto ex = expr_primaria(p);
   while (es_operador_posfijo(p->tipo) || p->tipo == CORCHETE_IZQ) {
      if (es_operador_posfijo(p->tipo)) {
         ex = new expresion_posfija(std::move(ex), *p++);
      } else {
         auto indice = expr(++p);
         espera(p, CORCHETE_DER);
         ex = new expresion_indexacion(std::move(ex), std::move(indice));
      }
   }
   return ex;
}

#endif
