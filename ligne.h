#ifndef LIGNE_H
#define LIGNE_H

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "bloc.h"
#include "backgroundsubtractoravg.h"

using namespace cv;

class Ligne
{
private:
    // Definir la taille d'un pieton. C'est la meme valeur pour tous le blocs sur une ligne
    int footerWidth;
    int footerHeight;

    Point P1, P2;
    Mat Data, Bgr;

    unsigned char seuil;//valeurs au dessus desquelles on détecte un bloc.
    int tailleMiniBloc;//taille en dessous de laquelle on analyse pas le bloc.
    vector<bloc*> theBlocs;//vecteur de pointeurs vers les blocs
public:
    Ligne();
    Ligne(Point P1, Point P2);

    Point getP1() const { return P1; }
    Point getP2() const { return P2; }

    int getfooterWidth() const { return footerWidth; }
    int getfooterHeight() const { return footerHeight; }
    void setfooterWidth(int width);
    void setfooterHeight(int height);

    const Mat& getData() const { return Data; }
    void extractFromImage(Mat image, Mat background);
private:
    void substractBackground(Mat refImg, Mat CurrentImg, Mat &OutputImg);
    void detectionDesBlocs(Mat imageSansFond);
    void cleanTheBlocs();

    Point P1, P2;
    Mat Data;
    BackgroundSubtractorAvg bgSubtractor;
};

#endif // LIGNE_H
