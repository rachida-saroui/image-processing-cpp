/***********************************************************************
Exo 4 Image, convolution et filtrage
Michele GOUIFFES
***********************************************************************/
#include "init.h"
using namespace cv;
using namespace std;



void InitWindows(int Width, int Height)
{
	// Creation de fenetres d'affichage
	namedWindow( "Initial Image", cv::WINDOW_AUTOSIZE);moveWindow("Initial Image", 0, 0);

	namedWindow( "Filtrage gaussien", cv::WINDOW_AUTOSIZE ); moveWindow("Filtrage gaussien", (Width), 0);
    namedWindow( "Filtrage median", cv::WINDOW_AUTOSIZE ); moveWindow("Filtrage median", (Width), Height);

    namedWindow( "Gradient x", cv::WINDOW_AUTOSIZE ); moveWindow("Gradient x", 2*(Width), 0);
    namedWindow( "Gradient y", cv::WINDOW_AUTOSIZE ); moveWindow("Gradient y", 2*(Width), Height);
    namedWindow( "Sobel", cv::WINDOW_AUTOSIZE ); moveWindow("Sobel", 2*(Width), 2*Height);

}

int main()
{
    // read configuration file
    Configuration config;
	config.Load("../config_exo4b.cfg");
    config.ReadConfigIm();
    int W;
    config.Get("W", W);

    Mat im;
    bool display = false;
    for(int i = config.n0; i<= config.nend; i++){
        cout << "Image " << i << endl;
        //image en niveaux de gris
        Color c(config, i, im, false);
        InitWindows(im.cols, im.rows);
        imshow("Initial Image", im);


        Filtering filter(config);
        Mat Kg;
        Kg=filter.GaussKernel ();
        Mat Igauss=filter.convolve(im, Kg);
        Display("Filtrage gaussien", Igauss);

       Mat Imedian;
        medianBlur(im, Imedian, W );
        imshow("Filtrage median", Imedian);


        Mat Kx, Ky;
        filter.SobelKernel (Kx, Ky);
        Mat Gx=filter.convolve(im, Kx);
        Mat Gy=filter.convolve(im, Ky);
        Mat N, A;
        filter.gradientNorm(Gx,Gy, N, A );
        Display("Sobel", N);
        Display("Gradient x", Gx);
        Display("Gradient y", Gy);
        waitKey(0);
        destroyAllWindows();
    }

    return 1;
}

