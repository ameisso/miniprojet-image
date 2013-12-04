#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"


#include <cstdio>
#include <iostream>

using namespace cv;
using namespace std;


int main()
{
    Mat lastImage;//derière image chargé dans le projet -> celle que l'on va traiter
    cout<<"ca marche"<<endl;
    lastImage = imread("data/detection_0200.jpeg", 1);
    if(lastImage.empty() )
    {
        cout << "Couldn't open image "<<endl;
        return 0;
    }
    namedWindow("org");
    imshow("org",lastImage);
    cvWaitKey();
}
