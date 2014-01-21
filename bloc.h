#ifndef BLOC_H
#define BLOC_H
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <cstdio>
#include <iostream>
#include <algorithm>
using namespace cv;
using namespace std;

class bloc
{
private:
    int footerWidth;
    int footerHeight;

    int blocHeight; //hauteur du bloc (nombre d'images ou le bloc existe.
    int lastRightPos, lastLeftPos;//position droite et gauche du dernier bloc de pixels que l'on a ajouté au bloc.
    int nbFooters;//nombre de piétons dans le bloc (0 de base, mis a jour a l'appel de deadBloc)
    int maxBlocWidth;
    bool dead;//true si le bloc est mort.
    int nbDead;//nombre de lignes ou l'on ajoute rien au bloc avant de le supprimer.
public:
    bloc(int leftPos_, int rightPos_, int footerWidth_, int footerHeight_);

    void setfooterWidth(int width) { footerWidth = width; }
    void setfooterHeight(int height) { footerHeight = height; }

    bool checkNewLine(int leftPos, int rightPos);
    int getNbFooters();
    void toString();
    bool checkDead();
    void setDead(bool val);
    void deadBloc();
    bool checkDuplicate(bloc *testedBloc);


};

#endif // BLOC_H
