#include <stdlib.h> 
#include <iostream> 
#include <regex> 
#include <string> 
#include <vector> 

bool reg_match(const std::regex& r, const std::string& s) {
   return std::regex_search(s, r);
}

bool reg_match(const std::regex& r, const std::string& s, std::vector<std::string>& v) { 
   std::smatch m; 
   bool res = std::regex_search(s, m, r); 
   v.assign(m.begin( ), m.end( )); 
   return res; 
} 

std::string reg_replace(const std::regex& r, const std::string& s, const std::string& t) {
   return std::regex_replace(s, r, t);
}

std::vector<std::string> reg_split(const std::regex& r, const std::string& s) {
   return std::vector<std::string>(std::sregex_token_iterator(s.begin( ), s.end( ), r, -1), std::sregex_token_iterator( ));
}

struct datos_instruccion { 
   void* tipo; 
   int ri, rj, rk, entero, eti_entero; 
   std::string eti_cadena; 
}; 

constexpr auto config = std::regex_constants::ECMAScript|std::regex_constants::icase;

std::regex dos_puntos(R"(:)", config);
std::regex espacios_izq(R"(^\s*)", config);
std::regex espacios_der(R"(\s*$)", config);
std::regex es_eti(R"(^[A-Z_]\w*$)", config);
std::regex es_nop(R"(^NOP$)", config);
std::regex es_end(R"(^END$)", config);
std::regex es_mov_imm(R"(^MOV\s+r(\d)\s*,\s*(\d+)$)", config);
std::regex es_get_dir_byte(R"(^GET\s+r(\d)\s*,\s*BYTE\s*\[\s*(\d+)\s*\]$)", config);
std::regex es_get_dir_word(R"(^GET\s+r(\d)\s*,\s*WORD\s*\[\s*(\d+)\s*\]$)", config);
std::regex es_put_dir_byte(R"(^PUT\s+r(\d)\s*,\s*BYTE\s*\[\s*(\d+)\s*\]$)", config);
std::regex es_put_dir_word(R"(^PUT\s+r(\d)\s*,\s*WORD\s*\[\s*(\d+)\s*\]$)", config);
std::regex es_push(R"(^PUSH\s+r(\d)$)", config);
std::regex es_pop(R"(^POP\s+r(\d)$)", config);
std::regex es_show_byte(R"(^SHOW\s+BYTE\s+r(\d)$)", config);
std::regex es_show_word(R"(^SHOW\s+WORD\s+r(\d)$)", config);

void trim(std::string& s) {
   s = reg_replace(espacios_izq, s, "");
   s = reg_replace(espacios_der, s, "");
}

int main( ) {  
   int ip = 0, reg[10] = { }; 
   datos_instruccion* actual; 
   std::vector<int> pila_datos; 
   std::vector<char> memoria(16777216); 
   std::vector<datos_instruccion> instrucciones; 
 
   std::string linea; 
   while (std::getline(std::cin, linea)) { 
      std::vector<std::string> partes = reg_split(dos_puntos, linea);
      if (partes.size( ) > 2) {
         std::cout << "ERROR\n";
         return 0;
      }
      
      if (partes.size( ) == 2) {
         trim(partes[0]);
         if (reg_match(es_eti, partes[0])) {
            // manejar la etiqueta
            partes[0] = partes[1];
         } else {
            std::cout << "ERROR\n";
            return 0;
         }
      }
      
      trim(partes[0]);
      std::vector<std::string> matches;
      if (reg_match(es_nop, partes[0], matches)) {
         instrucciones.push_back(datos_instruccion{ .tipo = &&nop });
      } else if (reg_match(es_end, partes[0], matches)) {
         instrucciones.push_back(datos_instruccion{ .tipo = &&end });
      } else if (reg_match(es_mov_imm, partes[0], matches)) {
         instrucciones.push_back(datos_instruccion{ .tipo = &&mov_imm, .ri = std::stoi(matches[1]), .entero = std::stoi(matches[2]) });
      } else if (reg_match(es_get_dir_byte, partes[0], matches)) {
         
      } else if (reg_match(es_get_dir_word, partes[0], matches)) {
         
      } else if (reg_match(es_put_dir_byte, partes[0], matches)) {
         
      } else if (reg_match(es_put_dir_word, partes[0], matches)) {
         
      } else if (reg_match(es_push, partes[0], matches)) {
         
      } else if (reg_match(es_pop, partes[0], matches)) {
         
      } else if (reg_match(es_show_byte, partes[0], matches)) {
         
      } else if (reg_match(es_show_word, partes[0], matches)) {
         
      } else {
         std::cout << "ERROR\n";
         return 0;
      }
   }
   
   despachar: { 
      actual = &instrucciones[ip++]; 
      goto *actual->tipo; 
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
      reg[actual->ri] = actual->entero; 
      goto despachar; 
   } 
   //... 
   get_dir_byte: { 
      reg[actual->ri] = *(char*)&memoria[actual->entero]; 
      goto despachar; 
   } 
   get_dir_word: { 
      reg[actual->ri] = *(int*)&memoria[actual->entero]; 
      goto despachar; 
   } 
   //... 
   put_dir_byte: { 
      *(char*)&memoria[actual->entero] = reg[actual->ri]; 
      goto despachar; 
   } 
   put_dir_word: { 
      *(int*)&memoria[actual->entero] = reg[actual->ri]; 
      goto despachar; 
   } 
   //... 
   push: { 
      pila_datos.push_back(reg[actual->ri]); 
      goto despachar; 
   } 
   pop: { 
      reg[actual->ri] = pila_datos.back( ); 
      pila_datos.pop_back( ); 
      goto despachar; 
   } 
   //... 
   show_word: { 
      printf("%d", reg[actual->ri]); 
      goto despachar; 
   } 
   show_byte: { 
      printf("%c", reg[actual->ri]); 
      goto despachar; 
   } 
   return 0; 
}
