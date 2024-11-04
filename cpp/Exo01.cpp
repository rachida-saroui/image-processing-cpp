/***********************************************************************
Exo 1 Image, couleur, histogrammes
Michele GOUIFFES
***********************************************************************/
#include "init.h"
using namespace cv;
using namespace std;
Mat im_s1, im_s2;
Mat im ;
const int th_max = 255;
int th_1=0, th_2=256, th_3=0, th_4=256, th_5=0, th_6=256;



/* Display sliders, image and the 3 histograms*/
void DisplayExo1(string s, Mat &I, Mat &H1, Mat &H2, Mat &H3, Mat &H2_1, Mat &H2_2, Mat &H2_3)
{
    Mat mosaic(testMax(I.rows, H1.rows) +100 +H2_1.rows, 3*H1.cols +I.cols , CV_8UC3);
    Zeros(mosaic);
    double minVal, maxVal;
    minMaxLoc(I, &minVal, &maxVal);
    Mat Im2;
    if(maxVal!=minVal) I.convertTo(Im2, (CV_8U), 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));
    else I.convertTo(Im2, (CV_8U), 1, 0);

    cvtColor(Im2, Im2, COLOR_GRAY2RGB);
    Rect dest(0,0,I.cols,I.rows);

    // first row : image + 3 histos 1D
    Im2.copyTo(mosaic(dest));

    dest=Rect(I.cols,0, H1.cols, H1.rows);
    H1.copyTo(mosaic(dest));

    dest=Rect(I.cols+H1.cols, 0, H2.cols, H2.rows);
    H2.copyTo(mosaic(dest));

    dest=Rect(I.cols+H1.cols+H2.cols, 0, H3.cols, H3.rows);
    H3.copyTo(mosaic(dest));

    putText(mosaic, "Channel 0", cv::Point(I.cols+ 10,          testMax(I.rows, H1.rows) +25),cv::FONT_HERSHEY_SCRIPT_SIMPLEX,  1.0, CV_RGB(255, 0, 0), 2);
    putText(mosaic, "Channel 1", cv::Point(I.cols + H1.cols+ 10, testMax(I.rows, H1.rows) +25),cv::FONT_HERSHEY_SCRIPT_SIMPLEX,  1.0, CV_RGB(0, 255, 0), 2);
    putText(mosaic, "Channel 2", cv::Point(I.cols + 2*H1.cols +10, testMax(I.rows, H1.rows) +25 ),cv::FONT_HERSHEY_SCRIPT_SIMPLEX,  1.0, CV_RGB(0, 0, 255), 2);

    // 2nd row :  3 histos 2D
    dest=Rect(I.cols, testMax(I.rows, H1.rows)+50,  H2_1.cols, H2_1.rows);
    H2_1.copyTo(mosaic(dest));
    dest=Rect(I.cols+H2_1.cols, testMax(I.rows, H1.rows)+50,  H2_1.cols, H2_1.rows);
    H2_2.copyTo(mosaic(dest));
    dest=Rect(I.cols+2*H2_1.cols, testMax(I.rows, H1.rows)+50,  H2_1.cols, H2_1.rows);
    H2_3.copyTo(mosaic(dest));

    putText(mosaic, "Channels 0 1", cv::Point(I.cols+ 10,          testMax(I.rows, H1.rows) + H1.rows+ 50),cv::FONT_HERSHEY_SCRIPT_SIMPLEX,  1.0, CV_RGB(255, 0, 0), 2);
    putText(mosaic, "Channels 1 2", cv::Point(I.cols + H1.cols+ 10, testMax(I.rows, H1.rows) + H1.rows+ 50),cv::FONT_HERSHEY_SCRIPT_SIMPLEX,  1.0, CV_RGB(0, 255, 0), 2);
    putText(mosaic, "Channels 2 0", cv::Point(I.cols + 2*H1.cols +10, testMax(I.rows, H1.rows) + H1.rows+ 50),cv::FONT_HERSHEY_SCRIPT_SIMPLEX,  1.0, CV_RGB(0, 0, 255), 2);


    imshow(s, mosaic);
}



