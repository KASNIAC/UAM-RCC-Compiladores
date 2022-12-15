//#define DEBUG

#include "debug.h"
#include "error.h"
#include "lexer.h"
#include "parser.h"
#include "semantico.h"
#include <iostream>
#include <sstream>
#include <string>

int main( ) {
   std::stringstream bufer;
   bufer << std::cin.rdbuf( );
   std::string entrada = std::move(bufer).str( );
   #ifdef DEBUG
      std::cerr << "Leimos " << entrada << "\n";
   #endif

   try {
      std::vector<token_anotado> tokens = lexer(entrada);
      #ifdef DEBUG
         for (const token_anotado& actual : tokens) {
            std::cerr << actual << "\n";
         }
      #endif

      arbol_sintactico a = parser(tokens);
      #ifdef DEBUG
         for (const auto& funcion : a.funciones) {
            std::cout << funcion << "\n";
         }
      #endif

      tabla_simbolos t = semantico(a);
   } catch (const error& e) {
      auto [linea, columna] = linea_columna(entrada, e.vista);
      std::cout << "ERROR " << linea + 1 << ":" << columna + 1 << "\n";
      std::cout << e.mensaje << "\n";
   }
}
