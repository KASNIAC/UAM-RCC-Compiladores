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

/*
   Expresión -> Expresión + Término
   Expresión -> Término
   Término -> Término + Unidad
   Término -> Unidad
   Unidad -> número
   Unidad -> ( Expresión )
*/

std::vector<std::string*> expresion(std::string*, std::string*, int);
std::vector<std::string*> termino(std::string*, std::string*, int);
std::vector<std::string*> unidad(std::string*, std::string*, int);

std::vector<std::string*> expresion(std::string* ini, std::string* fin, int prof) {
   if (prof < 0) {
      return { };
   } else {
      std::vector<std::string*> res = termino(ini, fin, prof - 1);
      for (std::string* p : expresion(ini, fin, prof - 1)) {
         if (p != fin && *p == "+") {
            for (std::string* q : termino(p + 1, fin, prof - 1)) {
               res.push_back(q);
            }
         }
      }
      return res;
   }
}

std::vector<std::string*> termino(std::string* ini, std::string* fin, int prof) {
   if (prof < 0) {
      return { };
   } else {
      std::vector<std::string*> res = unidad(ini, fin, prof - 1);
      for (std::string* p : termino(ini, fin, prof - 1)) {
         if (p != fin && *p == "*") {
            for (std::string* q : unidad(p + 1, fin, prof - 1)) {
               res.push_back(q);
            }
         }
      }
      return res;
   }
}

std::vector<std::string*> unidad(std::string* ini, std::string* fin, int prof) {
   static std::regex r(R"(^\d+$)");
   if (prof < 0) {
      return { };
   } else if (ini != fin && reg_match(r, *ini)) {
      return { ini + 1 };
   } else if (ini != fin && *ini == "(") {
      std::vector<std::string*> res;
      for (std::string* p : expresion(ini + 1, fin, prof - 1)) {
         if (p != fin && *p == ")") {
            res.push_back(p + 1);
         }
      }
      return res;
   } else {
      return { };
   }
}

int main( ) {
   std::string s = " 1+ 2 * ((3 +51)  * (6 + 7)) + ((8 * 9) + (0 * 1) ) ";
   std::vector<std::string> tokens = reg_split(std::regex(R"(\s+)"), reg_replace(std::regex(R"/(\+|\*|\(|\))/"), s, " $00 "));
   tokens.erase(std::remove(tokens.begin( ), tokens.end( ), ""), tokens.end( ));

   for (std::string* tope : expresion(&tokens[0], &tokens[0] + tokens.size( ), 20)) {
      for (std::string* p = &tokens[0]; p != tope; ++p) {
         std::cout << *p << " ";
      }
      std::cout << "\n";
   }
}
