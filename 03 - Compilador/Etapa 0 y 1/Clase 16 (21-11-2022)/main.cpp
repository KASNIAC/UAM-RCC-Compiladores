#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#define DEBUG

int main(int argc, const char* argv[]) {
    //----------------------ETAPA 0---------------------------------------------------------------------------------------------------------
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
   //contenido.push_back('\0'); //Un carácter nulo NO debería de aparecer en el archivo, pero string solito

   #ifdef DEBUG
      std::cerr << "Leimos " << contenido << "\n";
   #endif
   //----------------------------------------------------------------------------------------------------------------------------------------
}
