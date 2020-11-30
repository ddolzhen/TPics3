#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QMouseEvent>
#include<QtMath>
#include "QImage"
#include "QPixmap"
#include "QDebug"
#include "QMimeData"
#include "pixel_table/pixel_table.h"
#include "control_panel.h"
#include "qcustomplot.h"
#include "displaywindow.h"
#include "animation.h"
#include <QTimer>
#include "animationwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


protected:
    void dropEvent(QDropEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;

private slots:

    void on_horizontalSlider_sliderMoved(int position);

    void on_filter_pb_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

private:
    Ui::MainWindow *ui;
    void printPixels(uint* pixels,int x, int y );
    void mousePressEvent(QMouseEvent* event);
    pixel_table* main_data;
    pixel_table* filtered_data;
    QImage *image;
    uchar* pixel_data;
    QPixmap pixmap;
    Control_panel *panel;
    void updateAnimation();
    int frame_counter;
    Animation animation;


};
#endif // MAINWINDOW_H
