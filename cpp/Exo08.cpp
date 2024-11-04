/****************************************************************************
*  TP
*  DETECTION DE CONTOURS
*
*  Author : Michèle Gouiffès

****************************************************************************/

#include "init.h"
using namespace cv;
using namespace std;

void InitWindows(int Width, int Height)
{
	// Creation de fenetres d'affichage
	namedWindow( "1-Initial Image", cv::WINDOW_AUTOSIZE);moveWindow("1-Initial Image", 0, 0);
	namedWindow( "2-Sobel Gx", cv::WINDOW_AUTOSIZE ); moveWindow("2-Sobel Gx", Width, 0);
	namedWindow( "3-Sobel Gy", cv::WINDOW_AUTOSIZE ); 	moveWindow("3-Sobel Gy", 2*Width, 0);
	namedWindow( "4-Simple thresholding", cv::WINDOW_AUTOSIZE ); moveWindow("4-Simple thresholding", 3*Width, 0);
	namedWindow( "5-Hysteresis thresholding", cv::WINDOW_AUTOSIZE ); moveWindow("5-Hysteresis thresholding", 0, 120);
	namedWindow( "6-Gaussian Filtering", cv::WINDOW_AUTOSIZE ); moveWindow("6-Gaussian Filtering", Width, Height);
	namedWindow( "7-Laplacian of Gaussian", cv::WINDOW_AUTOSIZE ); moveWindow("7-Laplacian of Gaussian", 2*Width, Height);
	namedWindow( "8-Zeros", cv::WINDOW_AUTOSIZE ); moveWindow("8-Zeros", 3*Width, Height);
}

int main( int argc, char** argv )
{
    // Initialise
    Configuration config;
	config.Load("../config_exo8.cfg");
    config.ReadConfigIm();
    Filtering flt(config);

    int s, sh, sb;
    config.Get("s", s);
    config.Get("sh", sh);
    config.Get("sb", sb);

    //filtering kernels
    Mat Kgauss =  flt.GaussKernel();
    Mat Klapla =flt.LaplacianKernel();

    Mat  Kx, Ky ;
    flt.SobelKernel(Kx, Ky);

    Mat I, Gx, Gy, Ilog, Igauss, In, Itheta, Ic;

    InitWindows(400, 200);
	// Loop on the images
	for(int n=config.n0; n<=config.nend; n++){

        Color c(config, n, I, false);
         Display( "1-Initial Image", I);


        Gx = flt.convolve(I, Kx);
        Gy = flt.convolve(I, Ky);
        flt.gradientNorm(Gx, Gy, In, Itheta );

        Display("2-Sobel Gx", Gx);
        Display("3-Sobel Gy", Gy);

        threshold( In, Ic, s, 255, CV_8U);
        Display( "4-Simple thresholding", Ic);
//
        Clustering cl;
        Ic=cl.hysteresis(In, sb, sh);
        Display("5-Hysteresis thresholding", Ic);

        Igauss = flt.convolve(I, Kgauss);
        Ilog = flt.convolve(Igauss, Klapla);

        Morpho m;
        Ic=m.zeros(Ilog);

        Display("6-Gaussian Filtering", Igauss);
        Display("7-Laplacian of Gaussian", Ilog);
        Display("8-Zeros", Ic);
        waitKey(0);

}
	}





