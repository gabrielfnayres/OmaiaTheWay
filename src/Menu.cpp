#include <iostream>
#include "../includes/Menu.h"


using namespace std;

void Menu:: exibir(){

cout << "<========== BEM-VINDO AO FLAPPY BIRD ==========>" << endl;
cout << "DIGITE C PARA CONTINUAR" << endl;
cout << "DIGITE Q PARA DESISTIR" << endl;

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