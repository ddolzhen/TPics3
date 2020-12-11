#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include <QMainWindow>
#include<QtMath>
#include "mainwindow.h"


namespace Ui {
class DisplayWindow;
}

class DisplayWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DisplayWindow(QWidget *parent = nullptr);
    ~DisplayWindow();
    void printPixels(uint* pixels,int x, int y );
    void printPixels_TOA(ulong* pixels,int x, int y );
    void init(pixel_table* data_table);

private slots:
    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::DisplayWindow *ui;
    QPixmap pixmap;
    uchar* pixel_data;
    pixel_table* filtered_data;
    QImage *image;
    QImage *image_toa;
    ulong new_max;
};

#endif // DISPLAYWINDOW_H
