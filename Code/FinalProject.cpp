#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <utility>
#include <QtGui>
#include <algorithm>
#include "stdlib.h"
#include <vector>
#include "math.h"
#include <iostream>
#include <map>


//**********************************************************************************
//Method: restrictor
//Purpose:set the limitation of the rgb value of pixel
//parameter type:
//return: QRgb
//**********************************************************************************


QRgb restrictColor(double red, double green, double blue)
{
    int r = (int)(floor(red+0.5));
    int g = (int)(floor(green+0.5));
    int b = (int)(floor(blue+0.5));

    return qRgb(max(0, min(255, r)), max(0, min(255, g)), max(0, min(255, b)));
}


//**********************************************************************************
//Method: CovertQImage2Double
//Purpose: Convert QImage to a matrix of size (imageWidth*imageHeight)*3 having double values
//parameter type : QImage
//return: None
//**********************************************************************************


void MainWindow::ConvertQImage2Double(QImage image)
{
    // Global variables to access image width and height
    imageWidth = image.width();
    imageHeight = image.height();

    // Initialize the global matrix holding the image
    // This is how you will be creating a copy of the original image inside a function
    // Note: 'Image' is of type 'double**' and is declared in the header file (hence global variable)
    // So, when you create a copy (say buffer), write "double** buffer = new double ....."
    Image = new double* [imageWidth*imageHeight];
    for (int i = 0; i < imageWidth*imageHeight; i++)
            Image[i] = new double[3];

    // For each pixel
    for (int r = 0; r < imageHeight; r++)
        for (int c = 0; c < imageWidth; c++)
        {
            // Get a pixel from the QImage form of the image
            QRgb pixel = image.pixel(c,r);

            // Assign the red, green and blue channel values to the 0, 1 and 2 channels of the double form of the image respectively
            Image[r*imageWidth+c][0] = (double) qRed(pixel);
            Image[r*imageWidth+c][1] = (double) qGreen(pixel);
            Image[r*imageWidth+c][2] = (double) qBlue(pixel);
        }
}
// Round float values to the nearest integer values and make sure the value lies in the range [0,255]


//**********************************************************************************
//Method: CovertDouble2QImage
//Purpose: Convert Convert the matrix form of the image back to QImage for display
//parameter type : QImage
//return: None
//**********************************************************************************


void MainWindow::ConvertDouble2QImage(QImage *image)
{
    for (int r = 0; r < imageHeight; r++)
        for (int c = 0; c < imageWidth; c++)
            image->setPixel(c, r, restrictColor(Image[r*imageWidth+c][0], Image[r*imageWidth+c][1], Image[r*imageWidth+c][2]));
}


//**********************************************************************************
//Method: RandomSeed clustering
//Purpose: Perform K-means clustering on a color image using random seeds
//parameter type : QImage, int
//return: None
//**********************************************************************************


