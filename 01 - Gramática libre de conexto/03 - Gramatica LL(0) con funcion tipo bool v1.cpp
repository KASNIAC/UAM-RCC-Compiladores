/* 
   Expresión -> número Cexpr 
   Expresión -> ( Expresión ) Cexpr 
   Cexpr -> 
   Cexpr -> + Expresión 
   Cexpr -> * Expresión 
*/ 
 
#include <algorithm> 
#include <iostream> 
#include <regex> 
#include <string> 
#include <vector> 
 
bool reg_match(const std::regex& r, const std::string& s) { 
   return std::regex_search(s, r); 
} 
 
std::string reg_replace(const std::regex& r, const std::string& s, const std::string& t) { 
   return std::regex_replace(s, r, t); 
} 
 
std::vector<std::string> reg_split(const std::regex& r, const std::string& s) { 
   return std::vector<std::string>(std::sregex_token_iterator(s.begin( ), s.end( ), r, -1), std::sregex_token_iterator( )); 
} 
 
std::string* expresion(std::string*, std::string*); 
std::string* cexpr(std::string*, std::string*); 
 
std::string* expresion(std::string* ini, std::string* fin) { 
   static std::regex r(R"(^\d+$)"); 
 
   if (ini == fin) { 
      throw std::logic_error("CADENA MAL FORMADA"); 
   } else if (reg_match(r, *ini)) { 
      return cexpr(ini + 1, fin); 
   } else if (*ini == "(") { 
      std::string* p = expresion(ini + 1, fin); 
      if (p == fin || *p != ")") { 
         throw std::logic_error("CADENA MAL FORMADA"); 
      } 
      return cexpr(p + 1, fin); 
   } else { 
      throw std::logic_error("CADENA MAL FORMADA"); 
   } 
} 
 
std::string* cexpr(std::string* ini, std::string* fin) { 
   if (ini == fin) { 
      return ini; 
   } else if (*ini == "+" || *ini == "*") { 
      return expresion(ini + 1, fin); 
   } else { 
      return ini; 
   } 
} 
 
int main( ) try { 
   std::string s = " 1+ 2 * ((3 +51)  * (6 + 7)) + ((8 * 9) + (0 * 1) ) "; 
   std::vector<std::string> tokens = reg_split(std::regex(R"(\s+)"), reg_replace(std::regex(R"/(\+|\*|\(|\))/"), s, " $00 ")); 
   tokens.erase(std::remove(tokens.begin( ), tokens.end( ), ""), tokens.end( )); 
 
   std::string* tope = expresion(&tokens[0], &tokens[0] + tokens.size( )); 
   if (tope == &tokens[0] + tokens.size( )) { 
      std::cout << "OK\n"; 
   } else { 
      std::cout << "MAL\n"; 
   } 
 
   for (std::string* p = &tokens[0]; p < tope; ++p) { 
      std::cout << *p; 
   } 
} catch (const std::exception& ex) { 
   std::cout << ex.what( ); 
}