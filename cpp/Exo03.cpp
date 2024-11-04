/***********************************************************************
Exo 3 Représentation fréquentielle et filtrage

Michele GOUIFFES
***********************************************************************/
#include "init.h"
using namespace cv;
using namespace std;

#define WIDTH_SCREEN 1000


void InitWindows(int Width, int Height)
{
	// Creation de fenetres d'affichage
	namedWindow( "Initial Image", cv::WINDOW_AUTOSIZE);moveWindow("Initial Image", 0, 0);
	namedWindow( "FFT", cv::WINDOW_AUTOSIZE ); moveWindow("FFT", 0, Height);
    namedWindow( "FFT passe-bas", cv::WINDOW_AUTOSIZE ); moveWindow("FFT passe-bas", Width, 0);
    namedWindow( "FFT passe-haut", cv::WINDOW_AUTOSIZE ); moveWindow("FFT passe-haut", Width, Height);

	namedWindow( "Image filtree Passe-bas", cv::WINDOW_AUTOSIZE ); 	moveWindow("Image filtree Passe-bas", 2*Width, 0);
	namedWindow( "Image filtree Passe-haut", cv::WINDOW_AUTOSIZE ); moveWindow("Image filtree Passe-haut", 2*Width, Height);

}

int main()
{
    // read configuration file
    Configuration config;
	config.Load("../config_exo3.cfg");
    config.ReadConfigIm();
    config.ReadConfigStats();

    config.ReadConfigFilter();

    Mat im;
    bool display = false;
    for(int i = config.n0; i<= config.nend; i++){
        cout << "Image " << i << endl;
        //image en niveaux de gris
        Color c(config, i, im, false);
        InitWindows(im.cols, im.rows);
        imshow("Initial Image", im);

        Frequency f(im);
        Mat S, F, Fs, F_lp, F_hp;
        F=f.fft_z(im);
        Fs=f.fftshift(F);
        f.displayTFD( "FFT");

        F_lp= f.GaussLowPass(Fs, config.Fb, config.Fb);
        f.displayTFD( "FFT passe-bas");

        F_lp=f.fftshift(F_lp);
        Mat I_lp=f.ifft(F_lp);
        Display("Image filtree Passe-bas", I_lp);

        F_hp= f.GaussHighPass(Fs, config.Fh, config.Fh);
        f.displayTFD( "FFT passe-haut");
        F_hp=f.fftshift(F_hp);
        Mat I_hp=f.ifft(F_hp);
        Display("Image filtree Passe-haut", I_hp);

        waitKey(0);
        destroyAllWindows();
    }


    return 1;
}

