#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include <cstdio>
#include <iostream>
#include <vector>
#include "bloc.h"
#include "ligne.h"
#include "backgroundsubtractoravg.h"
#include "fonctions.h"

using namespace cv;
using namespace std;

//*************************************************************************
//Déclarations des fonctions
void CallBackFunc(int event, int x, int y, int flags, void* userdata);
void getLinesFromPrefsFile(string pathToFile, vector<Ligne> &lines);
void storeLinesToPrefsFile(string pathToFile, vector<Ligne> &lines);
void getLinesFromUser(InputArray refImg, string pathToFile, vector<Ligne> &lines);

//*************************************************************************

int main()
{
    //Variables
    bool run=true;//si on appuie sur la touche r, on continue d'exectuter le programme, sinon, on l'arrete, ca permet de voir ce qui se passe dans les matrices.
    std::vector<Ligne> inputLigne; //stockage des lignes de comptage

    string path = "data/detection_%04d.jpeg";//répertoire de travail ou index de la caméra
    string prefsFilePath = "prefs.xml"; //fichier de stockage des paramètres (position des lignes et autres)
    VideoCapture video(path);
    int imageIndex = 0; //nb de frames analysées

    Mat currentImg;//image que l'on est en train d'analyser
    if(!video.read(currentImg))//récupération de la première image
    {
        cout << "Unable to open device or file " << path << endl;
        return(1);
    }

    //Récupération des paramètres précédents
    getLinesFromPrefsFile(prefsFilePath, inputLigne);

    if(inputLigne.size() == 0) //si aucune ligne définie
        getLinesFromUser(currentImg, prefsFilePath, inputLigne);

    //TODO ajouter bouton pour entrée des lignes par l'utilisateur

    /*Ligne ligne1(P1, P2);
    ligne1.setfooterWidth(28);
    ligne1.setfooterHeight(20);
    A faire pour toutes les lignes ?*/

    //Mat montageImage(600, abs(ligne1.getP1().x - ligne1.getP2().x), CV_8UC3);
    Mat montageImage(600, 600, CV_8UC3);

    while (1)
    {
        int c = waitKey(20);
        if( (char)c == 27 )//touche echap
        {
            break;
        }
        else if((char)c=='r')//touche R
        {
            cout<<"run"<<endl;
            run=!run;
        }
        if (run)
        {
            if(video.read(currentImg))
            {
                for ( vector<Ligne>::iterator it=inputLigne.begin(); it !=inputLigne.end(); it++ )
                {
                    it->extractFromImage(currentImg);

                    line(currentImg, it->getP1(), it->getP2(), Scalar(255, 0, 0));
                    imshow("Image avec ligne", currentImg);

                    Mat data = it->getData();
                    data.copyTo(montageImage(Rect(0, imageIndex, data.cols, data.rows)));
                }

                imageIndex++;
            }
        }
    }
}

//Function qui detecte le click du sourie et stoque les x et y
//on envoie dans userdata un pointeur vers inputPoint qui est un vector de Point
void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    vector<Point> *inputPoints = (vector<Point>*)userdata;
    if( event == EVENT_RBUTTONDOWN )
    {
       // inputState = reset;
        inputPoints->clear();
    }

     if( event == EVENT_LBUTTONDOWN )
     {
         if( inputPoints->size() <= 1)
         {
             inputPoints->push_back(Point(x,y));
         }
         cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;

     }
}

/* Récupération des lignes de comptage depuis un fichier de préférénces XML */
void getLinesFromPrefsFile(string pathToFile, vector<Ligne> &lines)
{
    FileStorage prefs(pathToFile, FileStorage::READ);

    FileNode lignes = prefs["lignes"];
    FileNodeIterator it = lignes.begin(), it_end = lignes.end();
    int i=0;

    Point P1,P2;

    for( ; it != it_end ; ++it, i++)
    {
        (*it)["P1"] >> P1;
        (*it)["P2"] >> P2;

        lines.push_back(Ligne(i,P1,P2));
    }

    prefs.release();
}

/* Stockage des lignes de comptage dans un fichier de préférences XML */
void storeLinesToPrefsFile(string pathToFile, vector<Ligne> &lines)
{
    FileStorage prefs(pathToFile, FileStorage::WRITE);

    prefs << "lignes" << "[";
    for(vector<Ligne>::iterator it = lines.begin(); it != lines.end(); ++it)
    {
        prefs << "{" << "P1" << (*it).getP1() << "P2" << (*it).getP2() << "}";
    }
    prefs << "]";

    prefs.release();
}

/* Entrée des lignes de comptage par l'utilisateur puis stockage dans le fichier de préférences XML */
void getLinesFromUser(InputArray refImg, string pathToFile, vector<Ligne> &lines)
{
    std::vector<Point> inputPoint;
    Mat img = refImg.getMat();

    namedWindow("refImg",1);
    imshow("refImg", img );
    setMouseCallback("refImg", CallBackFunc, &inputPoint);

    //TODO affichage instructions

    while( int w = waitKey(60) )
    {
        if( (char)w == 27 )//touche echap
        {
            break;
        }
        else if ( inputPoint.size() == 2 )
        {
            lines.push_back(Ligne(lines.size(),inputPoint[0],inputPoint[1]));
            cout<<"ligne ajoute"<<endl;
            inputPoint.clear();

            line(img, inputPoint[0], inputPoint[1], Scalar(255, 0, 0));
            imshow("refImg", img);
        }
    }

    storeLinesToPrefsFile(pathToFile, lines);

}
