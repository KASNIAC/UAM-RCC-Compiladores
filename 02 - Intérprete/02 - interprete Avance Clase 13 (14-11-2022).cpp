#include <stdlib.h>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

bool reg_match(const std::regex& r, const std::string& s, std::vector<std::string>& v) {
   std::smatch m;
   bool res = std::regex_search(s, m, r);
   v.assign(m.begin( ), m.end( ));
   return res;
}

struct datos_instruccion {
   void* tipo;
   int ri, rj, rk, entero, eti_entero;
   std::string eti_cadena;
};

int main( ) {
   /*std::string linea;
   while (std::getline(std::cin, linea)) {

   }*/

   int ip = 0, r[10] = { };
   std::vector<datos_instruccion> instrucciones = {
      { .tipo = &&nop },
      { .tipo = &&nop },
      { .tipo = &&mov_imm, .ri = 5, .entero = 7 },
      { .tipo = &&show_int, .ri = 5 },
      { .tipo = &&nop },
      { .tipo = &&end }
   };

   despachar: {
      goto *instrucciones[ip++].tipo;
   }
   nop: {
      ;
      goto despachar;
   }
   end: {
      exit(0);
      goto despachar;
   }
   mov_imm: {
      r[instrucciones[ip - 1].ri] = instrucciones[ip - 1].entero;
      goto despachar;
   }
   //...
   show_word: {
      printf("%d", r[instrucciones[ip - 1].ri]);
      goto despachar;
   }

   return 0;
}
