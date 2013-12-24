#include "ligne.h"
#include "bloc.h"
#include <iostream>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

Ligne::Ligne() : Ligne(Point(0,0), Point(0,0))
{
}

Ligne::Ligne(Point P1, Point P2) : P1(P1), P2(P2)
{
}

// Extraire une ligne d'image d'origine et stocker dans Data
void Ligne::extractFromImage(Mat image, Mat background)
{
    LineIterator it(image, P1, P2, 4);
    Data = Mat(1, it.count, CV_8UC3);
    Bgr= Mat(1, it.count, CV_8UC3);

    for(int i = 0; i < it.count; i++, it++)
    {
        Data.at<Vec3b>(0, i) = image.at<Vec3b>(it.pos());
        Bgr.at<Vec3b>(0,i)=background.at<Vec3b>(it.pos());
    }
    //Soustraction de fond
    substractBackground(Bgr,Data,Data);//on soustrait le fond et on écrit dans data.
    //Detection des blocs
    detectionDesBlocs(Data);
    //Ici: Traitement de la ligne (Soustraction du fond. Blocs, etc)
}
/**************************************************
 * Fonction qui soustrait le fond à l'image.
 * Pour l'instant ca reste une fonction, dans le futur il faudra aussi modifier l'image de fond après la soustraction
 * C'est les histoires de moyenne glissante...
 * *************************************************/
void Ligne::substractBackground(Mat refImg,Mat CurrentImg, Mat &OutputImg)
{
        absdiff(CurrentImg,refImg,OutputImg);
}
/**************************************************
 * Fonction qui prend une image auquel on a déja soustrait l'image de fond
 * et balaye tout les points de la matrice pour voir si leur intensité diffère de 0.
 * Si oui, on voit si on ajoute ce point à un bloc existant ou si on crée un nouveau bloc.
 *
 * *************************************************/
void Ligne::detectionDesBlocs(Mat imageSansFond)
{
    int *ptrImageSansFond = (int*)(imageSansFond.data);
    //on multiplie la matrice pour voir x* la même ligne
    Mat grandeImageSansFond;
    for(int i=0; i<30;i++)
        grandeImageSansFond.push_back(imageSansFond);
    imshow("grandeImageSansFond",grandeImageSansFond);
    //on accède aux valeurs de pixels
    for(int i = 0;i < imageSansFond.cols;i++){
        cout<<"rgb : ["<< toString((unsigned char)ptrImageSansFond[ i + 2])<<","<< toString((unsigned char)ptrImageSansFond[ i + 1])<<","<< toString((unsigned char)ptrImageSansFond[ i ])<<"]"<<endl;
        //TODO tester si la variation dépasse un certain seuil (en paramètre de l'objet Ligne ?)
        //en fonction soit créer un objet soit ajouter les pixels au bloc.
    }
}

/**************************************************
Fonction qui convertie un int en string.
 * *************************************************/
string Ligne::toString(int val)
{
    stringstream ss;
    ss<<val;
    string str=ss.str();
    return str;
}
