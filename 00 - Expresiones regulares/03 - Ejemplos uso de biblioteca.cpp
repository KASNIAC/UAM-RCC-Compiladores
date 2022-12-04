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

bool reg_match_all(const std::regex& r, const std::string& s, std::vector<std::vector<std::string>>& v) {
   for (std::sregex_iterator ri(s.begin( ), s.end( ), r), rf; ri != rf; ++ri) {
      v.push_back(std::vector<std::string>(ri->begin( ), ri->end( )));
   }
   return !v.empty( );
}

std::string reg_replace(const std::regex& r, const std::string& s, const std::string& t) {
   return std::regex_replace(s, r, t);
}

template<typename F>     //Para usarlo con C, iría F f (en lúgar de auto).
std::string reg_replace_callback(const std::regex& r, const std::string& s, F f) {
   std::vector<std::pair<std::string::const_iterator, std::string::const_iterator>> reemplazos;
   for (std::sregex_iterator ri(s.begin( ), s.end( ), r), rf; ri != rf; ++ri) {
      reemplazos.emplace_back((*ri)[0].first, (*ri)[0].second);
   }

   std::string res = s;
   for (auto it = reemplazos.rbegin( ); it != reemplazos.rend( ); ++it) {
      res.replace(res.begin( ) + (it->first - s.begin( )), res.begin( ) + (it->second - s.begin( )), f(std::string(it->first, it->second)));
   }
   return res;
}

std::vector<std::string> reg_split(const std::regex& r, const std::string& s) {
   return std::vector<std::string>(std::sregex_token_iterator(s.begin( ), s.end( ), r, -1), std::sregex_token_iterator( ));
}

int main( ) {
   ejemplo1: {
      std::regex r(R"/(ab)/");
      std::cout << reg_match(r, ":) abigail :)") << "\n";
      std::cout << reg_match(r, "gatito bonito") << "\n";
   }
   std::cout << "------------\n";

   ejemplo2: {
      //std::regex r(R"/(hola \w+ \w+ adios)/");
      std::regex r(R"/(hola (\w+) (\w+) adios)/");  //Usamos los grupos de captura (lo que nos interesa de la expresión)
      std::vector<std::string> matches;
      reg_match(r, "xx hola gatito perrito adios xx", matches);
      for (int i = 0; i < matches.size( ); ++i) {
         std::cout << i << ": " << matches[i] << "\n";
      }
   }
   std::cout << "------------\n";

   //Ejemplo para comprobar que solo toma la primera coincidencia
   ejemplo2_1: {
      std::regex r(R"/(hola (\w+) (\w+) adios)/");
      std::vector<std::string> matches;
      reg_match(r, "xx hola gatito perrito adios hola cotito paloma adios xx", matches);
      for (int i = 0; i < matches.size( ); ++i) {
         std::cout << i << ": " << matches[i] << "\n";
      }
   }
   std::cout << "------------\n";

   ejemplo3: {
      std::regex r(R"/(hola (\w+) (\w+) adios)/");
      std::vector<std::vector<std::string>> matches;
      reg_match_all(r, "xx hola gatito perrito adios hola cotito paloma adios xx", matches);
      for (int i = 0; i < matches.size( ); ++i) {
         std::cout << "subcadena " << i << ":\n";
         for (int j = 0; j < matches[i].size( ); ++j) {
            std::cout << "   " << j << ": " << matches[i][j] << "\n";
         }
      }
   }
   std::cout << "------------\n";

    //"que" se reemplaza por "qx" porque eso hizo coincidir.
   ejemplo4: {
      std::regex r(R"/([aeiou]+)/"); //Intentar cambiar + por *
      std::string s = reg_replace(r, "hola a todos los que estan aqui", "x");
      std::cout << s << "\n";
   }
   std::cout << "------------\n";

   ejemplo5_0: {
      std::regex r(R"/([aeiou]+)/");
      std::string s = reg_replace_callback(
         r,
         "hola a todos los que estan aqui",
         [](const std::string& encontrada) {
            if (encontrada.size( ) > 1) {
               return "x";
            } else if (encontrada == "a") {
               return "@";
            } else if (encontrada == "i") {
               return "#";
            } else {
               return "";
            }
         }
      );
      std::cout << s << "\n";
   }
   std::cout << "------------\n";

   ejemplo5: {
      std::regex r(R"/((\d+)\s+.*)/");
      std::string s = reg_replace(r, "203305906 CASTRO RODRIGO", R"/(al$01@azc.uam.mx)/");
      std::cout << s << "\n";
   }
   std::cout << "------------\n";

   ejemplo6: {
      std::regex r(R"/([aeiou]+)/");
      std::string s = reg_replace_callback(r, "hola a todos los que estan aqui", [](const std::string& encontrada) {
         if (encontrada.size( ) > 1) {
            return "x";
         } else if (encontrada == "a") {
            return "@";
         } else if (encontrada == "i") {
            return "#";
         } else {
            return "";
         }
      });
      std::cout << s << "\n";
   }
   std::cout << "------------\n";

   ejemplo7: {
      std::regex r(R"/(\s*,\s*)/");
      std::vector<std::string> v = reg_split(r, "hola,perrito    ,    gatito   ,  adios");
      for (int i = 0; i < v.size( ); ++i) {
         std::cout << v[i] << "\n";
      }
   }
   std::cout << "------------\n";

}


