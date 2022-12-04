// https://omegaup.com/arena/problem/Interprete-de-un-lenguaje-de-baj/
#include <stdlib.h>
#include <iostream>
#include <map>
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
std::regex es_get_ind_byte(R"(^GET\s+r(\d)\s*,\s*BYTE\s*\[\s*r(\d)\s*\]$)", config);
std::regex es_get_ind_word(R"(^GET\s+r(\d)\s*,\s*WORD\s*\[\s*r(\d)\s*\]$)", config);
std::regex es_put_dir_byte(R"(^PUT\s+r(\d)\s*,\s*BYTE\s*\[\s*(\d+)\s*\]$)", config);
std::regex es_put_dir_word(R"(^PUT\s+r(\d)\s*,\s*WORD\s*\[\s*(\d+)\s*\]$)", config);
std::regex es_put_ind_byte(R"(^PUT\s+r(\d)\s*,\s*BYTE\s*\[\s*r(\d)\s*\]$)", config);
std::regex es_put_ind_word(R"(^PUT\s+r(\d)\s*,\s*WORD\s*\[\s*r(\d)\s*\]$)", config);
std::regex es_add(R"(^ADD\s+r(\d)\s*,\s*r(\d)\s*,\s*r(\d)$)", config);
std::regex es_equ(R"(^EQU\s+r(\d)\s*,\s*r(\d)\s*,\s*r(\d)$)", config);
std::regex es_jmp(R"(^JMP\s+([A-Z_]\w*)$)", config);
std::regex es_jif(R"(^JIF\s+r(\d)\s*,\s*([A-Z_]\w*)$)", config);
std::regex es_call(R"(^CALL\s+([A-Z_]\w*)$)", config);
std::regex es_ret(R"(^RET$)", config);
std::regex es_push(R"(^PUSH\s+r(\d)$)", config);
std::regex es_pop(R"(^POP\s+r(\d)$)", config);
std::regex es_show_byte(R"(^SHOW\s+BYTE\s+r(\d)$)", config);
std::regex es_show_word(R"(^SHOW\s+WORD\s+r(\d)$)", config);

void trim(std::string& s) {
   s = reg_replace(espacios_izq, s, "");
   s = reg_replace(espacios_der, s, "");
}

void error(const char* c) {
   std::cout << c << "\n";
   exit(0);
}

