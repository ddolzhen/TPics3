#include "animationwindow.h"
#include "ui_animationwindow.h"

AnimationWindow::AnimationWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnimationWindow)
{
    ui->setupUi(this);
}

AnimationWindow::~AnimationWindow()
{
    delete ui;
}

void AnimationWindow::init(pixel_table *table_in)
{
    table=table_in;
}

void AnimationWindow::on_pushButton_clicked()
{
    ulong time_min=ulong(ui->lineEdit->text().toDouble()*4096/25);
    ulong time_max=ulong(ui->lineEdit_2->text().toDouble()*4096/25);
    ulong time_inc=ulong(ui->lineEdit_3->text().toDouble()*4096/25);


    ulong time_curr=time_min;

    while (time_curr<=time_max)
    {
        animation.addFrame( table->filter( QString("toa(%1,%2)").arg(time_curr).arg(time_curr+time_inc))->matrixOut() );
        time_curr+=time_inc;
    }

}

void AnimationWindow::on_pushButton_2_clicked()
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &AnimationWindow::updateAnimation);
    timer->start(500);
    framenum=0;
}


void AnimationWindow::updateAnimation()
{
    uint* pixs=animation.frameOut(framenum++);
    printPixels(pixs,256,256);

}


void AnimationWindow::printPixels(uint* pixels,int x, int y){

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

//            if (!log_sc){


//                ui->labelMax->setText(QString::number(max,'e',2));
//                ui->labelMax_2->setText(QString::number(max*3/4,'e',2));
//                ui->labelMax_3->setText(QString::number(max/2,'e',2));
//                ui->labelMax_4->setText(QString::number(max/4,'e',2));
//            }else{

//                ui->labelMax->setText(QString::number((max)));
//                ui->labelMax_2->setText(QString::number(qExp(qLn(max)*3/4)));
//                ui->labelMax_3->setText(QString::number(qExp(qLn(max)*2/4)));
//                ui->labelMax_4->setText(QString::number(qExp(qLn(max)*1/4)));
//            }






            //ui->label->setPixmap(QString("scale.jpeg"));
            //ui->label->show();

            QPixmap pixmap=QPixmap::fromImage(*image);
            pixmap=pixmap.scaled(512,512,Qt::IgnoreAspectRatio);
            scene->clear();
            scene->addPixmap(pixmap);

            ui->graphicsView->setScene(scene);
            ui->graphicsView->show();
        }


}
