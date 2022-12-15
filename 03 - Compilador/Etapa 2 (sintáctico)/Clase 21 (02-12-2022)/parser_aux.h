#ifndef PARSER_AUX_H
#define PARSER_AUX_H

#include "lexer.h"

void espera(const token_anotado*& p, token esperado) {
   if (p->tipo != esperado) {
      throw error("Token inesperado", p->vista);
   }
   ++p;
}

bool es_operador_prefijo(token t) {
   return t == MAS || t == MENOS ||
          t == NOT || t == TAMANYO ||
          t == DIRECCION;
}

bool es_operador_binario(token t) {
   return t == MAS || t == MENOS ||
          t == POR || t == ENTRE ||
          t == MODULO || t == ASIGNACION ||
          t == MAS_ASIGNACION ||
          t == MENOS_ASIGNACION ||
          t == POR_ASIGNACION ||
          t == ENTRE_ASIGNACION ||
          t == MODULO_ASIGNACION ||
          t == MENOR || t == MENOR_IGUAL ||
          t == MAYOR || t == MAYOR_IGUAL ||
          t == IGUAL || t == DIFERENTE ||
          t == AND || t == OR;
}

bool es_operador_posfijo(token t) {
   return t == DESREFERENCIA;
}

bool es_inicio_expr(token t) {
   return t == IDENTIFICADOR ||
          t == LITERAL_ENTERA ||
          t == LITERAL_CADENA ||
          t == PARENTESIS_IZQ ||
          t == CORCHETE_IZQ ||
          es_operador_prefijo(t);
}

#endif