int main( ) {
   int ip = 0, reg[10] = { };
   datos_instruccion* actual;
   std::vector<int> pila_datos, pila_ip;
   std::vector<char> memoria(16777216);
   std::vector<datos_instruccion> instrucciones;
   std::map<std::string, int> tabla_eti;

   std::string linea;
   while (std::getline(std::cin, linea)) {
      std::vector<std::string> partes = reg_split(dos_puntos, linea);
      if (partes.size( ) > 2) {
         error("Linea malformada");
      }

      std::for_each(partes.begin( ), partes.end( ), trim);
      if (partes.size( ) == 2) {
         if (reg_match(es_eti, partes[0])) {
            if (tabla_eti.find(partes[0]) != tabla_eti.end( )) {
               error("Etiqueta repetida");
            }
            tabla_eti[partes[0]] = instrucciones.size( );
            if (partes[0] = partes[1]; partes[0] == "") {
               error("Etiqueta sobre instruccion vacia");
            }
         } else {
            error("Etiqueta malformada");
         }
      }

      std::vector<std::string> matches;
      if (reg_match(es_nop, partes[0], matches)) {
         instrucciones.push_back({ .tipo = &&nop });
      } else if (reg_match(es_end, partes[0], matches)) {
         instrucciones.push_back({ .tipo = &&end });
      } else if (reg_match(es_mov_imm, partes[0], matches)) {
         instrucciones.push_back({ .tipo = &&mov_imm, .ri = std::stoi(matches[1]), .entero = std::stoi(matches[2]) });
      } else if (reg_match(es_get_dir_byte, partes[0], matches)) {
         instrucciones.push_back({ .tipo = &&get_dir_byte, .ri = std::stoi(matches[1]), .entero = std::stoi(matches[2]) });
      } else if (reg_match(es_get_dir_word, partes[0], matches)) {
         instrucciones.push_back({ .tipo = &&get_dir_word, .ri = std::stoi(matches[1]), .entero = std::stoi(matches[2]) });
      } else if (reg_match(es_get_ind_byte, partes[0], matches)) {
         instrucciones.push_back({ .tipo = &&get_ind_byte, .ri = std::stoi(matches[1]), .rj = std::stoi(matches[2]) });
      } else if (reg_match(es_get_ind_word, partes[0], matches)) {
         instrucciones.push_back({ .tipo = &&get_ind_word, .ri = std::stoi(matches[1]), .rj = std::stoi(matches[2]) });
      } else if (reg_match(es_put_dir_byte, partes[0], matches)) {
         instrucciones.push_back({ .tipo = &&put_dir_byte, .ri = std::stoi(matches[1]), .entero = std::stoi(matches[2]) });
      } else if (reg_match(es_put_dir_word, partes[0], matches)) {
         instrucciones.push_back({ .tipo = &&put_dir_word, .ri = std::stoi(matches[1]), .entero = std::stoi(matches[2]) });
      } else if (reg_match(es_put_ind_byte, partes[0], matches)) {
         instrucciones.push_back({ .tipo = &&put_ind_byte, .ri = std::stoi(matches[1]), .rj = std::stoi(matches[2]) });
      } else if (reg_match(es_put_ind_word, partes[0], matches)) {
         instrucciones.push_back({ .tipo = &&put_ind_word, .ri = std::stoi(matches[1]), .rj = std::stoi(matches[2]) });
      } else if (reg_match(es_add, partes[0], matches)) {
         instrucciones.push_back({ .tipo = &&add, .ri = std::stoi(matches[1]), .rj = std::stoi(matches[2]), .rk = std::stoi(matches[3]) });
      } else if (reg_match(es_equ, partes[0], matches)) {
         instrucciones.push_back({ .tipo = &&equ, .ri = std::stoi(matches[1]), .rj = std::stoi(matches[2]), .rk = std::stoi(matches[3]) });
      } else if (reg_match(es_jmp, partes[0], matches)) {
         instrucciones.push_back({ .tipo = &&jmp, .eti_cadena = matches[1] });
      } else if (reg_match(es_jif, partes[0], matches)) {
         instrucciones.push_back({ .tipo = &&jif, .ri = std::stoi(matches[1]), .eti_cadena = matches[2] });
      } else if (reg_match(es_call, partes[0], matches)) {
         instrucciones.push_back({ .tipo = &&call, .eti_cadena = std::string(matches[1]) });
      } else if (reg_match(es_ret, partes[0], matches)) {
         instrucciones.push_back({ .tipo = &&ret });
      } else if (reg_match(es_push, partes[0], matches)) {
         instrucciones.push_back({ .tipo = &&push, .ri = std::stoi(matches[1]) });
      } else if (reg_match(es_pop, partes[0], matches)) {
         instrucciones.push_back({ .tipo = &&pop, .ri = std::stoi(matches[1]) });
      } else if (reg_match(es_show_word, partes[0], matches)) {
         instrucciones.push_back({ .tipo = &&show_word, .ri = std::stoi(matches[1]) });
      } else if (reg_match(es_show_byte, partes[0], matches)) {
         instrucciones.push_back({ .tipo = &&show_byte, .ri = std::stoi(matches[1]) });
      } else if (partes[0] != "") {
         error("Instruccion malformada");
      }
   }

   for (auto& actual : instrucciones) {
      if (actual.eti_cadena != "") {
         if (tabla_eti.find(actual.eti_cadena) == tabla_eti.end( )) {
            error("Etiqueta no declarada");
         }
         actual.eti_entero = tabla_eti[actual.eti_cadena];
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
   get_ind_byte: {
      reg[actual->ri] = *(char*)&memoria[reg[actual->rj]];
      goto despachar;
   }
   get_ind_word: {
      reg[actual->ri] = *(int*)&memoria[reg[actual->rj]];
      goto despachar;
   }
   put_dir_byte: {
      *(char*)&memoria[actual->entero] = reg[actual->ri];
      goto despachar;
   }
   put_dir_word: {
      *(int*)&memoria[actual->entero] = reg[actual->ri];
      goto despachar;
   }
   put_ind_byte: {
      *(char*)&memoria[reg[actual->rj]] = reg[actual->ri];
      goto despachar;
   }
   put_ind_word: {
      *(int*)&memoria[reg[actual->rj]] = reg[actual->ri];
      goto despachar;
   }
   add: {
      reg[actual->ri] = reg[actual->rj] + reg[actual->rk];
      goto despachar;
   }
   //...
   equ: {
      reg[actual->ri] = reg[actual->rj] == reg[actual->rk];
      goto despachar;
   }
   //...
   jmp: {
      ip = actual->eti_entero;
      goto despachar;
   }
   jif: {
      if (reg[actual->ri] != 0) {
         ip = actual->eti_entero;
      }
      goto despachar;
   }
   call: {
      pila_ip.push_back(ip);
      ip = actual->eti_entero;
      goto despachar;
   }
   ret: {
      ip = pila_ip.back( );
      pila_ip.pop_back( );
      goto despachar;
   }
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
