#ifndef SEMANTICO_H
#define SEMANTICO_H

#include "error.h"
#include "lexer.h"
#include "parser.h"
#include <charconv>
#include <map>
#include <string_view>
#include <vector>

enum modificador_tipo {
   VALOR, APUNTADOR, ARREGLO
};

enum tiempo_vida {
   VARIABLE, TEMPORAL
};

enum contexto_tipo {
   RETORNO, PARAMETRO, LOCAL, MOLDEADO
};

struct tipo_evaluado {
   token tipo_base;
   modificador_tipo modificador;
   tiempo_vida vida;
   int tamanyo;
};

struct info_funcion {
   const declaracion_funcion& decl;
   tipo_evaluado retorno;
   std::vector<tipo_evaluado> parametros;
   std::map<std::string_view, int> parametros_por_nombre;
};

struct tabla_simbolos {
   std::map<std::string_view, info_funcion> funciones;
};

tipo_evaluado evalua_tipo(const std::unique_ptr<expresion>& ex, contexto_tipo contexto) {
   auto vida = (contexto == RETORNO || contexto == MOLDEADO ? TEMPORAL : VARIABLE);
   if (auto b = dynamic_cast<const expresion_termino*>(ex.get( )); b != nullptr) {
      if (es_tipo(b->tipo) && (b->tipo != VOID || contexto == RETORNO)) {
         return { b->tipo, VALOR, vida };
      }
   } else if (auto p = dynamic_cast<const expresion_posfija*>(ex.get( )); p != nullptr) {
      if (auto b = dynamic_cast<const expresion_termino*>(p->ex.get( )); b != nullptr) {
         if (es_tipo(b->tipo) && b->tipo != VOID && p->op.tipo == DESREFERENCIA && contexto != MOLDEADO) {
            return { b->tipo, APUNTADOR, vida };
         }
      }
   } else if (auto p = dynamic_cast<const expresion_indexacion*>(ex.get( )); p != nullptr) {
      if (auto b = dynamic_cast<const expresion_termino*>(p->ex.get( )); b != nullptr) {
         if (auto x = dynamic_cast<const expresion_termino*>(p->indice.get( )); x != nullptr) {
            if (es_tipo(b->tipo) && b->tipo != VOID && x->tipo == LITERAL_ENTERA && contexto == LOCAL) {
               if (int tam; std::from_chars(ini, fin, tam).ec == std::errc( )) {
                  return { b->tipo, ARREGLO, vida, tam };
               }
            }
         }
      }
   }

   throw error("Tipo incorrecto", ex->vista);
}

tabla_simbolos semantico(const arbol_sintactico& arbol) {
   tabla_simbolos tabla;
   for (const auto& funcion : arbol.funciones) {
      tipo_evaluado ret = evalua_tipo(funcion.retorno, RETORNO);
      std::vector<tipo_evaluado> params;
      std::map<std::string_view, int> params_por_nombre;
      for (const auto& parametro : funcion.parametros) {
         params.push_back(evalua_tipo(parametro.tipo, PARAMETRO));
         if (!params_por_nombre.emplace(parametro.nombre.vista, params.size( ) - 1).second) {
            throw error("Nombre de parametro repetido", );
         }
      }
      if (!tabla.funciones.emplace(
         funcion.nombre.vista, info_funcion(funcion, ret, std::move(params), std::move(params_por_nombre))
      ).second) {
         throw error("Nombre de funcion repetida", funcion.nombre.vista);
      }
   }
   return tabla;
}

#endif
