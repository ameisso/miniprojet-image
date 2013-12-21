#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <cstdio>
#include <iostream>
#include "bloc.h"
#include "ligne.h"
using namespace cv;
using namespace std;
//*************************************************************************
//Variables
int imageIndex=0; //index de l'image que l'on est en train de traiter.
int studiedLine=100;//numéro de la ligne de matrice que l'on étudie
int studiedLineWidth=100;//nombre de lignes vers le bas par rapport à studiedLine que l'on prend en compte.

std::string path;//répertoire de travail
Size extractedLineNoBackgroundSize;//taille de la matrice étudiée
//image de référence qui sert à extraire le fond...
//pour le moment c'est toujours la même, ensuite, on pourra toujours la modifier avec les autres images.
Mat refImg=imread("data/detection_0000.jpeg");
Mat currentImg;//image que l'on est en train d'analyser
Mat extractedLine;//ligne extraite de l'image courante.
Mat backgroundLine; //correspond aux lignes de l'image de fond que l'on étudie.
Mat extractedLineNoBackground;//matrice de l'image sans fond.
CvCapture* capture;//pour brancher une caméra (j'ai fait un test pour voir si il n'y avait pas de problèmes de buffer...)
Mat test=Mat::eye(4, 4, CV_64F);//matrice de test des blocs.
//bloc theBloc;
//*************************************************************************
//Déclarations des fonctions
bool checkIfNewImage(int lastImageIndex);
string toString(int val);
void extractLine(Mat img,Mat &extractedLine,int lineNumber, int lineWidth);
void substractBackground(Mat refImg, Mat CurrentImg, Mat &OutputImg);
void CallBackFunc(int event, int x, int y, int flags, void* userdata);

//*************************************************************************

int main()
{

    //on vérifie que l'image de fond existe (benêt, benêt, benêt!!!)
    if(refImg.empty() )
    {
        cout << "Couldn't open background image "<<endl;
        return false;
    }
    imshow("image de fond",refImg);
    cvWaitKey(1000);//on affiche l'image de fond 1s pour vérifier

  /*
   namedWindow("My Window", 1);  //Creer une fenetre
   setMouseCallback("My Window", CallBackFunc, NULL); //settter la function de callback pour tout eveniment du sourie
   imshow("My Window", refIMG);//montre l'image
   waitKey(0);//attends jusqu'a le user appuye
   */



  //  Ligne ligne1(Point(30, 200), Point(600, 180));
  //  Ligne ligne1(Point(30, 200), Point(600, 200)); // droite y1=y2

    //Ligne pour sur la rue
    Ligne ligne1(Point(180, 300), Point(540, 200));
    Mat montageImage(600, abs(ligne1.getP1().x - ligne1.getP2().x), CV_8UC3);

    extractLine(refImg,backgroundLine,studiedLine,studiedLineWidth);
    imshow("image de fond",backgroundLine);
    while (1)
    {
        int c = waitKey(1);
        if( (char)c == 27 )//touche echap
            break;
        if(checkIfNewImage(imageIndex))
        {
            currentImg = imread(path);
            ligne1.extractFromImage(currentImg);

            line(currentImg, ligne1.getP1(), ligne1.getP2(), Scalar(255, 0, 0));
            imshow("Image", currentImg);

            Mat data = ligne1.getData();
            data.copyTo(montageImage(Rect(0, imageIndex, data.cols, data.rows)));

            // Afficher tous les x images -> c'est plus vite
            if (imageIndex % 1 == 0)
            {
                imshow("montageImage", montageImage);
            //    cout << data.cols << endl;
                waitKey(4);
            }



/*
            extractLine(currentImg,extractedLine,studiedLine,studiedLineWidth);
            imshow("extractedLines",extractedLine);
            substractBackground(backgroundLine,extractedLine,extractedLineNoBackground);

            cvWaitKey(1);

            //cout<<toString(extractedLineNoBackgroundSize.width)<<endl;
            extractedLineNoBackgroundSize=extractedLineNoBackground.size();

            for(int i=0; i<extractedLineNoBackgroundSize.width; i++)
            {
                cout<<(extractedLineNoBackground.at<bool>(0,1))<<endl;

            }
*/

            //Si on trouve une nouvelle image, on peut faire le traitement. c'est ici que ca commence.
            imageIndex++;
        }
    }

}


bool checkIfNewImage(int lastImageIndex)
{
    //fonction qui va regarder dans le répertoire data si il y a une nouvelle image
    if (lastImageIndex<10)
    {
        path="data/detection_000"+toString(lastImageIndex)+".jpeg";
    }
    else if (lastImageIndex<100)
    {
        path="data/detection_00"+toString(lastImageIndex)+".jpeg";
    }
    else if (lastImageIndex<1000)
    {
        path="data/detection_0"+toString(lastImageIndex)+".jpeg";
    }
    Mat img  = imread(path, 1);
    if(img.empty() )
    {
        cout << "Couldn't open image "<<lastImageIndex<<endl;
        cvWaitKey();
        return false;
    }
    else
    {
        //cout<<"image :"<<path<<" found"<<endl;
        return true;
    }
}

//Fonction qui retourne la valeur d'un INT sous forme de chaine de charactère.
string toString(int val)
{
    stringstream ss;
    ss<<val;
    string str=ss.str();
    return str;
}

//Fonction qui extrait la ligne que l'on analyse  de l'image courante. ...
//Dans la version deux, on pourra toujours appeller plusieure fois cette fonction pour extraire plusieures lignes.
void extractLine(Mat img,Mat &extractedLine,int lineNumber, int lineWidth)
{
    if((lineNumber+lineWidth<img.rows)||(lineNumber+lineWidth<img.rows))
    {
        extractedLine=img.rowRange(lineNumber,lineNumber+lineWidth);
    }
    else
    {
        // si on essaye d'extraire une ligne de matrice qui n'existe pas dans la matrice d'origine
        //on affiche un message d'erreur et on attend 1000ms.
        cout<<"Line to extract out of bound, max line is :"<<img.rows<<endl;
        cvWaitKey(1000);
    }
}
//Fonction qui soustrait le fond de l'image à l'image courante.
//refImg est l'image de base (sans pietons que l'on a choisis au début.)
void substractBackground(Mat refImg,Mat CurrentImg, Mat &OutputImg)
{
        Mat diff;
        absdiff(CurrentImg,refImg,OutputImg);

        imshow("Image Without Background",OutputImg);
}

//Function qui detect le click du sourie et stoque les x et y
void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
     if  ( event == EVENT_LBUTTONDOWN )
     {
          cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }
}

