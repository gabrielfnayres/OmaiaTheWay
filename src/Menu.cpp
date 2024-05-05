#include "../includes/Menu.h"
#include <iostream>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"


void exibir(){


putText	(smallImg, "Para iniciar o jogo clique em Y:", Point(300, 50), FONT_HERSHEY_PLAIN, 2, color); // fonte
putText	(smallImg, "Para sair do jogo clique em Q:", Point(200, 50), FONT_HERSHEY_PLAIN, 2, color); // fonte



}