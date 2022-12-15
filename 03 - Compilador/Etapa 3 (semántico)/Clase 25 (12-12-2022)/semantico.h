#ifndef SEMANTICO_H
#define SEMANTICO_H

#include "error.h"
#include "lexer.h"
#include "parser.h"
#include <map>
#include <string_view>
#include <vector>

enum modificador_tipo {
   VALOR, APUNTADOR, ARREGLO
};

enum tiempo_vida {
   VARIABLE, TEMPORAL
};

struct info_tipo {
   token tipo_base;
   modificador_tipo modificador;
   tiempo_vida vida;
   int tamanyo;
};

struct info_funcion {
   const declaracion_funcion& decl;
   info_tipo tipo;
   std::vector<info_tipo> parametros;
   std::map<std::string_view, int> tabla_parametros;
   //... :)-|-< adiós
};

struct tabla_simbolos {
   //...
};

tabla_simbolos semantico(const arbol_sintactico& a) {
   //...
}

#endif
