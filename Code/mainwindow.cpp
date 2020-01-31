#include "mainwindow.h"
#include "math.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <QFileDialog>
#include <string.h>
#include <iostream>
#ifdef _MSC_VER
    #include "headers/dirent.h"
#else
    #include <dirent.h>
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->select, SIGNAL(clicked()), this, SLOT(OpenImage()));
    connect(ui->load, SIGNAL(clicked()), this, SLOT(Load()));
    connect(ui->start, SIGNAL(clicked()), this, SLOT(Query()));

    //displayImage = QImage(ui->ImgDisplay->width(), ui->ImgDisplay->height(), QImage::Format_RGB32);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::Load()
{
    std::cout<<" !!!!!!!! "<<std::endl;

    ui->textBrowser_0->setText(QString::fromStdString("Loading..."));
    std::string dirname = QDir::currentPath().toStdString().substr(0,QDir::currentPath().toStdString().find("/Code")) + "/Database";
    DIR *dir = opendir(dirname.c_str());
    struct dirent *d;
    //ui->textBrowser_0->setText(QString::fromStdString("Loading..."));
    if (dir != NULL)
    {
        num_images = 0;
        while ((d = readdir(dir)) != NULL)
        {
            if (d->d_name[0] != '.')
            {
                names.push_back(dirname + "/" + d->d_name); num_images++;
            }
        }
        QImage image;
        for (int i=0; i<40; i++)
        {
            image.load(QString::fromStdString(names[i]));
            d_features.push_back(ExtractFeatureVector(image));
        }
        ui->textBrowser_0->setText(QString::fromStdString("Finish!!!"));
        std::cout<<num_images<<std::endl;
    }
}



void MainWindow::OpenImage()
{
    const QString title;
    QString fileName = QFileDialog::getOpenFileName(this, title);

    if (!fileName.isEmpty())
        inImage.load(fileName);

    //ui->progressBox->setText(QString::fromStdString("Processing query image.."));
    qfeature.clear();
    qfeature = ExtractFeatureVector(inImage.copy());

    //std::string filename = fileName.toStdString();
   // std::size_t found1 = fileName.toStdString().find_last_of("/");
    //std::string new_filename = fileName.toStdString().substr(fileName.toStdString().find_last_of("/")+1);
    //std::size_t found2 = fileName.toStdString().substr(fileName.toStdString().find_last_of("/")+1).find_last_of(".");
    std::string thumbnail_filename = QDir::currentPath().toStdString().substr(0,QDir::currentPath().toStdString().find("/Code"))+"/Thumbnails/"+fileName.toStdString().substr(fileName.toStdString().find_last_of("/")+1).substr(0, fileName.toStdString().substr(fileName.toStdString().find_last_of("/")+1).find_last_of("."))+"t.jpg";
    outImage.load(QString::fromStdString(thumbnail_filename));

    xScroll = (double) outImage.width()/2.0;
    yScroll = (double) outImage.height()/2.0;
    ui->textBrowser->setText(QString::fromStdString(fileName.toStdString().substr(fileName.toStdString().find_last_of("/")+1).substr(0, fileName.toStdString().substr(fileName.toStdString().find_last_of("/")+1).find_last_of("."))));
}


int compare(const void * a, const void * b){
    if (*(double*)a > *(double*)b) return 1;
    else if (*(double*)a < *(double*)b) return -1;
    else return 0;
}

void MainWindow::SortDistances()
{
    double* distances_copy = new double[num_images];
    int* visited = new int[num_images]();
    for(int i=0; i<num_images; i++)
        distances_copy[i] = distances[i];

    qsort(distances, num_images, sizeof(double), compare);

    names_copy.clear();
    for(int i=0; i<num_images; i++)
        for(int j=0; j<num_images; j++)
            if(distances[i] == distances_copy[j] && visited[j] == 0)
            {
                names_copy.push_back(names[j]); visited[j] = 1;
            }
}



void MainWindow::ViewDatabase()
{
    std::vector<std::string> new_names(num_images);
    std::vector<std::string> names_t(num_images);
    for (int i=0; i<num_images; i++)
    {
        std::string temp_name = names_copy[i].substr(names_copy[i].find_last_of("/")+1);
        new_names[i] = temp_name.substr(0, temp_name.find_last_of("."));
        names_t[i] = QDir::currentPath().toStdString().substr(0,QDir::currentPath().toStdString().find("/Code"))+"/Thumbnails/"+new_names[i]+"t.jpg";
    }
    outImage.load(QString::fromStdString(names_t[0]));
    xScroll = (double) outImage.width()/2.0; yScroll = (double) outImage.height()/2.0;

    std::vector<QString> distance(num_images);
    for (int i=0; i<num_images; i++)
    {
        char val[25] = ""; sprintf(val, "%.5f", distances[i]);
        std::string value(val);
        distance[i] = QString::fromStdString(new_names[i]+"\nd = "+value);
    }
    ui->textBrowser_1->setText(distance[0]);
    ui->textBrowser_2->setText(distance[1]);
    ui->textBrowser_3->setText(distance[2]);
    ui->textBrowser_4->setText(distance[3]);
    ui->textBrowser_5->setText(distance[4]);
    ui->textBrowser_6->setText(distance[5]);
    ui->textBrowser_7->setText(distance[6]);
    ui->textBrowser_8->setText(distance[7]);
    ui->textBrowser_9->setText(distance[8]);
    ui->textBrowser_10->setText(distance[9]);
    ui->textBrowser_11->setText(distance[10]);
    ui->textBrowser_12->setText(distance[11]);
    ui->textBrowser_13->setText(distance[12]);
    ui->textBrowser_14->setText(distance[13]);
    ui->textBrowser_15->setText(distance[14]);
    ui->textBrowser_16->setText(distance[15]);
    ui->textBrowser_17->setText(distance[16]);
    ui->textBrowser_18->setText(distance[17]);
    ui->textBrowser_19->setText(distance[18]);
    ui->textBrowser_20->setText(distance[19]);
    ui->textBrowser_21->setText(distance[20]);
    ui->textBrowser_22->setText(distance[21]);
    ui->textBrowser_23->setText(distance[22]);
    ui->textBrowser_24->setText(distance[23]);
    ui->textBrowser_25->setText(distance[24]);
    ui->textBrowser_26->setText(distance[25]);
    ui->textBrowser_27->setText(distance[26]);
    ui->textBrowser_28->setText(distance[27]);
    ui->textBrowser_29->setText(distance[28]);
    ui->textBrowser_30->setText(distance[29]);
    ui->textBrowser_31->setText(distance[30]);
    ui->textBrowser_32->setText(distance[31]);
    ui->textBrowser_33->setText(distance[32]);
    ui->textBrowser_34->setText(distance[33]);
    ui->textBrowser_35->setText(distance[34]);
    ui->textBrowser_36->setText(distance[35]);
    ui->textBrowser_37->setText(distance[36]);
    ui->textBrowser_38->setText(distance[37]);
    ui->textBrowser_39->setText(distance[38]);
    ui->textBrowser_40->setText(distance[39]);
}

void MainWindow::Query()
{
    distances = new double[num_images];
    CalculateDistances(false);
    SortDistances();
    ViewDatabase();
}

