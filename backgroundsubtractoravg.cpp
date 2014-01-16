#include "backgroundsubtractoravg.h"

BackgroundSubtractorAvg::BackgroundSubtractorAvg()
{
    _initialized = false;
    _threshold = 10.0; //TODO adaptatif ou choisi par l'utilisateur
}

/* Compute the background subtraction on the current frame
 *
 * @param image : the input frame from the video stream
 * @param fgmask : the computed image with background subtracted
 *  /!\ image and fgmask must be 2 different buffers
 * @param learningRate : the value to use for exponential smoothing ;
 *      if learningRate = 0, the input image will have no effect on the background computation
 *      il learningRate = 1, the previous images will have no effect on the bg computation
 */
void BackgroundSubtractorAvg::operator()(InputArray image, OutputArray fgmask, double learningRate)
{
    //Check the parameters
    CV_Assert(learningRate >= 0.0 && learningRate <= 1.0);
    CV_Assert(image.type() == CV_8UC3);

    Mat currentFrame = image.getMat();

    if(!_initialized)
        initialize(currentFrame);

    //Gaussian blur on the current frame to limit spatial noise
    Mat blurredCurrent;
    GaussianBlur(currentFrame, blurredCurrent, Size(3,3), 0, 0);

    //TODO regarder sur une zone spatiale ?

    //Apply the adaptive exponential smoothing
    //_bgImage = learningRate*currentFrame + (1-learningRate)*_bgImage;
    MatConstIterator_<Vec3b> itCur = blurredCurrent.begin<Vec3b>(), itCur_end = blurredCurrent.end<Vec3b>();
    MatIterator_<Vec3b> itBg = _bgImage.begin<Vec3b>();

    //Parse the matrix
    for( ; itCur != itCur_end; ++itBg, ++itCur)
    {
        //if the pixel in the new image is too different we don't take it into
        //account in the bg image
        if(norm(*itCur, *itBg, NORM_L1) < _threshold)
            *itBg = learningRate*(*itCur)+(1-learningRate)*(*itBg);
    }

    //Output the difference between the current frame and the computed bg image
    absdiff(currentFrame, _bgImage, fgmask);
}

void BackgroundSubtractorAvg::getBackgroundImage(OutputArray bgImage) const
{
    _bgImage.copyTo(bgImage);
}

/* Initialize the internal background image to the first sample
 *
 * @param firstImage : the first image of the video stream will first
 * be used as the background image
 */
void BackgroundSubtractorAvg::initialize(InputArray firstImage)
{
    _bgImage = firstImage.getMat();

    _initialized = true;
}
