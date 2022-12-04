#define DEBUG

#ifdef DEBUG
   #include "debug.h"
#endif
#include "lexer.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, const char* argv[]) {
   if (argc < 2) {
      std::cerr << "Uso: " << argv[0] << " archivo\n";
      return 0;
   }

   std::ifstream arch(argv[1]);
   if (!arch.is_open( )) {
      std::cerr << "No se pudo abrir " << argv[1] << "\n";
      return 0;
   }

   std::stringstream bufer;
   bufer << arch.rdbuf( );
   std::string contenido = std::move(bufer).str( );
   #ifdef DEBUG
      std::cerr << "Leimos " << contenido << "\n";
   #endif

   std::vector<token_anotado> tokens = lexer(contenido);
   #ifdef DEBUG
      for (const token_anotado& actual : tokens) {
         std::cerr << actual << "\n";
      }
   #endif
}
