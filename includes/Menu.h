#pragma once
#include <iostream>

using namespace std;

class Menu{
public:
void exibir();
void setUsuario(string nomee);
Menu();
char lerResposta();



protected:
string nome;

};