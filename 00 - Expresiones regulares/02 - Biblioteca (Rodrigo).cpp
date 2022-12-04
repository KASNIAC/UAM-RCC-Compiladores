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

int main(){

}