void RandomSeed(QImage *image, int num_clusters, int maxiteration)
{
           int width = image->width(), height = image->height();
           int a = maxiteration;

           std::vector<QRgb> centers_point;
           std::vector<QRgb> new_cenpoint;
           std::vector<int> iter_dis;
           QImage temp = image->copy();
           int i = 0;
           while (i < num_clusters)
           {
               int red;
               int green;
               int blue;
               red = rand()%256;
               green = rand()%256;
               blue = rand()%256;
               //QRgb centerpoint = qRgb(red, green, blue);
               new_cenpoint.push_back(qRgb(red, green, blue));
               centers_point.push_back(qRgb(red, green, blue));

               i++;
           }
           int j = 0;
           while (j < a){
                   iter_dis.clear();
                   for (int h = 0; h < height; h++){
                       for (int w = 0; w < width; w++){
                           int maxdistance = 10000000;
                           int index = 0;
                           for (int i = 0; i < num_clusters; i++){
                                int d_red=abs(qRed(centers_point[i]) - qRed(temp.pixel(w, h)));
                                int d_green=abs(qGreen(centers_point[i]) - qGreen(temp.pixel(w, h)));
                                int d_blue=abs(qBlue(centers_point[i]) - qBlue(temp.pixel(w, h)));
                                bool check= (d_red+d_green+d_blue) < maxdistance;
                                maxdistance = ((d_red+d_green+d_blue) < maxdistance) ? (d_red+d_green+d_blue) : maxdistance;
                                index = check ? i : index;
                           }
                           iter_dis.push_back(index);
                       }
                   }
                   std::vector<int> num;
                   std::vector<int> R;
                   std::vector<int> G;
                   std::vector<int> B;
                   for (int count = 0; count < num_clusters; count++){
                           num.push_back(0);
                           R.push_back(0);
                           G.push_back(0);
                           B.push_back(0);
                   }
                   for (int h = 0; h < height; h++){
                       for (int w = 0; w < width; w++){
                           int index=iter_dis[h * width + w];
                           num[index]++;
                           R[index] = R[index] + qRed(temp.pixel(w,h));
                           G[index] = G[index] + qGreen(temp.pixel(w,h));
                           B[index] = B[index] + qBlue(temp.pixel(w,h));
                       }
                   }
                   for (int d = 0;  d < num_clusters; d++){
                       if (num[d] == 0)
                           continue;
                       else{
                           int a = R[d]/num[d];
                           int b = G[d]/num[d];
                           int c= B[d]/num[d];
                           new_cenpoint[d] = qRgb(a,b,c);
                       }
                   }
                   centers_point = new_cenpoint;
                   j++;
           }
           for (int h = 0; h < height; h++){
               for (int w = 0; w < width; w++){
                   int r = iter_dis[h * width + w];
                   int g = iter_dis[h * width + w];
                   int b = iter_dis[h * width + w];
                   image->setPixel(w, h, qRgb(r,g,b));
                }
          }
}


//**********************************************************************************
//Method: uf_union, uf_find and conrgn
//Purpose: CODE FOR FINDING CONNECTED COMPONENTS
//parameter type : QImage, int
//return: None
//**********************************************************************************


#include "utils.h"

#define MAX_LABELS 80000

#define I(x,y)   (image[(y)*(width)+(x)])
#define N(x,y)   (nimage[(y)*(width)+(x)])

void uf_union( int x, int y, unsigned int parent[] )
{
    while ( parent[x] )
        x = parent[x];
    while ( parent[y] )
        y = parent[y];
    if ( x != y ) {
        if ( y < x ) parent[x] = y;
        else parent[y] = x;
    }
}

int next_label = 1;

int uf_find( int x, unsigned int parent[], unsigned int label[] )
{
    while ( parent[x] )
        x = parent[x];
    if ( label[x] == 0 )
        label[x] = next_label++;
    return label[x];
}

void conrgn(int *image, int *nimage, int width, int height)
{
    unsigned int parent[MAX_LABELS], labels[MAX_LABELS];
    int next_region = 1, k;

    memset( parent, 0, sizeof(parent) );
    memset( labels, 0, sizeof(labels) );

    for ( int y = 0; y < height; ++y )
    {
        for ( int x = 0; x < width; ++x )
        {
            k = 0;
            if ( x > 0 && I(x-1,y) == I(x,y) )
                k = N(x-1,y);
            if ( y > 0 && I(x,y-1) == I(x,y) && N(x,y-1) < k )
                k = N(x,y-1);
            if ( k == 0 )
            {
                k = next_region; next_region++;
            }
            if ( k >= MAX_LABELS )
            {
                fprintf(stderr, "Maximum number of labels reached. Increase MAX_LABELS and recompile.\n"); exit(1);
            }
            N(x,y) = k;
            if ( x > 0 && I(x-1,y) == I(x,y) && N(x-1,y) != k )
                uf_union( k, N(x-1,y), parent );
            if ( y > 0 && I(x,y-1) == I(x,y) && N(x,y-1) != k )
                uf_union( k, N(x,y-1), parent );
        }
    }
    for ( int i = 0; i < width*height; ++i )
        if ( nimage[i] != 0 )
            nimage[i] = uf_find( nimage[i], parent, labels );

    next_label = 1; // reset its value to its initial value
    return;
}


