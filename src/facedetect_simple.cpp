#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace cv;

char r;
int t = 0;
int x = 600;
int x1 = 600;
int x2 = 600;
int x3 = 600;
int y_down = 240;
int y_up = 0;
int c = 0;
int pont = 0;

int detectAndDraw( Mat& img, CascadeClassifier& cascade, double scale, bool tryflip);

string cascadeName;

int main( int argc, const char** argv )
{
    VideoCapture capture;
    Mat frame;
    bool tryflip;
    CascadeClassifier cascade;
    double scale;

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

    if( capture.isOpened() ) {
        cout << "Video capturing has been started ..." << endl;

        while (1)
        {
            capture >> frame;
            if( frame.empty() )
                break;

            t = detectAndDraw( frame, cascade, scale, tryflip );
            if(t == 0)
                break;

            char c = (char)waitKey(10);
            if( c == 27 || c == 'q' || c == 'Q' )
                break;
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
    Mat orange = cv::imread("../data/pipe_original.png", IMREAD_UNCHANGED);
    Rect orangeRect = Rect(y_down, (x-=2), orange.cols, orange.rows);
    drawTransparency(smallImg, orange, x, y_down);
    printf("orang::width: %d, height=%d\n", orange.cols, orange.rows);
    c++;
    //cout << "valor de x: " << x << endl;
    //cout << "valor de c: " << c << endl;
    
    if(c > 75)
    {
        Mat pipe = cv::imread("../data/pipe_original.png", IMREAD_UNCHANGED);
        Rect pipeRect = Rect(y_up, (x1-=2), pipe.cols, pipe.rows);
        drawTransparency(smallImg, pipe, x1, y_up);
        printf("pipe::width: %d, height=%d\n", pipe.cols, pipe.rows);
    }

    if(c > 150)
    {
        Mat pipe = cv::imread("../data/pipe_original.png", IMREAD_UNCHANGED);
        Rect pipeRect = Rect(y_down, (x2-=2), pipe.cols, pipe.rows);
        drawTransparency(smallImg, pipe, x2, y_down);
        printf("pipe::width: %d, height=%d\n", pipe.cols, pipe.rows);
    }

    if(c > 225)
    {
        Mat pipe = cv::imread("../data/pipe_original.png", IMREAD_UNCHANGED);
        Rect pipeRect = Rect(y_up, (x3-=2), pipe.cols, pipe.rows);
        drawTransparency(smallImg, pipe, x3, y_up);
        printf("pipe::width: %d, height=%d\n", pipe.cols, pipe.rows);
    }

    if(x == 0)
    {
        x = 600;
        pont++;
    }   
    if(x1 == 0)
    {
        x1 = 600;
        pont++;
    } 
    if(x2 == 0)
    {
        x2 = 600;
        pont++;
    } 
    if(x3 == 0)
    {
        x3 = 600;
        pont++;
    } 

    for ( size_t i = 0; i < faces.size(); i++ )
    {
        Rect r = faces[i];
        if((r & orangeRect).area() > 10)
        {
            color = Scalar(0,0,255);
        }   
        else
        {   
            color = Scalar(255,0,0);
            cout << "VOCÊ PERDEU!" << endl;
            putText	(smallImg, "GAME OVER", Point(280, 200), FONT_HERSHEY_PLAIN,7, Scalar(255,255,255));
            return 0;
        }
        rectangle( smallImg, Point(cvRound(r.x), cvRound(r.y)), Point(cvRound((r.x + r.width-1)), cvRound((r.y + r.height-1))), color, 3);
    }
    //cout << "Valor de pont: " << pont << endl;
    putText	(smallImg, to_string(pont), Point(320, 50), FONT_HERSHEY_PLAIN,3, Scalar(255,255,255)); // fonte

    /* Desenha quadrados com transparencia
    double alpha = 0.3;
    drawTransRect(smallImg, Scalar(0,255,0), alpha, Rect(  0, 0, 200, 200));
    drawTransRect(smallImg, Scalar(255,0,0), alpha, Rect(200, 0, 200, 200));*/

    // Desenha um texto
    //color = Scalar(0,0,255);
    //putText	(smallImg, "Placar:", Point(300, 50), FONT_HERSHEY_PLAIN, 2, color); // fonte

    // Desenha o frame na tela
    imshow("result", smallImg );
    printf("image::width: %d, height=%d\n", smallImg.cols, smallImg.rows );

    return 1;
}