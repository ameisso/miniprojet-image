#include "bloc.h"

bloc::bloc(int rightPos_, int leftPos_)
{
    lastRightPos=rightPos_;
    lastLeftPos=leftPos_;
    maxBlocWidth=lastRightPos-lastLeftPos;
    cout<<"new bloc ["<<lastRightPos<<","<<lastLeftPos<<"] widht :"<<maxBlocWidth<<endl;
    blocHeight=1;
}

//Fonction qui prend en paramètre les position des valeurs non noires de la matrice "extractedLineNoBackground" et qui vérifie si elle appartient a ce bloc.
//elle retourne true si le bloc proposé appartient à ce bloc, false sinon.
bool bloc::checkNewLine(int rightPos, int leftPos)
{
    if(((rightPos>lastRightPos) && (rightPos<lastLeftPos))||((leftPos>=lastRightPos) && (leftPos >=lastLeftPos)))
    {
        cout<<"owned by this bloc"<<endl;//
        blocHeight++;// on augmente le nombre de frames pour mesurer la longeur du bloc.(+1);
        if (leftPos-rightPos>maxBlocWidth)
        {
            maxBlocWidth=leftPos-rightPos;
            cout<<"MaxBlocWidth updated"<<endl;
        }

        return true;
    }
    else
    {
        deadBloc();
        return false;
    }
}
//Fonction qui calcule le nombre de personnes dans le bloc et incrémente le compteur.
void bloc::deadBloc()
{
    int nbFooterInWidth=maxBlocWidth/footerWidth;
    cout<<nbFooterInWidth<<endl;
    nbFooters=nbFooterInWidth;
}
int bloc::getNbFooters()
{
    return nbFooters;
}
