#ifndef PARSER_SENTENCIA_H
#define PARSER_SENTENCIA_H

#include "parser_aux.h"
#include "parser_expresion.h"
#include <memory>
#include <string>
#include <vector>

struct sentencia {
   virtual ~sentencia( ) = default;
};

struct sentencia_expresion : sentencia {
   std::unique_ptr<expresion> ex;

   sentencia_expresion(std::unique_ptr<expresion> e)
   : ex(std::move(e)) {
   }
};

struct sentencia_declaracion : sentencia {
   std::unique_ptr<expresion> tipo;
   const token_anotado& nombre;
   std::unique_ptr<expresion> inicializador;

   sentencia_declaracion(std::unique_ptr<expresion> t, const token_anotado& n, std::unique_ptr<expresion> i)
   : tipo(std::move(t)), nombre(n), inicializador(std::move(i)) {
   }
};

struct sentencia_if : sentencia {
   std::unique_ptr<expresion> condicion;
   std::vector<std::unique_ptr<sentencia>> parte_si;
   std::vector<std::unique_ptr<sentencia>> parte_no;

   sentencia_if(std::unique_ptr<expresion> c, std::vector<std::unique_ptr<sentencia>> s, std::vector<std::unique_ptr<sentencia>> n)
   : condicion(std::move(c)), parte_si(std::move(s)), parte_no(std::move(n)) {
   }
};

struct sentencia_while : sentencia {
   std::unique_ptr<expresion> condicion;
   std::vector<std::unique_ptr<sentencia>> cuerpo;

   sentencia_while(std::unique_ptr<expresion> c, std::vector<std::unique_ptr<sentencia>> v)
   : condicion(std::move(c)), cuerpo(std::move(v)) {
   }
};

struct sentencia_do : sentencia {
   std::vector<std::unique_ptr<sentencia>> cuerpo;
   std::unique_ptr<expresion> condicion;

   sentencia_do(std::vector<std::unique_ptr<sentencia>> v, std::unique_ptr<expresion> c)
   : cuerpo(std::move(v)), condicion(std::move(c)) {
   }
};

struct sentencia_for : sentencia {
   std::unique_ptr<sentencia_declaracion> inicializacion;
   std::unique_ptr<expresion> condicion;
   std::unique_ptr<expresion> actualizacion;
   std::vector<std::unique_ptr<sentencia>> cuerpo;

   sentencia_for(std::unique_ptr<sentencia_declaracion> i, std::unique_ptr<expresion> c, std::unique_ptr<expresion> a, std::vector<std::unique_ptr<sentencia>> v)
   : inicializacion(std::move(i)), condicion(std::move(c)), actualizacion(std::move(a)), cuerpo(std::move(v)) {
   }
};

struct sentencia_return : sentencia {
   std::unique_ptr<expresion> ex;

   sentencia_return(std::unique_ptr<expresion> e)
   : ex(std::move(e)) {
   }
};

struct sentencia_break : sentencia {

};

struct sentencia_continue : sentencia {

};

struct sentencia_exit : sentencia {

};

std::unique_ptr<sentencia> stmt(const token_anotado*&);

std::vector<std::unique_ptr<sentencia>> lista_stmt(const token_anotado*& p) {
   std::vector<std::unique_ptr<sentencia>> res;
   while (p->tipo != LLAVE_DER) {
      res.push_back(stmt(p));
   }
   return res;
}

std::unique_ptr<sentencia_declaracion> stmt_declaracion(const token_anotado*& p, std::unique_ptr<expresion> tipo, bool permitir_init) {
   auto nombre = espera(p, IDENTIFICADOR);
   auto ex = (p->tipo == ASIGNACION && permitir_init ? expr(++p) : nullptr);
   return std::make_unique<sentencia_declaracion>(std::move(tipo), *nombre, std::move(ex));
}

std::unique_ptr<sentencia_declaracion> stmt_declaracion(const token_anotado*& p, bool permitir_init) {
   return stmt_declaracion(p, expr(p), permitir_init);
}

std::unique_ptr<sentencia> stmt(const token_anotado*& p) {
   if (p->tipo == IF) {
      auto ex = expr(++p);
      espera(p, LLAVE_IZQ);
      auto parte_si = lista_stmt(p), parte_no = decltype(parte_si)( );
      espera(p, LLAVE_DER);
      if (p->tipo == ELSE) {
         if ((++p)->tipo == IF) {
            parte_no.push_back(stmt(p));
         } else {
            espera(p, LLAVE_IZQ);
            parte_no = lista_stmt(p);
            espera(p, LLAVE_DER);
         }
      }
      return std::make_unique<sentencia_if>(std::move(ex), std::move(parte_si), std::move(parte_no));
   } else if (p->tipo == WHILE) {
      auto ex = expr(++p);
      espera(p, LLAVE_IZQ);
      auto cuerpo = lista_stmt(p);
      espera(p, LLAVE_DER);
      return std::make_unique<sentencia_while>(std::move(ex), std::move(cuerpo));
   } else if (p->tipo == DO) {
      // por hacer
   } else if (p->tipo == FOR) {
      auto init = (es_inicio_expr((++p)->tipo) ? stmt_declaracion(p, true) : nullptr);
      espera(p, PUNTO_COMA);
      auto cond = (es_inicio_expr(p->tipo) ? expr(p) : nullptr);
      espera(p, PUNTO_COMA);
      auto act = (es_inicio_expr(p->tipo) ? expr(p) : nullptr);
      espera(p, LLAVE_IZQ);
      auto cuerpo = lista_stmt(p);
      espera(p, LLAVE_DER);
      return std::make_unique<sentencia_for>(std::move(init), std::move(cond), std::move(act), std::move(cuerpo));
   } else if (p->tipo == RETURN) {
      auto ex = (es_inicio_expr((++p)->tipo) ? expr(p) : nullptr);
      espera(p, PUNTO_COMA);
      return std::make_unique<sentencia_return>(std::move(ex));
   } else if (p->tipo == BREAK) {
      espera(++p, PUNTO_COMA);
      return std::make_unique<sentencia_break>( );
   } else if (p->tipo == CONTINUE) {
      espera(++p, PUNTO_COMA);
      return std::make_unique<sentencia_continue>( );
   } else if (p->tipo == EXIT) {
      espera(++p, PUNTO_COMA);
      return std::make_unique<sentencia_exit>( );
   } else {
      auto ex = expr(p);
      if (p->tipo == IDENTIFICADOR) {
         auto sen = stmt_declaracion(p, std::move(ex), true);
         espera(p, PUNTO_COMA);
         return sen;
      } else {
         espera(p, PUNTO_COMA);
         return std::make_unique<sentencia_expresion>(std::move(ex));
      }
   }
}

#endif
