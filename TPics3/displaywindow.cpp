#include "displaywindow.h"
#include "ui_displaywindow.h"

DisplayWindow::DisplayWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DisplayWindow)
{
    ui->setupUi(this);

    this->setFixedSize(this->size());
}

DisplayWindow::~DisplayWindow()
{
    delete ui;
    ui->customPlot->setVisible(false);
    ui->label_3->setVisible(false);

    ui->customPlot_2->setVisible(false);
    ui->label_5->setVisible(false);
}

void DisplayWindow::init(pixel_table* data_table)
{
    filtered_data=data_table;
    uint* pixs=filtered_data->matrixOut();
    image=new QImage();
    image_toa=new QImage();


    printPixels(pixs,256,256);
    filtered_data->toa_hist(100,ui->customPlot);
    filtered_data->tot_hist(100,ui->customPlot_2);
    ui->label_3->setVisible(true);
    ui->label_5->setVisible(true);
    ulong* pixs_toa=filtered_data->matrixOut_TOA();
    printPixels_TOA(pixs_toa,256,256);


}


void DisplayWindow::printPixels(uint* pixels,int x, int y)
{
    bool log_sc;
    if (ui->radioButton->isChecked())
    {
        log_sc=false;
    }else{
        log_sc=true;
    }
    QGraphicsScene* scene=new QGraphicsScene;
    //Find Max
    ulong max=0;
    delete image;
    uint max_pos;
    image= new QImage(256,256,QImage::Format_RGB32);
    for (uint i=0;i<(x*y)-1;i++){
        if (pixels[i]>max)
        {
            max=pixels[i];
            max_pos=i;
        }
    }
    if (max==0)
    {
        qDebug()<<"Oh no..";
    }else // Scale them down
    {
        //qDebug() << "max is" << max<< " at " << max_pos;


        if (log_sc)
        {
            for (uint i=0;i<(x*y)-1;i++){
                double intensity_dbl= qLn(pixels[i]);
                intensity_dbl=intensity_dbl*1023/qLn(max);

                uint intensity=uint(intensity_dbl);

                if (intensity_dbl<1.0 and intensity_dbl>1e-7)
                {
                    intensity=1;
                }


                //Find Color
                QColor pixel_color;
                if (intensity==0){
                    pixel_color=QColor(255,255,255);
                }else if (intensity<256){
                    pixel_color=QColor(0,intensity,255);
                }else if (intensity<512){
                    pixel_color=QColor(0,255,511-intensity);
                }else if (intensity<768){
                    pixel_color=QColor(intensity-512,255,0);
                }else{
                    pixel_color=QColor(255,1023-intensity,0);
                }


                image->setPixelColor(i/256,255-i%256,pixel_color);
            }

        }else{

            for (uint i=0;i<(x*y)-1;i++){
                double intensity_dbl= double(pixels[i])*1023.0/double(max);
                uint intensity=uint(intensity_dbl);

                if (intensity_dbl<1.0 and intensity_dbl>1e-7)
                {
                    intensity=1;
                }

                //Find Color
                QColor pixel_color;
                if (intensity==0){
                    pixel_color=QColor(255,255,255);
                }else if (intensity<256){
                    pixel_color=QColor(0,intensity,255);
                }else if (intensity<512){
                    pixel_color=QColor(0,255,511-intensity);
                }else if (intensity<768){
                    pixel_color=QColor(intensity-512,255,0);
                }else{
                    pixel_color=QColor(255,1023-intensity,0);
                }

                image->setPixelColor(i/256,255-i%256,pixel_color);
            }
        }

        if (!log_sc){


            ui->labelMax->setText(QString::number(max,'e',2));
            ui->labelMax_2->setText(QString::number(max*3/4,'e',2));
            ui->labelMax_3->setText(QString::number(max/2,'e',2));
            ui->labelMax_4->setText(QString::number(max/4,'e',2));
        }else{

            ui->labelMax->setText(QString::number((max)));
            ui->labelMax_2->setText(QString::number(qExp(qLn(max)*3/4)));
            ui->labelMax_3->setText(QString::number(qExp(qLn(max)*2/4)));
            ui->labelMax_4->setText(QString::number(qExp(qLn(max)*1/4)));
        }



        ui->label->setPixmap(QString("scale.jpeg"));
        ui->label->show();




        QPixmap pixmap=QPixmap::fromImage(*image);
        pixmap=pixmap.scaled(512,512,Qt::IgnoreAspectRatio);
        scene->addPixmap(pixmap);
        ui->graphicsView->setScene(scene);
        ui->graphicsView->show();
    }

}




