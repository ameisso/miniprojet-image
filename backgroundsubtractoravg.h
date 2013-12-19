#ifndef BACKGROUNDSUBTRACTORAVG_H
#define BACKGROUNDSUBTRACTORAVG_H

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/background_segm.hpp"

/* extends openCV's BackgroundSubtractor abstract class to implement
 * a background subtraction algorithm based on adaptive moving average
 *
 * An exponential moving average is used because of its simplicity
 * (no need to record the previous values)
 *
 * Tom Magnier - 19/12/13
 */

using namespace cv;
using namespace std;

class BackgroundSubtractorAvg : public cv::BackgroundSubtractor
{
public:
    BackgroundSubtractorAvg();
    ~BackgroundSubtractorAvg() {}
    void operator()(InputArray image, OutputArray fgmask, double learningRate=0);
    void getBackgroundImage(OutputArray bgImage) const;
};

#endif // BACKGROUNDSUBTRACTORAVG_H
