#ifndef LIGNE_H
#define LIGNE_H

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

class Ligne
{
    Point P1, P2;
    Mat Data;
public:
    Ligne();
    Ligne(Point P1, Point P2) : P1(P1), P2(P2) {}

    Mat& getData() { return Data; }

    void extractFromImage(Mat image);
};

#endif // LIGNE_H
