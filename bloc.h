#ifndef BLOC_H
#define BLOC_H
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <cstdio>
#include <iostream>
using namespace cv;
using namespace std;

#define footerWidth 10//largeur en pixel d'un piéton
#define footerHeight 20//"hauteur en pixels d'un piéton
class bloc
{
public:

    bloc(int leftPos_, int rightPos_);
    bool checkNewLine(int leftPos, int rightPos);
    int getNbFooters();
    void toString();
    bool checkDead();
private:
    void deadBloc();
    int blocHeight; //hauteur du bloc (nombre d'images ou le bloc existe.
    int lastRightPos, lastLeftPos;//position droite et gauche du dernier bloc de pixels que l'on a ajouté au bloc.
    int nbFooters;//nombre de piétons dans le bloc (0 de base, mis a jour a l'appel de deadBloc)
    int maxBlocWidth;
    bool dead;//true si le bloc est mort.
};

#endif // BLOC_H
