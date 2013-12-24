#ifndef LIGNE_H
#define LIGNE_H

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

class Ligne
{
    Point P1, P2;
    Mat Data,Bgr;
public:
    Ligne();
    Ligne(Point P1, Point P2);

    Point getP1() const { return P1; }
    Point getP2() const { return P2; }
    const Mat& getData() const { return Data; }

    void extractFromImage(Mat image, Mat background);
    void substractBackground(Mat refImg,Mat CurrentImg, Mat &OutputImg);
    void detectionDesBlocs(Mat imageSansFond);
    string toString(int val);
};

#endif // LIGNE_H
