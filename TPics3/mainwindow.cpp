#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlTableModel>


// TODO:    ANIMATED PIXELS!
//          Cluster/Pixel association
//          Export as .csv/.pdf/jupiter
//          Filtering through sliders

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    setAcceptDrops(true);


    main_data=new pixel_table(NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,0,0,0);
    filtered_data=new pixel_table(NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,0,0,0);
    image= new QImage(512,512,QImage::Format_RGB32);
    QGraphicsScene* scene=new QGraphicsScene;

    QPixmap pixmap=QPixmap::fromImage(*image);

    scene->addPixmap(pixmap);





    ui->setupUi(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->show();



    panel=new Control_panel;
    panel->show();
    this->hide();

    this->setFixedSize(this->size());
    ui->graphicsView->setAcceptDrops(true);

    ui->customPlot->setVisible(false);
    ui->label_3->setVisible(false);

    ui->customPlot_2->setVisible(false);
    ui->label_5->setVisible(false);


}


void MainWindow::printPixels(uint* pixels,int x, int y)
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
        scene->clear();
        scene->addPixmap(pixmap);

        ui->graphicsView->setScene(scene);
        ui->graphicsView->show();
    }

}


// TODO: Handle presses on the screen
void MainWindow::mousePressEvent(QMouseEvent* event)
{
    qDebug() << "Stop clicking me!!!";


    // Max width ~311

}

// Handles loading files
void MainWindow::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasUrls()){

        QFile file(event->mimeData()->urls()[0].path());
        delete main_data;


        qDebug() << "Creating a table";
        main_data=new pixel_table(file);
        qDebug() << "Table created, creating matrix";
        uint* pixs=main_data->matrixOut();



        printPixels(pixs,256,256);
        main_data->toa_hist(100,ui->customPlot);
        main_data->tot_hist(100,ui->customPlot_2);
        ui->label_3->setVisible(true);
        ui->label_5->setVisible(true);


//        for (int i=0;i<20;i++)
//        {
//            animation.addFrame(main_data->filter(QString("toa(%1,%2)").arg(ulong((1e9+ i*(1e5))*4096/25)).arg(ulong((1e9+(i+1)*1e5)*4096/25)))->matrixOut());
//            //qDebug() << QString("toa(%1,%2)").arg(ulong((1e9+ i*(1e5))*4096/25)).arg(ulong((1e9+(i+1)*1e5)*4096/25));

//        }
//        qDebug()<< "Contructed animation";

//        QTimer *timer = new QTimer(this);
//        connect(timer, &QTimer::timeout, this, &MainWindow::updateAnimation);
//        timer->start(500);
//        frame_counter=0;

//        AnimationWindow* an_window=new AnimationWindow;
//        an_window->init(main_data);
//        an_window->show();

    }
}







void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    QGraphicsScene* scene=new QGraphicsScene;
    QPixmap pixmap=QPixmap::fromImage(*image);
    pixmap=pixmap.scaled(512+10*position,512+10*position);
    scene->addPixmap(pixmap);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->show();

}

void MainWindow::on_filter_pb_clicked()
{
    delete filtered_data;
    QString rules=panel->countRules();
    filtered_data=main_data->filter(rules);
    DisplayWindow* filtered_window=new DisplayWindow;
    filtered_window->init(filtered_data);
    filtered_window->show();
    //this->hide();
}

void MainWindow::on_radioButton_clicked()
{
    uint* pixs=main_data->matrixOut();



    printPixels(pixs,256,256);
}

void MainWindow::on_radioButton_2_clicked()
{
    uint* pixs=main_data->matrixOut();



    printPixels(pixs,256,256);
}

void MainWindow::updateAnimation()
{
    uint* pixs=animation.frameOut(frame_counter++);
    if (frame_counter==20)
    {
        frame_counter=0;
    }
    printPixels(pixs,256,256);
    qDebug() << "UPD";
}


