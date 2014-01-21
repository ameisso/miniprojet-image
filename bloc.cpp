#include "bloc.h"


bloc::bloc(int leftPos_, int rightPos_, int footerWidth_, int footerHeight_)
{
    lastRightPos = rightPos_;
    lastLeftPos = leftPos_;
    footerWidth = footerWidth_;
    footerHeight = footerHeight_;

    maxBlocWidth=lastRightPos-lastLeftPos;
   // cout<<"new bloc ["<<lastLeftPos<<","<<lastRightPos<<"] widht :"<<maxBlocWidth<<endl;
    blocHeight=1;
    dead=false;
    nbFooters=0;
    nbDead=2;//on s'autorise une ligne sans rien ajouter au bloc avant de le supprimer.
}

//Fonction qui prend en paramètre les position des valeurs non noires de la matrice "extractedLineNoBackground" et qui vérifie si elle appartient a ce bloc.
//elle retourne true si le bloc proposé appartient à ce bloc, false sinon.
bool bloc::checkNewLine(int leftPos, int rightPos)
{
    if(((rightPos<=lastRightPos) && (rightPos>=lastLeftPos))|((leftPos>=lastLeftPos) && (leftPos <=lastRightPos))|((leftPos<=lastLeftPos)&&(rightPos>lastRightPos)))
    {
        //cout<<leftPos<<","<<rightPos<<" owned by ["<<lastLeftPos<<","<<lastRightPos<<"] widht :"<<maxBlocWidth<<endl;
        blocHeight++;// on augmente le nombre de frames pour mesurer la longeur du bloc.(+1);
        if (rightPos-leftPos>maxBlocWidth)//mise à jour de la largeur maximale du bloc.
        {
            maxBlocWidth=rightPos-leftPos;
            //cout<<"MaxBlocWidth updated"<<endl;
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
   //int nbFooterInWidth=round((float)maxBlocWidth/(float)footerWidth);
   //int nbFooterInHeight=round((float)blocHeight/(float)footerHeight);
    int nbFooterInWidth=maxBlocWidth/footerWidth;
    int nbFooterInHeight=blocHeight/footerHeight;
    if(nbFooterInHeight != 0 && nbFooterInWidth != 0)
    {
        nbFooters=nbFooterInWidth;
        //toString();
        cout<<"["<<lastLeftPos<<","<<lastRightPos<<"] h :"<<blocHeight<<", w :"<<maxBlocWidth<<"-->"<<nbFooterInWidth<<" piétons comptés dans le bloc dont "<<nbFooterInWidth<<" dans la largeur et "<<nbFooterInHeight <<" dans la hauteur."<<endl;

    }
}

int bloc::getNbFooters()
{
    return nbFooters;
}
void bloc::toString()
{
    cout<<"["<<lastLeftPos<<","<<lastRightPos<<"] h :"<<blocHeight<<", w :"<<maxBlocWidth<<" nbDead :"<<nbDead<<endl;
}
bool bloc::checkDead()
{
    //toString();
    if (dead==true && nbDead!=0)
    {
        nbDead--;
        dead=false;
    }
    return dead;
}
void bloc::setDead(bool val)
{
    dead=val;
}
//Fonction qui renvoie true si un bloc est équivalent au bloc testé
bool bloc::checkDuplicate  (bloc *testedBloc)
{
    if ((testedBloc->lastRightPos==lastRightPos) && (testedBloc->lastLeftPos==lastLeftPos))
    {
        //cout<<"equivalent bloc found ["<<lastLeftPos<<","<<lastRightPos<<"]"<<endl;
        //on copie les plus grandes valeurs dans ce bloc.
        maxBlocWidth=max<int>(maxBlocWidth,testedBloc->maxBlocWidth);
        blocHeight=max<int>(blocHeight,testedBloc->blocHeight);
        return true;
    }
}
