#include "ligne.h"

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
void Ligne::extractFromImage(Mat image)
{
    LineIterator it(image, P1, P2, 4);
    Data = Mat(1, it.count, CV_8UC3);

    for(int i = 0; i < it.count; i++, ++it)
    {
        Data.at<Vec3b>(0, i) = image.at<Vec3b>(it.pos());
    }


    //Ici: Traitement de la ligne (Soustraction du fond. Blocs, etc)

}
