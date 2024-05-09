#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
#include "../includes/Menu.h"



using namespace std;
using namespace cv;

unsigned int microsec = 1000000;
float velo = 1;
char r;
int t = 0;
float x = 600;
float x1 = 600;
float x2 = 600;
float x3 = 600;

int y_up = 0;

int y_small_down = 240;
int y_mid_down = 269;
int y_down = 240;

int c = 0;
int pont = 0;
fstream stream;
string texto;
string novotexto;
int pontos;
Menu menu;
char resp;
string nome;
string numeracao;
string usuarioTopo;




void playlose(){
    system("mplayer ../data/hit.wav >/dev/null 2>&1 &"); 
}

void playwins(){
    system("mplayer ../data/point.wav >/dev/null 2>&1 &"); 
}

int detectAndDraw( Mat& img, CascadeClassifier& cascade, double scale, bool tryflip);

string cascadeName;

int main( int argc, const char** argv )
{
    VideoCapture capture;
    Mat frame;
    bool tryflip;
    CascadeClassifier cascade;
    double scale;

    stream.open("records.txt", ios_base::in);

    if(!stream.is_open( )){
     
    cout<<"Não foi possível abrir arquivo";
    return 0;
    }

    getline(stream, texto);

      size_t pos = texto.find_first_of(' ');

    if (pos != string::npos) {
     usuarioTopo = texto.substr(0, pos);  
     numeracao = texto.substr(pos + 1);

    }

      stream.close();
    
    

    pontos = stoi(numeracao); 

    cascadeName = "haarcascade_frontalface_default.xml";
    scale = 1; // usar 1, 2, 4.
    if (scale < 1)
        scale = 1;
    tryflip = true;

    if (!cascade.load(cascadeName)) {
        cerr << "ERROR: Could not load classifier cascade: " << cascadeName << endl;
        return -1;
    }

    if(!capture.open(0)) // para testar com um video
    //if(!capture.open(0)) // para testar com a webcam
    {
        cout << "Capture from camera #0 didn't work" << endl;
        return 1;
    }

    
    
    menu.exibir();
    resp = menu.lerResposta();
    if(resp != 99 && resp !=67){

        //break;

    }
    if(resp == 99 || resp == 67){
     cout<< "Digite o nome do seu usuário";
     cin >> nome;
     menu.setUsuario(nome);
     

    if( capture.isOpened() ) {
        cout << "Video capturing has been started ..." << endl;

        while (1)
        {
            capture >> frame;
            if( frame.empty() )
                break;

            t = detectAndDraw( frame, cascade, scale, tryflip );
            cout << "Pontuacao: "<< pont << endl;
            if(pont > pontos){

                stream.open("records.txt", ios_base::out);
                novotexto = nome + " " + to_string(pont);
                stream << novotexto;
                stream.close();
            }
            if(t == 0)
                break;

            char c = (char)waitKey(10);
            if( c == 27 || c == 'q' || c == 'Q' )
                break;
        }
    }

    }
    
     

    return 0;
}

/**
 * @brief Draws a transparent image over a frame Mat.
 * 
 * @param frame the frame where the transparent image will be drawn
 * @param transp the Mat image with transparency, read from a PNG image, with the IMREAD_UNCHANGED flag
 * @param xPos x position of the frame image where the image will start.
 * @param yPos y position of the frame image where the image will start.
 */
void drawTransparency(Mat frame, Mat transp, int xPos, int yPos) {
    Mat mask;
    vector<Mat> layers;

    split(transp, layers); // seperate channels
    Mat rgb[3] = { layers[0],layers[1],layers[2] };
    mask = layers[3]; // png's alpha channel used as mask
    merge(rgb, 3, transp);  // put together the RGB channels, now transp insn't transparent 
    transp.copyTo(frame.rowRange(yPos, yPos + transp.rows).colRange(xPos, xPos + transp.cols), mask);
}

/**
 * @brief Draws a transparent rect over a frame Mat.
 * 
 * @param frame the frame where the transparent image will be drawn
 * @param color the color of the rect
 * @param alpha transparence level. 0 is 100% transparent, 1 is opaque.
 * @param regin rect region where the should be positioned
 */
void drawTransRect(Mat frame, Scalar color, double alpha, Rect region) {
    Mat roi = frame(region);
    Mat rectImg(roi.size(), CV_8UC3, color); 
    addWeighted(rectImg, alpha, roi, 1.0 - alpha , 0, roi); 
}

