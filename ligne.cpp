#include "ligne.h"
#include "bloc.h"
#include <iostream>
#include <vector>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

/*
Ligne::Ligne() : Ligne(Point(0,0), Point(0,0))
{
}
*/

Ligne::Ligne(int nrLigne , Point P1, Point P2) : P1(P1), P2(P2)
{
    this->nrLigne = nrLigne;

    //Valeurs par défaut
    footerWidth = 1;
    footerHeight = 1;
    bgSubtractor.setThreshold(10.0);
    seuil=1;

    tailleMiniBloc=footerWidth/2;
    nbFooters=0;

}

// Pour changer la taille d'un pieton, il faut iterer dans tous les blocs
void Ligne::setfooterWidth(int width)
{
    footerWidth = width;
    tailleMiniBloc=width/2;

    for(vector<bloc*>::iterator it = theBlocs.begin(); it != theBlocs.end(); it++)
        (*it)->setfooterWidth(width);
}

void Ligne::setfooterHeight(int height)
{
    footerHeight = height;

    for(vector<bloc*>::iterator it = theBlocs.begin(); it != theBlocs.end(); it++)
        (*it)->setfooterHeight(height);
}

// Extraire une ligne d'image d'origine et stocker dans Data
void Ligne::extractFromImage(Mat image)
{
    LineIterator it(image, P1, P2, 4);
    Mat newLine = Mat(1, it.count, CV_8UC3);

    for(int i = 0; i < it.count; i++, it++)
    {
        newLine.at<Vec3b>(0, i) = image.at<Vec3b>(it.pos());
    }
    //Soustraction de fond
    bgSubtractor(newLine, Data, bgLearningRate);
    //Detection des blocs
    detectionDesBlocs(Data);
    //supression des blocs vides
    int footers = cleanTheBlocs();
    nbFooters += footers;

    //if(footers != 0 )
        //cout<<"nombre ligne"<<nrLigne<<"pietons avoir passe la ligne "<<nbFooters<<endl;
}

/**************************************************
 * Fonction qui prend une image auquel on a déja soustrait l'image de fond
 * et balaye tout les points de la matrice pour voir si leur intensité diffère de 0.
 * Si oui, on voit si on ajoute ce point à un bloc existant ou si on crée un nouveau bloc.
 *
 * *************************************************/
void Ligne::detectionDesBlocs(Mat imageSansFond)
{
    // on erode le bloc pour virer le bruit.
/*    Mat lignePreTraitement=imageSansFond;
    Mat element = getStructuringElement(1,Size(tailleMiniBloc-1, tailleMiniBloc-1));
    erode(lignePreTraitement,imageSansFond,element);*/


    Mat grandeImageSansFond=imageSansFond;//aucun traitement sur cette matrice, ne sert qu'à l'affichage.
    int leftPos=0, rightPos=0; //Position extrème des pixels du bloc.
    bloc*currentBloc;
    bool ajoutBloc;//booléen qui teste si on a réussit a ajouter la ligne de pixels non noir a un bloc.
    cout<<"-----------------------------------"<<endl;//nouvelle image


    //---->début du traitement
    //on met tout le blocs qui existent déjà comme mort.
    //Si on ajoute une ligne au bloc alors on lui enlève son status de mort, sinon on le supprime.
    cout<<"theBlocks :"<<endl;
    for(vector<bloc*>::iterator it = theBlocs.begin(); it != theBlocs.end(); it++)
    {
        (*it)->setDead(true);
        (*it)->toString();    //affichage de touts les blocs du vecteur (pour vérif)
    }
    cout<<"*****************"<<endl;
    //on accède aux valeurs de pixels
    bool debutSeuil=false;
    for(int i = 0;i < imageSansFond.cols;i++){
        //détection du début du bloc.
        Vec3b intensity = imageSansFond.at<Vec3b>(0, i);//pour gagner en temps on peut acceder à la matrice par pointeurs... il parait
        uchar blue = intensity.val[0];
        uchar green = intensity.val[1];
        uchar red = intensity.val[2];
        uchar intensite=(red+green+blue)/3;
        //cout<<toString(intensite)<<",";


        if ((intensite>seuil)&&(!debutSeuil))
        {
            debutSeuil=true;
            leftPos=i;
        }
        //on détecte la fin du bloc
        else if ((leftPos!=0) &&(intensite<seuil)&&( i-1-leftPos>tailleMiniBloc))
        {
            debutSeuil=false;//permet la detection d'un nouveau seuil sur la même ligne
            //affiche le début du bloc détecté en rouge
            Vec3b &intensityd = grandeImageSansFond.at<Vec3b>(0, leftPos);
            intensityd.val[2]=255;
            //affiche la fin du bloc détecté en vert.
            Vec3b &intensity = grandeImageSansFond.at<Vec3b>(0, i);
            intensity.val[0]=255;
            intensity.val[1]=255;
            rightPos=i-1;
            ajoutBloc=false;
            cout<<" ["<<leftPos<<","<<rightPos<<"] "<<"w :"<<(rightPos-leftPos)<<endl;
            //on vérifie dans le vecteur des blocs si on peut ajouter ce bloc.
            for(vector<bloc*>::iterator it = theBlocs.begin(); it != theBlocs.end(); it++)
            {
                if((*it)->checkNewLine(leftPos,rightPos))
                {
                    //si on a réussis à ajouter la ligne à un bloc.
                    ajoutBloc=true;
                    //break;
                }
            }
            if(ajoutBloc==false)
            {
                //si on a pas réussis a ajouter la ligne non noire à un bloc, on crée un nouveau bloc.
                if(rightPos-leftPos>tailleMiniBloc)//on néglige les trop petits blocs.
                {
                    currentBloc=new bloc(leftPos,rightPos, footerWidth, footerHeight);
                    theBlocs.push_back(currentBloc);
                }

            }
            rightPos=0;
            leftPos=0;
        }
    }

    //on multiplie la matrice pour voir x* la même ligne pour y voir plus clair
    for(int i=0; i<30;i++)
        grandeImageSansFond.push_back(imageSansFond);

    imshow("grandeImageSansFond",grandeImageSansFond);
}

int Ligne::getNbFooters()
{
    return nbFooters;
}
//on fait le ménage dans les blocs (on supprime les blocs morts)
int Ligne::cleanTheBlocs()
{
    cout<<"clean **********************"<<endl;
    int newfooters=0;
    for(int i=0; i<theBlocs.size();i++)
    //for(int i=theBlocs.size();i>-1;i--)
    {
        if (theBlocs[i]->checkDead())
        {
            theBlocs[i]->deadBloc();
            newfooters += theBlocs[i]->getNbFooters();

            theBlocs.erase(theBlocs.begin()+i);
        }
        else//si le bloc est pas mort, on vérifie qu'il n'existe pas un autre bloc avec les même paramètre
        {
            for (int j=i+1; j<theBlocs.size();j++)
            {
                if(theBlocs[i]->checkDuplicate(theBlocs[j]))
                {
                    //theBlocs[j]->deadBloc();
                    theBlocs.erase(theBlocs.begin()+j);
                }
            }
        }
    }
    return newfooters;
}

void Ligne::setBgLearningRate(double bgLR)
{
    bgLearningRate = bgLR;
}

void Ligne::setBgThreshold(int bgThresh)
{
    bgSubtractor.setThreshold(bgThresh);
}

void Ligne::setDetectionThreshold(int detThresh)
{
    seuil = detThresh;
}
