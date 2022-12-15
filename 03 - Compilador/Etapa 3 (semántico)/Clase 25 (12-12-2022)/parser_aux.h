#ifndef PARSER_AUX_H
#define PARSER_AUX_H

#include "lexer.h"

struct control_vista {
   const token_anotado* ini;
   const token_anotado*& ref;

   control_vista(const token_anotado*& p)
   : ini(p), ref(p) {
   }

   explicit operator std::string_view( ) const {
      return std::string_view(
         ini->vista.begin( ),
         (ref - bool(ini != ref))->vista.end( ) - ini->vista.begin( )
      );
   }
};

const token_anotado* espera(const token_anotado*& p, token esperado) {
   if (p->tipo != esperado) {
      throw error("Token inesperado", p->vista);
   }
   return p++;
}

bool es_operador_prefijo(token t) {
   return t == MAS || t == MENOS || t == NOT || t == TAMANYO || t == DIRECCION;
}

bool es_operador_binario(token t) {
   return t == MAS || t == MENOS || t == POR || t == ENTRE || t == MODULO ||
          t == ASIGNACION ||
          t == MAS_ASIGNACION || t == MENOS_ASIGNACION || t == POR_ASIGNACION || t == ENTRE_ASIGNACION || t == MODULO_ASIGNACION ||
          t == MENOR || t == MENOR_IGUAL || t == MAYOR || t == MAYOR_IGUAL || t == IGUAL || t == DIFERENTE ||
          t == AND || t == OR;
}

bool es_operador_posfijo(token t) {
   return t == DESREFERENCIA;
}

bool es_tipo(token t) {
   return t == INT || t == CHAR || t == VOID;
}

bool es_inicio_expr(token t) {
   return t == IDENTIFICADOR || t == LITERAL_ENTERA || t == LITERAL_CADENA ||
          t == PRINT || t == SCAN ||
          t == PARENTESIS_IZQ || t == CORCHETE_IZQ ||
          es_tipo(t) || es_operador_prefijo(t);
}

int precedencia(token t) {
   switch (t) {
   case ASIGNACION:
   case MAS_ASIGNACION:
   case MENOS_ASIGNACION:
   case POR_ASIGNACION:
   case ENTRE_ASIGNACION:
   case MODULO_ASIGNACION:
      return 0;
   case OR:
      return 1;
   case AND:
      return 2;
   case MENOR:
   case MENOR_IGUAL:
   case MAYOR:
   case MAYOR_IGUAL:
   case IGUAL:
   case DIFERENTE:
      return 3;
   case MAS:
   case MENOS:
      return 4;
   case POR:
   case ENTRE:
   case MODULO:
      return 5;
   default:
      return -1;
   }
}

int asociatividad(token t) {
   switch (t) {
   case ASIGNACION:
   case MAS_ASIGNACION:
   case MENOS_ASIGNACION:
   case POR_ASIGNACION:
   case ENTRE_ASIGNACION:
   case MODULO_ASIGNACION:
      return 0;
   default:
      return 1;
   }
}

#endif