/*
//---------------------------------------Código de ejemplo by DARK-MX ---------------------------------------------------------------
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

bool reg_match_all(const std::regex& r, const std::string& s, std::vector<std::vector<std::string>>& v) {
   for (std::sregex_iterator ri(s.begin( ), s.end( ), r), rf; ri != rf; ++ri) {
      v.push_back(std::vector<std::string>(ri->begin( ), ri->end( )));
   }
   return !v.empty( );
}

std::string reg_replace(const std::regex& r, const std::string& s, const std::string& t) {
   return std::regex_replace(s, r, t);
}

template<typename F>
std::string reg_replace_callback(const std::regex& r, const std::string& s, F f) {
   std::vector<std::pair<std::string::const_iterator, std::string::const_iterator>> reemplazos;
   for (std::sregex_iterator ri(s.begin( ), s.end( ), r), rf; ri != rf; ++ri) {
      reemplazos.emplace_back((*ri)[0].first, (*ri)[0].second);
   }

   std::string res = s;
   for (auto it = reemplazos.rbegin( ); it != reemplazos.rend( ); ++it) {
      res.replace(res.begin( ) + (it->first - s.begin( )), res.begin( ) + (it->second - s.begin( )), f(std::string(it->first, it->second)));
   }
   return res;
}

std::vector<std::string> reg_split(const std::regex& r, const std::string& s) {
   return std::vector<std::string>(std::sregex_token_iterator(s.begin( ), s.end( ), r, -1), std::sregex_token_iterator( ));
}

int main( ) {
   ejemplo1: {
      std::regex r(R"/(ab)/");
      std::cout << reg_match(r, ":) abigail :)") << "\n";
      std::cout << reg_match(r, "gatito bonito") << "\n";
   }
   std::cout << "------------\n";

   ejemplo2: {
      std::regex r(R"/(hola (\w+) (\w+) adios)/");
      std::vector<std::string> matches;
      reg_match(r, "xx hola gatito perrito adios hola cotito paloma adios xx", matches);
      for (int i = 0; i < matches.size( ); ++i) {
         std::cout << i << ": " << matches[i] << "\n";
      }
   }
   std::cout << "------------\n";

   ejemplo3: {
      std::regex r(R"/(hola (\w+) (\w+) adios)/");
      std::vector<std::vector<std::string>> matches;
      reg_match_all(r, "xx hola gatito perrito adios hola cotito paloma adios xx", matches);
      for (int i = 0; i < matches.size( ); ++i) {
         std::cout << "subcadena " << i << ":\n";
         for (int j = 0; j < matches[i].size( ); ++j) {
            std::cout << "   " << j << ": " << matches[i][j] << "\n";
         }
      }
   }
   std::cout << "------------\n";

   ejemplo4: {
      std::regex r(R"/([aeiou]+)/");
      std::string s = reg_replace(r, "hola a todos los que estan aqui", "x");
      std::cout << s << "\n";
   }
   std::cout << "------------\n";

   ejemplo5: {
      std::regex r(R"/((\d+)\s+.*)/");
      std::string s = reg_replace(r, "203305906 CASTRO RODRIGO", R"/(al$01@azc.uam.mx)/");
      std::cout << s << "\n";
   }
   std::cout << "------------\n";

   ejemplo6: {
      std::regex r(R"/([aeiou]+)/");
      std::string s = reg_replace_callback(r, "hola a todos los que estan aqui", [](const std::string& encontrada) {
         if (encontrada.size( ) > 1) {
            return "x";
         } else if (encontrada == "a") {
            return "@";
         } else if (encontrada == "i") {
            return "#";
         } else {
            return "";
         }
      });
      std::cout << s << "\n";
   }
   std::cout << "------------\n";

   ejemplo7: {
      std::regex r(R"/(\s*,\s*)/");
      std::vector<std::string> v = reg_split(r, "hola,perrito    ,    gatito   ,  adios");
      for (int i = 0; i < v.size( ); ++i) {
         std::cout << v[i] << "\n";
      }
   }
   std::cout << "------------\n";
}

*/