int detectAndDraw( Mat& img, CascadeClassifier& cascade, double scale, bool tryflip)
{
    double t = 0;
    vector<Rect> faces;
    Mat gray, smallImg;
    Scalar color = Scalar(255,0,0);

    double fx = 1 / scale;
    resize( img, smallImg, Size(), fx, fx, INTER_LINEAR_EXACT );
    if( tryflip )
        flip(smallImg, smallImg, 1);
    cvtColor( smallImg, gray, COLOR_BGR2GRAY );
    equalizeHist( gray, gray );

    t = (double)getTickCount();

    cascade.detectMultiScale( gray, faces,
        1.3, 2, 0
        //|CASCADE_FIND_BIGGEST_OBJECT
        //|CASCADE_DO_ROUGH_SEARCH
        |CASCADE_SCALE_IMAGE,
        Size(40, 40) );
    t = (double)getTickCount() - t;
    printf( "detection time = %g ms\n", t*1000/getTickFrequency());

    // Desenha uma imagem
    Mat pipe1 = cv::imread("../data/pipe_original_medio.png", IMREAD_UNCHANGED);
    Rect pipeRect1 = Rect((x-=2*velo), y_mid_down, pipe1.cols, pipe1.rows);
    drawTransparency(smallImg, pipe1, x, y_mid_down);

    Mat pipe2 = cv::imread("../data/pipe_original.png", IMREAD_UNCHANGED);
    Rect pipeRect2 = Rect(x1, y_up, pipe2.cols, pipe2.rows);

    Mat pipe3 = cv::imread("../data/pipe_original.png", IMREAD_UNCHANGED);
    Rect pipeRect3 = Rect(x2, y_down, pipe3.cols, pipe3.rows);

    Mat pipe4 = cv::imread("../data/pipe_original.png", IMREAD_UNCHANGED);
    Rect pipeRect4 = Rect(x3, y_up, pipe4.cols, pipe4.rows);


    Mat flappy = cv::imread("../data/bird_sprite(1).png");


    c++;
    cout << endl << "c = " << c << endl;
    
    if(c > 75)
    {
        x1-=2*velo;
        drawTransparency(smallImg, pipe2, x1, y_up);
        printf("pipe::width: %d, height=%d\n", pipe2.cols, pipe2.rows);
        drawTransparency(smallImg, pipe2, x1, y_up);
        printf("pipe::width: %d, height=%d\n", pipe2.cols, pipe2.rows);
    }

    if(c > 150)
    {
        x2-=2*velo;
        drawTransparency(smallImg, pipe3, x2, y_down);
        printf("pipe::width: %d, height=%d\n", pipe3.cols, pipe3.rows);
    }

    if(c > 225)
    {
        x3-=2*velo;
        drawTransparency(smallImg, pipe4, x3, y_up);
        printf("pipe::width: %d, height=%d\n", pipe4.cols, pipe4.rows);
    }

    if(c % 100 == 0)
        velo+=0.1;

    if(x < 10)
    {
        x = 600;
        pont++;
        playwins();
    }   
    if(x1 < 15)
    {
        x1 = 600;
        pont++;
        playwins();
    } 
    if(x2 < 15)
    {
        x2 = 600;
        pont++;
        playwins();
    } 
    if(x3 < 15)
    {
        x3 = 600;
        pont++;
        playwins();
    } 

    for ( size_t i = 0; i < faces.size(); i++ )
    {
        Rect r = faces[i];


        //cout << "Rectangle - P1: " << Point(cvRound(r.x+60), cvRound(r.y+60)) << endl << endl;
        //cout << "Rectangle - P2: " << Point(cvRound(r.x+110), cvRound(r.y+110)) << endl << endl;

        //drawTransparency(smallImg, flappy, cvRound(r.x+85),cvRound(r.y+85));


        Rect fac = Rect(cvRound(r.x+(r.width/2) - 25),cvRound(r.y+ (r.height/2) - 25), 50, 50);

        if(((fac & pipeRect1).area() > 1)||((fac & pipeRect2).area() > 1)||((fac & pipeRect3).area() > 1)||((fac & pipeRect4).area() > 1))
        {
            color = Scalar(0,0,255);
            
            putText	(smallImg, "GAME OVER", Point(200, 200), FONT_HERSHEY_PLAIN, 3, Scalar(255,255,255));
            playlose();
          
            //usleep(3*microsec);
        }   

        else
        {
            color = Scalar(255,0,0);
        }
        //rectangle( smallImg, Point(cvRound(r.x+60), cvRound(r.y+60)), Point(cvRound((r.x + 110)), cvRound((r.y + 110))), color, 3);
        rectangle( smallImg, Point(cvRound(fac.x), cvRound(fac.y)), Point(cvRound((fac.x + fac.width)), cvRound((fac.y + fac.height))), color, 3);


        if(cvRound(r.x + 85) < 300 && cvRound(r.y + 85) < 300){
          drawTransparency(smallImg, flappy, cvRound(r.x+(r.width/2) - 25),cvRound(r.y+ (r.height/2) - 25));
        }

    }


    putText	(smallImg, to_string(pont), Point(320, 50), FONT_HERSHEY_PLAIN,3, Scalar(255,255,255)); // fonte

    imshow("result", smallImg );
    printf("image::width: %d, height=%d\n", smallImg.cols, smallImg.rows );

    return 1;
}