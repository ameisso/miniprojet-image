#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
<<<<<<< HEAD
//#include "highgui.h"//pour les fader
#include "opencv2/core/core.hpp"
#include "opencv2/objdetect/objdetect.hpp"
=======
>>>>>>> 1dcbd9796f147bdd7e64ef0fbbc42476f94269f6

#include <cstdio>
#include <iostream>

using namespace cv;
using namespace std;
<<<<<<< HEAD

int main()
{
    Mat img0,img, img2;
    cout<<"ca marche"<<endl;
=======
//---------------------------------------------------------------------------------------------
void tresh(double seuil);//binarise l'image
void sub(Mat img1, Mat img2); //soustraire deux images
//---------------------------------------------------------------------------------------------
static void help()
{
    cout << "programme qui charge les images et fait les traitements dont on a besoin"<< endl;
}
Mat img, img2;
Mat imgTresh;//image seuillée



int main( int argc, char** argv )
{
    Mat img0 = imread("data/detection_0000.jpeg", 1);
    if( img0.empty() )
    {
        cout << "Couldn'g open image "<<endl;
        return 0;
    }
    img0.copyTo(img);
    imshow("originale",img0);
    Mat img1 = imread("data/detection_0200.jpeg", 1);
    if( img1.empty() )
    {
        cout << "Couldn'g open image 2"<<endl;
        return 0;
    }
    img0.copyTo(img2);
    imshow("originale (deuxième)",img1);
    help();





    for(;;)
    {
        int c = waitKey(0);

        if( (char)c == 27 )//touche echap
            break;


        if( (char)c == 't' || (char)c == ' ' )
        {
            tresh(50);
        }
        if( (char)c == 's' || (char)c == ' ' )
        {
            sub(img2,img);
        }
    }

    return 0;
}
void tresh (double seuil)
{
    cout<<"binarisation d l'image"<<endl;
    cvtColor(img, img, CV_BGR2GRAY);//on converti l'image en niveaux de gris  sinon on a une image en 16 couleurs...
    threshold(img,imgTresh,seuil,255,THRESH_BINARY);//le seuil max est fixé ici à 255, mais o peut le changer
    imshow("binarize",imgTresh);
}
void sub(Mat img1, Mat img2)
{
    Mat sub;
    cvAbsDiff(img1,img2,sub);
    imshow("sub",sub);
>>>>>>> 1dcbd9796f147bdd7e64ef0fbbc42476f94269f6
}
