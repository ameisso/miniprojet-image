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
void getParamsFromPrefsFile(string pathToFile, vector<Ligne> &lines, int &footerWidth, int &footerHeight, double &bgLearningRate, int &bgThreshold, int &detThreshold);
void storeParamsToPrefsFile(string pathToFile, vector<Ligne> &lines, int &footerWidth, int &footerHeight, double &bgLearningRate, int &bgThreshold, int &detThreshold);
void getLinesFromUser(InputArray refImg, vector<Ligne> &lines);
void modifierLignesCallBack(int state, void* userdata);

//*************************************************************************

const string path = "data/detection_%04d.jpeg";//répertoire de travail ou index de la caméra
const string prefsFilePath = "prefs.xml"; //fichier de stockage des paramètres (position des lignes et autres)
const string windowName = "Comptage de pietons"; //Nom de la fenêtre

int main()
{
    //Variables
    bool run=true;//si on appuie sur la touche r, on continue d'exectuter le programme, sinon, on l'arrete, ca permet de voir ce qui se passe dans les matrices.
    int nbFootersMoyenne = 0 ;//nb piétons en moyenne sur toutes les lignes

    int footerWidth, footerHeight; //Taille d'un piéton pour la détection
    double bgLearningRate; //Learning rate pour la soustraction de fond ; doit être compris entre 0 et 1 avec 0 : pas d'évolution du fond et 1 : pas de soustraction
    int bgThreshold; //Seuil pour la soustraction de fond : si les différences entre 2 images sont plus importantes que ce seuil la nouvelle image n'est pas prise en compte dans le fond
    int detectionThreshold; //Seuil pour la détection de piétons sur le fond

    std::vector<Ligne> inputLigne; //stockage des lignes de comptage

    VideoCapture video(path);
    Mat currentImg;//image que l'on est en train d'analyser
    if(!video.read(currentImg))//récupération de la première image
    {
        cout << "Unable to open device or file " << path << endl;
        return(1);
    }

    //Récupération des paramètres précédents
    getParamsFromPrefsFile(prefsFilePath, inputLigne, footerWidth, footerHeight, bgLearningRate, bgThreshold, detectionThreshold);

    if(inputLigne.size() == 0) //si aucune ligne définie
    {
        getLinesFromUser(currentImg, inputLigne);
        storeParamsToPrefsFile(prefsFilePath, inputLigne, footerWidth, footerHeight, bgLearningRate, bgThreshold, detectionThreshold);
    }

    //Passage des paramètres à toutes les lignes
    for(vector<Ligne>::iterator it=inputLigne.begin(); it !=inputLigne.end(); ++it)
    {
        (*it).setfooterWidth(footerWidth);
        (*it).setfooterHeight(footerHeight);
        (*it).setBgLearningRate(bgLearningRate);
        (*it).setBgThreshold(bgThreshold);
        (*it).setDetectionThreshold(detectionThreshold);
    }

    //Création de la fenêtre d'affichage
    namedWindow(windowName);

    //Bouton pour proposer à l'utilisateur de modifier les lignes
    //bool updateLines = false;
    //createButton("Modifier lignes", modifierLignesCallBack, &updateLines); //openCV doit être compilé avec le "Qt support" pour que ça fonctionne

    //Mat montageImage(600, abs(ligne1.getP1().x - ligne1.getP2().x), CV_8UC3);
    //Mat montageImage(600, 600, CV_8UC3);

    cout << "Comptage de pietons ; appuyer sur 'r' pour pause/redémarrage" << endl << "appuyer sur 'c' pour changer les lignes de comptage" << endl;

    while (1)
    {
        int c = waitKey(1);
        if( (char)c == 27 )//touche echap
        {
            break;
        }
        else if((char)c=='r')//touche R
        {
            cout<<"run"<<endl;
            run=!run;
        }
        else if((char)c=='c')
        {
            getLinesFromUser(currentImg, inputLigne);
            storeParamsToPrefsFile(prefsFilePath, inputLigne, footerWidth, footerHeight, bgLearningRate, bgThreshold, detectionThreshold);
            break;
        }
        if (run)
        {
            if(video.read(currentImg))
            {
                for ( vector<Ligne>::iterator it=inputLigne.begin(); it !=inputLigne.end(); ++it )
                {
                    it->extractFromImage(currentImg);

                    line(currentImg, it->getP1(), it->getP2(), Scalar(255, 0, 0));

                   /* Mat data = it->getData();
                    data.copyTo(montageImage(Rect(0, imageIndex, data.cols, data.rows)));*/

                    int nbFootersnew= it->getNbFooters();
                    nbFootersMoyenne += nbFootersnew;

                }
                nbFootersMoyenne = nbFootersMoyenne/inputLigne.size();
                putText(currentImg, intToString(nbFootersMoyenne), Point(20,80), FONT_HERSHEY_SIMPLEX, 2.0f, Scalar(255,255,255), 2);

                //cout<<"nombre des pietons en moyenne"<<nbFootersMoyenne<<endl;
                nbFootersMoyenne = 0;

                imshow(windowName, currentImg);
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
         //cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;

     }
}

/* Récupération des lignes de comptage et autres paramètres depuis un fichier de préférénces XML */
void getParamsFromPrefsFile(string pathToFile, vector<Ligne> &lines, int &footerWidth, int &footerHeight, double &bgLearningRate, int &bgThreshold, int &detThreshold)
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

    prefs["footerWidth"] >> footerWidth;
    prefs["footerHeight"] >> footerHeight;
    prefs["bgLearningRate"] >> bgLearningRate;
    prefs["bgThreshold"] >> bgThreshold;
    prefs["detThreshold"] >> detThreshold;

    prefs.release();
}

/* Stockage des lignes de comptage et autres paramètres dans un fichier de préférences XML */
void storeParamsToPrefsFile(string pathToFile, vector<Ligne> &lines, int &footerWidth, int &footerHeight, double &bgLearningRate, int &bgThreshold, int &detThreshold)
{
    FileStorage prefs(pathToFile, FileStorage::WRITE);

    prefs << "lignes" << "[";
    for(vector<Ligne>::iterator it = lines.begin(); it != lines.end(); ++it)
    {
        prefs << "{" << "P1" << (*it).getP1() << "P2" << (*it).getP2() << "}";
    }
    prefs << "]";

    prefs << "footerWidth" << footerWidth;
    prefs << "footerHeight" << footerHeight;
    prefs << "bgLearningRate" << bgLearningRate;
    prefs << "bgThreshold" << bgThreshold;
    prefs << "detThreshold" << detThreshold;

    prefs.release();
}

/* Entrée des lignes de comptage par l'utilisateur puis stockage dans le fichier de préférences XML */
void getLinesFromUser(InputArray refImg, vector<Ligne> &lines)
{
    std::vector<Point> inputPoint;
    Mat img = refImg.getMat();

    namedWindow("Modification lignes",1);
    imshow("Modification lignes", img );
    setMouseCallback("Modification lignes", CallBackFunc, &inputPoint);

    lines.clear();

    cout << "Cliquer pour définir les extrémités de la nouvelle ligne puis Echap quand termine" << endl;

    while( int w = waitKey(60) )
    {
        if( (char)w == 27 )//touche echap
        {
            break;
        }
        else if ( inputPoint.size() == 2 )
        {
            lines.push_back(Ligne(lines.size(),inputPoint[0],inputPoint[1]));
            cout<<"ligne ajoutee"<<endl;
            inputPoint.clear();

            line(img, inputPoint[0], inputPoint[1], Scalar(255, 255, 255));
            imshow("Modification lignes", img);
        }
    }

    destroyWindow("Modification lignes");
}

/* Fonction de callback pour le bouton "Modifier lignes" */
/*void modifierLignesCallBack(int state, void* userdata)
{
    bool *update = (bool*)userdata;

    if(state)
        *update = true;
}*/
