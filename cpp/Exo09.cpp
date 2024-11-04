
#include "init.h"
using namespace cv;
using namespace std;





//  type :1 : histo
/****************************************************************************
*  TP
*  ANALYSE de FORMES
*
*  Author : Michèle Gouiffès

****************************************************************************/


using namespace cv;
using namespace std;



void InitWindows(int Width, int Height)
{
	// Creation de fenetres d'affichage
	namedWindow( "Image initiale", cv::WINDOW_AUTOSIZE);moveWindow("Image initiale", 0, 0);
	namedWindow( "Image binaire", cv::WINDOW_AUTOSIZE ); moveWindow("Image binaire", Width, 0);
	namedWindow( "Image Segmentee", cv::WINDOW_AUTOSIZE ); 	moveWindow("Image Segmentee", 2*Width, 0);
	namedWindow( "Contours des regions", cv::WINDOW_AUTOSIZE ); moveWindow("Contours des regions", 3*Width, 0);

}

int main( int argc, char** argv )
{
    // Initialise
    Configuration config;
	config.Load("../config_exo9.cfg");
    config.ReadConfigIm();
    config.ReadConfigStats();
    config.ReadConfigCluster();


    int s;
    config.Get("s", s);

    Mat im, im_e, im_b;
    bool display = false;

    InitWindows(400, 200);
	// Loop on the images
	for(int i=config.n0; i<=config.nend; i++){
       cout << "Image " << i << endl;

        Color c(config, i, im, true);
        InitWindows(im.cols, im.rows);


        Mat im_c =c.convert(im, config.colorspace, display);
        imshow("Image initiale", im_c);

        Clustering cl;
        im_b=cl.kmeans1D(im_c, config.K, 2);
        im_b=select_val(im_b,2);
        Display("Image binaire", im_b);

        Segmentation s(im);

        Mat im_r=s.regions(im_b);
        s.Display("Image Segmentee", im_r);
        im_c=s.contours(im_r, s.nr);
        s.Display("Contours des regions", im_c);

        Shape sh(s);
        sh.compute(s);
        sh.analysis();

        waitKey(0);

}
	}