void DisplayWindow::printPixels_TOA(ulong* pixels,int x, int y)
{
    bool log_sc;

    log_sc=false;

    QGraphicsScene* scene=new QGraphicsScene;
    //Find Max
    ulong max=0;
    ulong min=std::numeric_limits<ulong>::max();
    delete image_toa;
    uint max_pos;
    image_toa= new QImage(256,256,QImage::Format_RGB32);
    for (uint i=0;i<(x*y)-1;i++){
        if ((pixels[i]<min && pixels[i] != 0))
        {
            min=pixels[i];
        }
        if (pixels[i]>max)
        {
            max=pixels[i];
            max_pos=i;
        }
    }
    if (max==0)
    {
        qDebug()<<"printPixels_TOA : input matrix is empty";
    }else // Scale them down
    {
        //qDebug() << "max is" << max<< " at " << max_pos;


        if (log_sc)
        {
            for (uint i=0;i<(x*y)-1;i++){
                double intensity_dbl= qLn(pixels[i]);
                intensity_dbl=intensity_dbl*1023/qLn(max);

                uint intensity=uint(intensity_dbl);

                if (intensity_dbl<1.0 and intensity_dbl>1e-7)
                {
                    intensity=1;
                }


                //Find Color
                QColor pixel_color;
                if (intensity==0){
                    pixel_color=QColor(255,255,255);
                }else if (intensity<256){
                    pixel_color=QColor(0,intensity,255);
                }else if (intensity<512){
                    pixel_color=QColor(0,255,511-intensity);
                }else if (intensity<768){
                    pixel_color=QColor(intensity-512,255,0);
                }else{
                    pixel_color=QColor(255,1023-intensity,0);
                }


                image_toa->setPixelColor(i/256,255-i%256,pixel_color);
            }

        }else{

            for (uint i=0;i<(x*y)-1;i++){
                double intensity_dbl= double(pixels[i]-min)*1023.0/double(max-min);
                uint intensity=uint(intensity_dbl);

                if (pixels[i]==0){
                    intensity=0;
                }


                if (intensity_dbl<1.0 and intensity_dbl>1e-7)
                {
                    intensity=1;
                }

                //Find Color
                QColor pixel_color;
                if (intensity==0){
                    pixel_color=QColor(255,255,255);
                }else if (intensity<256){
                    pixel_color=QColor(0,intensity,255);
                }else if (intensity<512){
                    pixel_color=QColor(0,255,511-intensity);
                }else if (intensity<768){
                    pixel_color=QColor(intensity-512,255,0);
                }else{
                    pixel_color=QColor(255,1023-intensity,0);
                }

                image_toa->setPixelColor(i/256,255-i%256,pixel_color);
            }
        }

//        if (!log_sc){


//            ui->labelMax->setText(QString::number(max,'e',2));
//            ui->labelMax_2->setText(QString::number(max*3/4,'e',2));
//            ui->labelMax_3->setText(QString::number(max/2,'e',2));
//            ui->labelMax_4->setText(QString::number(max/4,'e',2));
//        }else{

//            ui->labelMax->setText(QString::number((max)));
//            ui->labelMax_2->setText(QString::number(qExp(qLn(max)*3/4)));
//            ui->labelMax_3->setText(QString::number(qExp(qLn(max)*2/4)));
//            ui->labelMax_4->setText(QString::number(qExp(qLn(max)*1/4)));
//        }







        QPixmap pixmap=QPixmap::fromImage(*image_toa);
        pixmap=pixmap.scaled(512,512,Qt::IgnoreAspectRatio);
        scene->addPixmap(pixmap);
        ui->graphicsView_2->setScene(scene);
        ui->graphicsView_2->show();
    }

}

void DisplayWindow::on_radioButton_clicked()
{
    uint* pixs=filtered_data->matrixOut();



    printPixels(pixs,256,256);
}

void DisplayWindow::on_radioButton_2_clicked()
{
    uint* pixs=filtered_data->matrixOut();



    printPixels(pixs,256,256);
}

void DisplayWindow::on_horizontalSlider_valueChanged(int value)
{
    QGraphicsScene* scene=new QGraphicsScene;
    QPixmap pixmap=QPixmap::fromImage(*image);
    pixmap=pixmap.scaled(512+10*value,512+10*value);
    scene->addPixmap(pixmap);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->show();
}

void DisplayWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void DisplayWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
