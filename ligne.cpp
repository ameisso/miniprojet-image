#include "ligne.h"

#include <iostream>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

Ligne::Ligne()
{
}

// Extraire une ligne d'image d'origine et stocker dans Data
void Ligne::extractFromImage(Mat image)
{
    //Nombre de pixels dans le vecteur
    int nombre = abs(P1.x - P2.x);

    cout << nombre << endl;

    Data = Mat(Size(1, nombre), CV_8UC3);

    for (int i = 0; i < nombre; i++) {
        // Calculer les points sur une droite
        Point_<double> pos = P1 + (double) i / nombre * (P2-P1);

        // Stocker dans la matrice Data
        Data.at<int>(Point(0, i)) = image.at<int>(pos);


        //cout << pos << "--" << image.at<int>(pos) << endl;



    }

  //  cout << Data << endl;

}
