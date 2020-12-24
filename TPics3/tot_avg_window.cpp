#include "tot_avg_window.h"
#include "ui_tot_avg_window.h"

tot_avg_window::tot_avg_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tot_avg_window)
{
    ui->setupUi(this);
}

tot_avg_window::~tot_avg_window()
{
    delete ui;
}


void tot_avg_window::init(pixel_table* data_table)
{
    pixel_data=data_table;




}

void tot_avg_window::on_pushButton_clicked()
{
    uint* pixs=pixel_data->matrixOut();
    uchar a_x=ui->lineEdit->text().toInt();
    uchar a_y=ui->lineEdit_2->text().toInt();
    uchar b_x=ui->lineEdit_3->text().toInt();
    uchar b_y=ui->lineEdit_4->text().toInt();
    uint sum=0;
    for (uchar i=a_x;i<=b_x;i++)
    {
        for (uchar j=a_y;j<=b_y;j++)
        {
            sum+=pixs[j*256+i];
        }
    }

    ui->label_5->setText(QString(sum));

}
