#include "ligne.h"

#include <iostream>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

Ligne::Ligne(Point P1, Point P2) : P1(P1), P2(P2)
{
    int nombre = abs(P1.x - P2.x);
    Data = Mat::zeros(1, nombre, CV_8UC3);
}

// Extraire une ligne d'image d'origine et stocker dans Data
void Ligne::extractFromImage(Mat image)
{
    //Nombre de pixels dans le vecteur
    int nombre = abs(P1.x - P2.x);
    for (int i = 0; i < nombre; i++) {
        // Calculer les points sur une droite
        Point_<int> pos = P1 + (double) i / nombre * (P2-P1);

        //Stocker dans la matrice Data! Vec3b, parce qu'il faut copier les valeurs BlueGreenRed!
        Data.at<Vec3b>(0, i) = image.at<Vec3b>(pos.y, pos.x);
    }

/*  //En fait, c'est la meme chose!
    LineIterator it(image, P1, P2, 4);
    Data = Mat::zeros(1, it.count, CV_8UC3);

    for(int i = 0; i < it.count; i++, ++it)
    {
        Vec3b val = image.at<Vec3b>(it.pos());
        Data.at<Vec3b>(0, i) = val;
    }
*/

    //Ici: Traitement de la ligne (Soustraction du fond. Blocs, etc)

}