//**********************************************************************************
//Method: ExtractFeatureVector
//Purpose: Extract the feature vector for each final region that we use to compare
//parameter type : QImage
//return: None
//**********************************************************************************


std::vector<double*> MainWindow::ExtractFeatureVector(QImage image)
{


    int num_clusters = 5;
    int max_iterations = 50;

    QImage image_copy = image;
    ConvertQImage2Double(image_copy);//Image=image_copy
    //Run the color clustering on it to obtain a labeled cluter image
    RandomSeed(&image_copy,num_clusters,max_iterations);


    int r, c, w = image_copy.width(), h = image_copy.height();
    int *img = (int*)malloc(w*h * sizeof(int));
    memset( img, 0, w * h * sizeof( int ) );
    int *nimg = (int*)malloc(w*h *sizeof(int));
    memset( nimg, 0, w * h * sizeof( int ) );

    for (r=0; r<h; r++)
        for (c=0; c<w; c++)
            img[r*w + c] = qRed(image_copy.pixel(c,r));

    conrgn(img, nimg, w, h);

    int num_regions=0;
    for (r=0; r<h; r++){
        for (c=0; c<w; c++){
            num_regions = (nimg[r*w+c]>num_regions)? nimg[r*w+c]: num_regions;
        }
    }


    //#################### Clean Noise ##################################
    int* RS = new int[num_regions+1];
    memset(RS, 0, sizeof(int)*(num_regions+1) );
    int size=h*w;
    for(int i=0;i<size;i++){
        int index=nimg[i];//index of RGN
        RS[index]=RS[index]+1;
    }


    int count = 0;
    int* newR = new int [num_regions+1];
    int temp_index=0;
    for (int i=0; i< num_regions; i++)
    {
        temp_index=i+1;
        bool check = RS[temp_index] > 50;
        count=(check) ? (count+1) : count;
        newR[temp_index]=(check) ? count : 0;
    }

    for(int i=0;i<size;i++){
        int index=nimg[i];//index of RGN
        nimg[i] = newR[index];
    }
    num_regions=count;

     //#################### Clean Noise ##################################

    featurevectorlength = 7;//we are going to extract 7 features
    std::vector<double*> featurevector;
    double **store_feature = new double* [num_regions];//features
    for(int i=0;i<num_regions; i++){//init
        store_feature[i] = new double[featurevectorlength]();
    }

    for(int r=0; r<h; r++){// 0 1 2 3
        for (int c=0; c<w; c++){
            if(nimg[r*w+c] != 0){
                QRgb pixel=image.pixel(c,r);
                int index=nimg[r*w+c]-1;
                store_feature[index][0]+=1;
                double temp_red=(double) qRed(pixel);
                store_feature[index][1]+=temp_red;
                double temp_green=(double) qGreen(pixel);
                store_feature[index][2]+=temp_green;
                double temp_blue=(double) qBlue(pixel);
                store_feature[index][3]+=temp_blue;
            }
        }
    }

    //implement GLC
    map<int, map<pair<int, int>, int> > each_regiongray_level_matrix;
    QImage gray_matrix = image.convertToFormat(QImage::Format_Grayscale8);
    map<int, double> matrix_count;
    for(int hei=0; hei<h-1; hei++){
        for(int wid=0; wid<w-1; wid++){
            if(nimg[hei*w+wid] == nimg[(hei+1)*w+wid+1] &&nimg[hei*w+wid] != 0){
                QRgb a = gray_matrix.pixel(wid,hei);
                QRgb b = gray_matrix.pixel(wid+1,hei+1);
                each_regiongray_level_matrix[nimg[hei*w+wid]][{(int) qRed(a), (int) qRed(b)}] ++;
                matrix_count[nimg[hei*w+wid]] ++;
             }
        }
    }
    // calculate the some basic featuers like energy, entropy, and contrast
    double energy = 0.0;
    double entropy = 0.0;
    double contrast = 0.0;
    // according to the formular to traval the map to caculate all the values
    for(auto val : each_regiongray_level_matrix){
        energy = 0.0;
        entropy = 0.0;
        contrast = 0.0;
        for(auto val1 : val.second){
            double temp_nu=val1.second/matrix_count[val.first];
            double temo_con=val1.first.first - val1.first.second;
            double temp_entropy = temp_nu*temp_nu;
            double temp_contrast = temp_nu * temo_con*temo_con;
            double temp_energy = temp_entropy;

            entropy = entropy + temp_entropy;
            contrast = contrast + temp_contrast;
            energy = energy + temp_energy;
        }
        store_feature[val.first-1][4] = energy;
        store_feature[val.first-1][5] = entropy;
        store_feature[val.first-1][6] = contrast;
   }

    double max=-99999.0;//normalize
    double min=99999.9;
    int i=0;
    while(i<featurevectorlength-4){//except 0,1,2,3
        int temp_index=i+4;
        for(int j=0; j<num_regions; j++){
        if(store_feature[j][temp_index]<min){//find min,max
            min=store_feature[j][temp_index];
        }
        if(store_feature[j][temp_index]>max){
            max=store_feature[j][temp_index];
        }

        //std::cout<<"max "<<max<<" = "<<"min "<<min<<std::endl;
    }

    for(int q=0; q<num_regions; q++){//
        double difference=max - min;
        store_feature[q][temp_index]-=min;
        store_feature[q][temp_index]/=difference;
    }
    max=-99999.0;
    min=99999.0;
    i++;
    }

    for(int i=0; i<num_regions; i++){//push back
        store_feature[i][1] /= store_feature[i][0]*255.0;
        store_feature[i][2] /= store_feature[i][0]*255.0;
        store_feature[i][3] /= store_feature[i][0]*255.0;

        store_feature[i][0] /= (double) size;
        featurevector.push_back(store_feature[i]);
    }

    //free memory
    delete[] RS;
    delete[] newR;

    return featurevector;
}


