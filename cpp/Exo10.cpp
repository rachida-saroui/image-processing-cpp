#include "init.h"
#include "Filtering.h"  
#include "Segmentation.h"  
#include "Clustering.h"  
#include "Morpho.h"
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
    namedWindow("Image initiale", cv::WINDOW_AUTOSIZE);moveWindow("Image initiale", 0, 0);
    namedWindow("Image binaire", cv::WINDOW_AUTOSIZE); moveWindow("Image binaire", Width, 0);
    namedWindow("Image Segmentee", cv::WINDOW_AUTOSIZE); 	moveWindow("Image Segmentee", 2 * Width, 0);
    namedWindow("Contours des regions", cv::WINDOW_AUTOSIZE); moveWindow("Contours des regions", 3 * Width, 0);

}

int main(int argc, char** argv)
{
    // Initialise
    Configuration config;
    config.Load("../config_exo10.cfg");
    config.ReadConfigIm();
    config.ReadConfigStats();
    config.ReadConfigCluster();


    int s;
    config.Get("s", s);

    Mat im, im_e, im_b;
    bool display = false;

    InitWindows(400, 200);
    // Loop on the images
    for (int i = config.n0; i <= config.nend; i++) {
        cout << "Image " << i << endl;

        Color c(config, i, im, true);
        InitWindows(im.cols, im.rows);

        imshow("Image initiale", im);
        Mat im_c = c.convert(im, config.colorspace, display);
        imshow("Image convertie", im_c);
        Filtering filter;
        Segmentation segment;
        Morpho morpho;
        Clustering clustering;
         // Appliquez un filtre Gaussien pour lisser l'image
        Mat smoothed = filter.GaussKernel();
        Mat im_filtered = filter.convolve(im, smoothed);

        // Appliquez la segmentation par couleur en utilisant la méthode appropriée
        // Exemple : Si votre classe Clustering a une méthode pour K-means
        Mat im_clustered = clustering.kmeans2D(im_filtered, 2,1,2); // Ajustez les paramètres selon besoin

        // Choisissez le label correspondant au toit rouge
        float redRoofColor[] = { 0,10,100,255};
        Mat redRoofs = clustering.select_label(im_clustered, redRoofColor);

        // Utilisez la morphologie pour nettoyer l'image
        Mat redRoofsCleaned = morpho.opening(redRoofs);

        // Trouvez les contours des toits rouges
        Mat redRoofContours = morpho.contours(redRoofsCleaned);

        // Comptez le nombre de régions distinctes (bâtiments)
        Mat labeledBuildings = segment.etiquetage_cc(redRoofContours);
        int buildingCount = segment.nr; // Assumant que 'nr' est mis à jour pour refléter le nombre de régions distinctes

        // Affichez le nombre de bâtiments
        cout << "Nombre de bâtiments au toit rouge : " << buildingCount << endl;

        // Optionnel: Affichez les images traitées pour vérification
        segment.Display("Régions des Toits Rouges", redRoofs);
        segment.Display("Contours des Toits Rouges", redRoofContours);

        waitKey(0);

    }
}
