#ifndef LEXER_H
#define LEXER_H

#include <map>
#include <string_view>
#include <vector>
#include <ctype.h>

enum token {
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
   IDENTIFICADOR,
   LITERAL_ENTERA,
   LITERAL_CADENA,
   FIN_ARCHIVO,
   DESCONOCIDO
};

const std::map<std::string, token> palabras = {
   { "int", INT },
   { "char", CHAR },
   { "void", VOID },
   { "if", IF },
   { "else", ELSE },
   { "while", WHILE },
   { "do", DO },
   { "for", FOR },
   { "break", BREAK },
   { "continue", CONTINUE },
   { "return", RETURN },
   { "print", PRINT },
   { "scan", SCAN },
   { "exit", EXIT }
};

const std::map<std::string, token> op_puntuacion = {
   { "+", MAS },
   { "-", MENOS },
   { "*", POR },
   { "/", ENTRE },
   { "%", MODULO },
   { "=", ASIGNACION },
   { "+=", MAS_ASIGNACION },
   { "-=", MENOS_ASIGNACION },
   { "*=", POR_ASIGNACION },
   { "/=", ENTRE_ASIGNACION },
   { "%=", MODULO_ASIGNACION },
   { "<", MENOR },
   { "<=", MENOR_IGUAL },
   { ">", MAYOR },
   { ">=", MAYOR_IGUAL },
   { "==", IGUAL },
   { "!=", DIFERENTE },
   { "&", AND },
   { "|", OR },
   { "!", NOT },
   { "#", TAMAÑO },
   { "@", DIRECCION },
   { "'", DESREFERENCIA },
   { "(", PARENTESIS_IZQ },
   { ")", PARENTESIS_DER },
   { "[", CORCHETE_IZQ },
   { "]", CORCHETE_DER },
   { "{", LLAVE_IZQ },
   { "}", LLAVE_DER },
   { ",", COMA },
   { ";", PUNTO_COMA }
};

struct token_anotado {
   token tipo;
   std::string_view pos;

   token_anotado(token t, const char* ini, const char* fin)
   : tipo(t), pos(ini, fin) {
   }
};

token token_id_palabra(const std::string& s) {
   auto iter = mapa.find(s);
   return (iter != mapa.end( ) ? iter->second : IDENTIFICADOR);
}

token token_op_puntuacion(const std::string& s) {
   return mapa.find(s)->second;
}

void esquiva_espacios(const char*& p) {
   while (isspace(*p)) {
      ++p;
   }
}

bool reconoce_entero(const char*& p) {
   if (idigit(*p)) {
      do {
         ++p;
      } while (isdigit(*p));
      return true;
   }
   return false;
}

bool reconoce_id_palabra(const char*& p) {
   if (isalpha(*p) || *p == '_') {
      do {
         ++p;
      } while (isalnum(*p) || *p == '_');
      return true;
   }
   return false;
}

bool reconoce_op_puntuacion(const char*& p) {
   if (op_puntuacion.contains(std::string(p, p + 1))) {
      if (op_puntuacion.contains(std::string(p, p + 2))) {
         p += 2;
         return true;
      }
      p += 1;
      return true;
   }
   return false;
}

std::vector<token_anotado> lexer(const std::string& s) {
   std::vector<token_anotado> res;
   auto ini = s.c_str( ), fin = ini + s.size( );
   while (ini != fin) {
      esquiva_espacios(ini);

      auto temp = ini;
      if (reconoce_entero(ini)) {
         res.emplace_back(LITERAL_ENTERA, temp, ini);
      } else if (reconoce_id_palabra(ini)) {
         res.emplace_back(token_id_palabra(std::string(temp, ini)), temp, ini);
      } else if (reconoce_op_puntuacion(ini)) {
         res.emplace_back(token_op_puntuacion(std::string(temp, ini)), temp, ini);
      }
   }

   return res;
}

#endif
