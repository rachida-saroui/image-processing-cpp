/***********************************************************************
Exo 5 : Clustering and thesholding
Michele GOUIFFES
***********************************************************************/
#include "init.h"
using namespace cv;
using namespace std;



void InitWindows(int Width, int Height)
{
	// Creation de fenetres d'affichage
	namedWindow( "Image initiale", cv::WINDOW_AUTOSIZE);moveWindow("Image initiale", 0, Height);

	namedWindow( "Image apres conversion ", cv::WINDOW_AUTOSIZE ); moveWindow("Image apres conversion ", (Width), Height);

	namedWindow("Clustering", cv::WINDOW_AUTOSIZE ); moveWindow("Clustering", 2*(Width), Height);



}

int main()
{
    // read configuration file
    Configuration config;
	config.Load("../config_exo5.cfg");
    config.ReadConfigIm();    // Initialise
    config.ReadConfigStats();
    config.ReadConfigCluster();

    Mat im, im_e;
    bool display = false;
    for(int i = config.n0; i<= config.nend; i++){
        cout << "Image " << i << endl;


        Color c(config, i, im, true);
        InitWindows(im.cols, im.rows);
        imshow("Image initiale", im);

        Mat im_c =c.convert(im, config.colorspace, display);
        imshow("Image apres conversion ", im_c);

        Clustering cl;
        Mat J=cl.kmeans1D(im_c, config.K, 2); //rouge
        Display("Clustering", J);


        waitKey(0);
        destroyAllWindows();
    }

    return 1;
}


