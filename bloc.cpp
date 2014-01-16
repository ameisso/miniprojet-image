#include "bloc.h"
#include "fonctions.h"


bloc::bloc(int leftPos_, int rightPos_, int footerWidth_, int footerHeight_, Mat matriceFooters_, int matriceFootersLeftPos_,
           int matriceFootersRightPos_)
    : matriceFooters(matriceFooters_), matriceFootersLeftPos(matriceFootersLeftPos_), matriceFootersRightPos(matriceFootersRightPos_)
{
    lastRightPos = rightPos_;
    lastLeftPos = leftPos_;
    footerWidth = footerWidth_;
    footerHeight = footerHeight_;

    maxBlocWidth=lastRightPos-lastLeftPos;
   // cout<<"new bloc ["<<lastLeftPos<<","<<lastRightPos<<"] widht :"<<maxBlocWidth<<endl;
    blocHeight=1;
    dead=false;
}

//Fonction qui prend en paramètre les position des valeurs non noires de la matrice "extractedLineNoBackground" et qui vérifie si elle appartient a ce bloc.
//elle retourne true si le bloc proposé appartient à ce bloc, false sinon.
bool bloc::checkNewLine(int leftPos, int rightPos)
{
    if(((rightPos<=lastRightPos) && (rightPos>=lastLeftPos))|((leftPos>=lastLeftPos) && (leftPos <=lastRightPos))|((leftPos<=lastLeftPos)&&(rightPos>lastRightPos)))
    {
        //cout<<leftPos<<","<<rightPos<<" owned by ["<<lastLeftPos<<","<<lastRightPos<<"] widht :"<<maxBlocWidth<<endl;
        blocHeight++;// on augmente le nombre de frames pour mesurer la longeur du bloc.(+1);
        if (rightPos-leftPos>maxBlocWidth)
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
//Fonction qui calcule le nombre de personnes dans le bloc et incrémente le ur.
void bloc::deadBloc()
{
    int nbFooterInWidth=maxBlocWidth/footerWidth;
    int nbFooterInHeight=blocHeight/footerHeight;

    if(nbFooterInHeight!=0 && nbFooterInWidth!=0)
    {
        nbFooters=nbFooterInWidth+nbFooterInHeight;
        cout<<nbFooterInWidth<<"+"<<nbFooterInHeight<<"piétons comptés dans le bloc";


        //Pour tester: Afficher et attendre
        imshow("XXX", matriceFooters);
        waitKey();

        //Enregistrer
        imwrite("bloc_" + intToString(lastLeftPos) + "_" + intToString(lastRightPos) + ".png", matriceFooters);


        toString();
    }
    else
    {
        nbFooters=0;
        //cout<<"pas de nouveaux piétons"<<endl;
    }
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
        //cout<<" deleted ["<<lastLeftPos<<","<<lastRightPos<<"]"<<endl;
    return dead;
}
void bloc::setDead(bool val)
{
    dead=val;
}
//Fonction qui renvoie true si un bloc est équivalent au bloc testé
bool bloc::checkDuplicate  (bloc *testedBloc)
{
    if ((testedBloc->lastRightPos=lastRightPos) && (testedBloc->lastLeftPos==lastLeftPos))
    {
        //cout<<"equivalent bloc found ["<<lastLeftPos<<","<<lastRightPos<<"]"<<endl;
        //on copie les plus grandes valeurs dans ce bloc.
        maxBlocWidth=max<int>(maxBlocWidth,testedBloc->maxBlocWidth);
        blocHeight=max<int>(blocHeight,testedBloc->blocHeight);
        return true;
    }
}


/*
 * Creer la matrice matriceFooter pour
 *
 * Maintenant, ca ne marche pas tres bien.
 * L'image de la matrice est pas optimale, c'est p-e difficile de faire un traitement avec ca?
 *
 * Possibilites du probleme:
 * La fonction Ligne::DetectionBloc ne detecte pas tous d'un personne qui marche par la "ligne"
 *
 * Affichage de la matrice dans la fonction ::deadBloc()
 *
 **/
void bloc::pushBackToMatriceFooters(Mat mat, int leftPos, int rightPos)
{
    //On cherche les positions situees le plus a gauche et le plus a droite.
    //Ca la nouvelle taille de la matrice en direction x (nombre rows)
    int minVal = min<int>(leftPos, matriceFootersLeftPos);
    int maxVal = max<int>(rightPos, matriceFootersRightPos);
    int nbCols = maxVal - minVal;

//    cout << minVal << "..." << maxVal << endl;


    //Si leftPos < matriceFootersLeftPos il faut creer une nouvelle matrice "matriceFooters"
    //et ajouter la matrice originale "matriceFooters" a la position row=0, col=matriceFootersLeftPos-LeftPos
    if(leftPos < matriceFootersLeftPos)
    {

        Mat _matriceFooters(matriceFooters);
        matriceFooters = Mat::zeros(_matriceFooters.rows, nbCols, CV_8UC3);

        int pos1 = matriceFootersLeftPos - leftPos;
        int pos2 = pos1 + _matriceFooters.cols;

/*
        //Debugging:
        cout << "#" << matriceFooters.rows << endl;
        cout << "CAS 1" << endl;

        cout << "matriceFooter: " << matriceFooters.cols << "; " << matriceFootersLeftPos << endl;
        cout << "mat:           " << mat.cols << "; " << leftPos << endl;

        cout << "right " << matriceFootersRightPos << " | mat_right " << rightPos << endl;
        cout << "pos1 " << pos1 << " ; pos2 " << pos2 << endl;
        cout << "matriceFooters.cols " << matriceFooters.cols << endl;
*/

        //Ajouter la matrice originalle
        _matriceFooters.copyTo(matriceFooters.colRange(pos1, pos2));

        //Enregistrer la nouvelle taille!
        matriceFootersLeftPos = minVal;
        matriceFootersRightPos = maxVal;

        //Preparer une matrice (=vecteur ligne) avec la taille de la matrice qu'avant
        //pour ajouter dans la matriceFooters
        // Mat _mat = Mat(1, maxCols, CV_8UC3);
        Mat _mat = Mat::zeros(1, nbCols, CV_8UC3);

        //Copier la matrice mat(:) a _mat(0:mat.cols)
        mat.copyTo(_mat.row(0).colRange(leftPos - matriceFootersLeftPos, mat.cols));

        //Ajouter
        matriceFooters.push_back(_mat);

    }
    else
    {
        //On ajoute des pixels seulement dans la taille ::cols ou a droit. Alors, on doit pas decaler la matrice.
        //Ca suffit, d'ajouter qq cols a droite et faire un push_back.

/*
        //Debugging:
        cout << "#" << matriceFooters.rows << endl;
        cout << "CAS 2" << endl;

        cout << "matriceFooter: " << matriceFooters.cols << "; " << matriceFootersLeftPos << endl;
        cout << "mat:           " << mat.cols << "; " << leftPos << endl;

        cout << "right " << matriceFootersRightPos << " | mat_right " << rightPos << endl;
        cout << "matriceFooters.cols " << matriceFooters.cols << endl;

*/

        //Changer la taille de la matrice
        int c = matriceFooters.cols;
        matriceFooters.create(matriceFooters.rows, nbCols, CV_8UC3);
        matriceFooters.colRange(c, nbCols) = Scalar(0, 0, 0); //pour supprimer les pixels non-initialises

        //Enregistrer la nouvelle taille!
        matriceFootersRightPos = maxVal;


        //Preparer une matrice (=vecteur ligne) avec la taille de la matrice qu'avant
        //pour ajouter dans la matriceFooters
        // Mat _mat = Mat(1, maxCols, CV_8UC3);
        Mat _mat = Mat::zeros(1, nbCols, CV_8UC3);

        //Copier la matrice mat(:) a _mat(0:mat.cols)
        mat.copyTo(_mat.row(0).colRange(leftPos - matriceFootersLeftPos, leftPos - matriceFootersLeftPos + mat.cols));

        //Ajouter
        matriceFooters.push_back(_mat);
    }
}
