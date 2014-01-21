#ifndef LIGNE_H
#define LIGNE_H

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "bloc.h"
#include "backgroundsubtractoravg.h"

using namespace cv;

class Ligne
{
public:
    //Ligne();
    Ligne(int nrLigne, Point P1, Point P2);

    Point getP1() const { return P1; }
    Point getP2() const { return P2; }
    int   getNbFooters();

    int getfooterWidth() const { return footerWidth; }
    int getfooterHeight() const { return footerHeight; }
    //   int nbFooters() const {return nbFooter;}
    void setfooterWidth(int width);
    void setfooterHeight(int height);
    void setBgLearningRate(double bgLR);
    void setBgThreshold(int bgThresh);
    void setDetectionThreshold(int detThresh);

    const Mat& getData() const { return Data; }

    void extractFromImage(Mat image);
private:
    void detectionDesBlocs(Mat imageSansFond);
    int cleanTheBlocs();


    Point P1, P2;
    Mat Data;
    BackgroundSubtractorAvg bgSubtractor;
    double bgLearningRate; //Learning rate pour la soustraction de fond

    int nbFooters;
    int nrLigne;

    // Definir la taille d'un pieton. C'est la meme valeur pour tous le blocs sur une ligne
    int footerWidth;
    int footerHeight;

    unsigned char seuil;//valeurs au dessus desquelles on détecte un bloc.
    int tailleMiniBloc;//taille en dessous de laquelle on analyse pas le bloc.
    vector<bloc*> theBlocs;//vecteur de pointeurs vers les blocs
};

#endif // LIGNE_H
