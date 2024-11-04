/***********************************************************************
Exo 6 : Morphologie Mathematique
Michele GOUIFFES
***********************************************************************/
#include "init.h"
using namespace cv;
using namespace std;



void InitWindows(int Width, int Height)
{
	// Creation de fenetres d'affichage
	namedWindow( "Image initiale", cv::WINDOW_AUTOSIZE);moveWindow("Image initiale", 0, 0);
	namedWindow( "Image binaire", cv::WINDOW_AUTOSIZE);moveWindow("Image binaire", 0, Height);
	namedWindow( "Image erodee", cv::WINDOW_AUTOSIZE ); moveWindow("Image erodee", (Width), 0);
    namedWindow( "Image dilatee", cv::WINDOW_AUTOSIZE ); moveWindow("Image dilatee", (Width), Height);
	namedWindow( "Image apres fermeture", cv::WINDOW_AUTOSIZE ); moveWindow("Image apres fermeture", 2*(Width), 0);
    namedWindow( "Image apres  ouverture", cv::WINDOW_AUTOSIZE ); moveWindow("Image apres  ouverture", 2*(Width), Height);
	namedWindow( "Contours interieurs", cv::WINDOW_AUTOSIZE ); moveWindow("Contours interieurs", 3*(Width), 0);


}

int main()
{
    // read configuration file
    Configuration config;
	config.Load("../config_exo6.cfg");
    config.ReadConfigIm();    // Initialise
    config.ReadConfigCluster();

    Mat im, im_e, im_b, im_b1;
    bool display = false;
    for(int i = config.n0; i<= config.nend; i++){
        cout << "Image " << i << endl;


        Color c(config, i, im, true);
        InitWindows(im.cols, im.rows);
        imshow("Image initiale", im);

        Mat im_c =c.convert(im, config.colorspace, display);


        Clustering cl;
        Mat J=cl.kmeans1D(im_c, config.K, 2);

        im_b=select_val(J,2);
        imshow("Image binaire", im_b);

        Morpho m;
        Mat im_e=m.erode(im_b, 1);
        Display("Image erodee", im_e);

        Mat im_d=m.dilate(im_b, 1);
        Display("Image dilatee", im_d);

         im_c=m.closing(im_b);
        Display("Image apres fermeture", im_c);

        Mat im_o=m.opening(im_b);
        Display("Image apres  ouverture", im_o);

        Mat im_co=m.contours(im_b);
        Display("Contours interieurs", im_co);


        waitKey(0);
        destroyAllWindows();
    }

    return 1;
}


