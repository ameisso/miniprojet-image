#include "bloc.h"


bloc::bloc(int leftPos_, int rightPos_)
{
    lastRightPos=rightPos_;
    lastLeftPos=leftPos_;
    maxBlocWidth=lastRightPos-lastLeftPos;
    cout<<"new bloc ["<<lastLeftPos<<","<<lastRightPos<<"] widht :"<<maxBlocWidth<<endl;
    blocHeight=1;
    dead=false;
}

//Fonction qui prend en paramètre les position des valeurs non noires de la matrice "extractedLineNoBackground" et qui vérifie si elle appartient a ce bloc.
//elle retourne true si le bloc proposé appartient à ce bloc, false sinon.
bool bloc::checkNewLine(int leftPos, int rightPos)
{
    if(((rightPos<=lastRightPos) && (rightPos>=lastLeftPos))|((leftPos>=lastLeftPos) && (leftPos <=lastRightPos))|((leftPos<=lastLeftPos)&&(rightPos>lastRightPos)))
    {
        cout<<leftPos<<","<<rightPos<<" owned by ["<<lastLeftPos<<","<<lastRightPos<<"] widht :"<<maxBlocWidth<<endl;
        blocHeight++;// on augmente le nombre de frames pour mesurer la longeur du bloc.(+1);
        if (rightPos-leftPos>maxBlocWidth)
        {
            maxBlocWidth=rightPos-leftPos;
            cout<<"MaxBlocWidth updated"<<endl;
        }
        lastLeftPos=leftPos;
        lastRightPos=rightPos;
        dead=false;//on a pu ajouter une ligne au bloc, donc il n'est pas mort.
        return true;
    }
    else
    {
        return false;
    }
}
//Fonction qui calcule le nombre de personnes dans le bloc et incrémente le compteur.
void bloc::deadBloc()
{
    int nbFooterInWidth=maxBlocWidth/footerWidth;

    //cout<<"pietons comptés dans ce bloc"<<nbFooterInWidth<<endl;
    nbFooters=nbFooterInWidth;
}
int bloc::getNbFooters()
{
    return nbFooters;
}
void bloc::toString()
{
    cout<<"["<<lastLeftPos<<","<<lastRightPos<<"] h :"<<blocHeight<<", w :"<<maxBlocWidth<<endl;
}
bool bloc::checkDead()
{
    if (dead==true)
        cout<<" deleted ["<<lastLeftPos<<","<<lastRightPos<<"]"<<endl;
    return dead;
}
void bloc::setDead(bool val)
{
    dead=val;
}

