#include <regex>
#include<iostream>
#include <string>
int main( ) {
   //Forma fea
   //std::regex r("< *a\\s+href\\s*=\\s*\"(.*?)\"");



    //Row string literals
    std::regex r(R"(<\s*a\s+href\s*=\s*"(.*?)")");



    //Tiene que iniciar u cerrar con alguno de los caracteres básicos de C, en este caso las " O /       //std::cout<<R"Q(/s)ñ";
    std::regex r2(R""(<\s*a\s+href\s*=\s*"(.*?)")"");
    std::regex r3(R"/(<\s*a\s+href\s*=\s*"(.*?)")/");
}
