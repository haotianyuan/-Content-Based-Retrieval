#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

template <class T> const T& max ( const T& a, const T& b ) {
  return (b<a)?a:b;     // or: return comp(b,a)?a:b; for the comp version
}

template <class T> const T& min ( const T& a, const T& b ) {
  return (b>a)?a:b;     // or: return comp(b,a)?a:b; for the comp version
}

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QImage inImage;
    QImage outImage;
    QImage displayImage;
    std::vector<double*> qfeature;
    std::vector<std::vector<double*>> d_features;
    double *distances;
    int num_images;
    int featurevectorlength;
    double** Image;
    int imageWidth;
    int imageHeight;
    std::vector<std::string> names;
    std::vector<std::string> names_copy;
    double xScroll;
    double yScroll;

    void CalculateDistances(bool check);
    void SortDistances();
    void ViewDatabase();
    std::vector<double*> ExtractFeatureVector(QImage image);

private slots:
    void Load();
    void OpenImage();
    void Query();

    void ConvertQImage2Double(QImage image);
    void ConvertDouble2QImage(QImage *image);
};

#endif // MAINWINDOW_H
