//#define DEBUG
//PARA LA TAREA 4: Eliminar comentarios si quiero usar el servicio web de Rodrigod

/*
#ifdef DEBUG
   #include "debug.h"
#endif
*/
#include "debug.h"
#include "error.h"
#include "lexer.h"
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
      for (const token_anotado& actual : tokens) {
         std::cout << actual.tipo;
         if (actual.tipo == IDENTIFICADOR || actual.tipo == LITERAL_ENTERA || actual.tipo == LITERAL_CADENA) {
            std::cout << " " << actual.vista;
         }
         std::cout << "\n";
      }
   } catch (const error& e) {
      auto [linea, columna] = linea_columna(entrada, e.vista);
      std::cout << "ERROR " << linea + 1 << ":" << columna + 1 << "\n";
      std::cout << e.mensaje << "\n"; //Comentarlo para la tarea
   }
}