//**********************************************************************************
//Method: distance1
//Purpose: caculate the distance(similarity) between different region in different image
//parameter type : double, int
//return: double value
//**********************************************************************************


double distance1(double* v1, double* v2, int length){
    double sum=0.0;
    for(int i=0;i<length;i++){
        sum+=abs(v1[i]-v2[i]);
    }
    return sum;
}


//*********************************************************************************************************
//Method: distance2
//Purpose: caculate the distance(similarity on the other aspect) between different region in different image
//parameter type : double, int
//return: double value
//**********************************************************************************************************


double distance2(double* v1, double* v2, int length){
    double sum=0.0;
    for(int i=0;i<length;i++){
        sum+=abs(v1[i]-v2[i]);
    }
    return sum;
}


//*********************************************************************************************************************
//Method: calculateDistance
//Purpose: caculate the total distance(similarity) between the base image in database and the image in our query system
//parameter type : double, int
//return: None finish the caculate in the function
//*********************************************************************************************************************


void MainWindow::CalculateDistances(bool check){
    memset( distances, 0, num_images*sizeof(double) );
    //go through all images
    for(int n=0; n<num_images; n++){
        //
        distances[n] = 0.0;
        //go through all regions
        for (int i = 0; i < qfeature.size(); i++){
            double current=(double) RAND_MAX;
            double distance;
            for (int j = 0; j < d_features[n].size(); j++){
                distance=check ? distance1(qfeature[i], d_features[n][j], featurevectorlength) : distance2(qfeature[i], d_features[n][j], featurevectorlength);
                //find the distance between two images
                current = (current<distance) ? current : distance;
            }
            distances[n] += current;
        }
        distances[n] /= (double) qfeature.size();
    }
}
