#ifndef LEXER_H
#define LEXER_H

#include <string_view>

enum token {
   IDENTIFICADOR,
   INT,
   CHAR,
   VOID,
   MAS,
   MENOS,
   POR,
   ENTRE,
   MODULO,
   ASIGNACION,
   MAS_ASIGNACION,
   MENOS_ASIGNACION,
   POR_ASIGNACION,
   ENTRE_ASIGNACION,
   MODULO_ASIGNACION,
   MENOR,
   MENOR_IGUAL,
   MAYOR,
   MAYOR_IGUAL,
   IGUAL,
   DIFERENTE,
   AND,
   OR,
   NOT,
   TAMAÑO,
   DIRECCION,
   DESREFERENCIA,
   PARENTESIS_IZQ,
   PARENTESIS_DER,
   CORCHETE_IZQ,
   CORCHETE_DER,
   LLAVE_IZQ,
   LLAVE_DER,
   COMA,
   PUNTO_COMA,
   IF,
   ELSE,
   WHILE,
   DO,
   FOR,
   BREAK,
   CONTINUE,
   RETURN,
   PRINT,
   SCAN,
   EXIT,
   LITERAL_ENTERA,
   LITERAL_CADENA,
   FIN_ARCHIVO,
   DESCONOCIDO
};

struct token_anotado {
   token tipo;
   std::string_view pos;
};

#endif