/*Sliders for 6 thresholds parameters  */
static void on_trackbar(int, void*)
{
        Clustering cl;
        Color c;
        Stats st;
        Mat channel, im_s;
        channel= c.select_channel(im, 0);
        Mat im_s0=cl.threshold(channel, th_1, th_2);
        Mat H1 = st.histo_1D(channel, 0, th_1, th_2, false);

        channel= c.select_channel(im, 1);
        Mat H2 = st.histo_1D(channel, 1, th_3, th_4, false);
        Mat im_s1=cl.threshold(channel,th_3, th_4);

        channel= c.select_channel(im, 2);
        Mat H3 = st.histo_1D(channel, 2, th_5, th_6, false);
        Mat im_s2=cl.threshold(channel, th_5, th_6);

        bitwise_and(im_s0, im_s1, im_s);
        bitwise_and(im_s, im_s2, im_s);

        Mat H2D1 = st.histo_2D( im, 0, 1, th_1, th_2, th_3, th_4, false);
        Mat H2D2 = st.histo_2D( im, 1, 2, th_3, th_4, th_5, th_6,false);
        Mat H2D3 = st.histo_2D( im, 2, 0, th_5, th_6, th_1, th_2, false);

        DisplayExo1("Image seuillee", im_s, H1, H2, H3, H2D1, H2D2, H2D3);

}

void InitWindows(int Width, int Height)
{
	// Creation de fenetres d'affichage
	namedWindow( "1-Initial Image", cv::WINDOW_AUTOSIZE);moveWindow("1-Initial Image", 0, 0);
	namedWindow( "2-Color space change ", cv::WINDOW_AUTOSIZE ); moveWindow("2-Color space change ", 0, Height);
    namedWindow( "2-Color 0", cv::WINDOW_AUTOSIZE ); moveWindow("2-Color 0", Width, 0);
    namedWindow( "2-Color 1", cv::WINDOW_AUTOSIZE ); moveWindow("2-Color 1", Width, Height);
    namedWindow( "2-Color 2", cv::WINDOW_AUTOSIZE ); moveWindow("2-Color 2", Width, 2*Height);
	namedWindow( "Image seuillee", cv::WINDOW_AUTOSIZE ); moveWindow("Image seuillee", 4*Width, 0);

}

int main()
{
    // read configuration file
    Configuration config;
	config.Load("../config_exo1b.cfg");
    config.ReadConfigIm();    // Initialise


    Mat RGB;
    bool display = false;
    for(int i = config.n0; i<= config.nend; i++){
        cout << "Image " << i << endl;

        //recuperatio des parametres dans le fichier de configuration
        Color c(config, i, RGB, true);
        //Initialisation des fenetres
        InitWindows(RGB.cols, RGB.rows);
        imshow("1-Initial Image", RGB);
        //changement espace couleur
        im =c.convert(RGB, config.colorspace, display);
        imshow("2-Color space change ", im);
        Mat channel;
        Stats st;
        //affichage des images de canaux
        channel= c.select_channel(im, 0);
        imshow("2-Color 0", channel);
        channel= c.select_channel(im, 1);
        imshow("2-Color 1", channel);
        channel= c.select_channel(im, 2);
        imshow("2-Color 2", channel);


        channel= c.select_channel(im, 0);

        createTrackbar("C0 min", "Image seuillee", &th_1, th_max, on_trackbar);
        createTrackbar("C0 max", "Image seuillee", &th_2, th_max, on_trackbar);
        createTrackbar("C1 min", "Image seuillee", &th_3, th_max, on_trackbar);
        createTrackbar("C1 max", "Image seuillee", &th_4, th_max, on_trackbar);
        createTrackbar("C2 min", "Image seuillee", &th_5, th_max, on_trackbar);
        createTrackbar("C2 max", "Image seuillee", &th_6, th_max, on_trackbar);

        // call first time, to show image (view callback function)
        on_trackbar(th_1, 0);

        waitKey(0);
        destroyAllWindows();
    }

    return 1;
}

