#include <iostream>
#include "../includes/Menu.h"


using namespace std;

void Menu:: exibir(){

cout << "BEM-VINDO AO FLAPPY BIRD";
cout << "DIGITE C PARA CONTINUAR";
cout << "DIGITE Q PARA DESISTIR";


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