#include <iostream>
#include "../includes/Menu.h"


using namespace std;

void Menu:: exibir(){

cout << "\n<====== FLAPPY BIRD ======>" << endl;
cout << "[C] INICIAR" << endl;
cout << "[Q] SAIR" << endl;
cout << "-> ";
}

char Menu:: lerResposta(){
 char resp;

 cin >> resp;

 return resp;
}

void Menu:: setUsuario(string nomee){
nome = nomee;

}

Menu:: Menu(){

};