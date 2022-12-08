#ifndef PARSER_SENTENCIA_H
#define PARSER_SENTENCIA_H

#include "parser_aux.h"
#include "parser_expresion.h"
#include <memory>
#include <vector>

struct tipo_declaracion {
   const token_anotado& tipo_base;
   int modificador;  // -1 es variable normal, -2 es apuntador, >= 0 es arreglo
};

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
   tipo_declaracion tipo;
   const token_anotado& nombre;
   std::unique_ptr<expresion> inicializador;

   sentencia_declaracion(tipo_declaracion t, const token_anotado& n, std::unique_ptr<expresion> i)
   : tipo(t), nombre(n), inicializador(std::move(i)) {
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

struct sentencia_break : sentencia {

};

struct sentencia_continue : sentencia {

};

struct sentencia_return : sentencia {
   std::unique_ptr<expresion> ex;

   sentencia_return(std::unique_ptr<expresion> e)
   : ex(std::move(e)) {
   }
};

struct sentencia_exit : sentencia {

};

std::unique_ptr<sentencia> parsea_sentencia(const token_anotado*& p) {
   if (p->tipo == BREAK) {
      espera(++p, PUNTO_COMA);
      return std::make_unique<sentencia_break>( );
   } else if (p->tipo == CONTINUE) {
      espera(++p, PUNTO_COMA);
      return std::make_unique<sentencia_continue>( );
   } else if (p->tipo == EXIT) {
      espera(++p, PUNTO_COMA);
      return std::make_unique<sentencia_exit>( );
   }
}
