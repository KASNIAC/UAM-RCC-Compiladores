#ifndef PARSER_EXPRESION_H
#define PARSER_EXPRESION_H

#include <memory>
#include <utility>
#include <vector>

struct token_anotado { };

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

int main( ) {
   token_anotado vamos_polonia;
   expresion_termino ex = { vamos_polonia };
}

#endif
