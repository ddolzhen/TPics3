#include "control_panel.h"
#include "ui_control_panel.h"

Control_panel::Control_panel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Control_panel)
{

    ui->setupUi(this);

    ui->comboBox->addItem(QString("TOT"));
    ui->comboBox->addItem(QString("col"));
    ui->comboBox->addItem(QString("row"));
    ui->comboBox->addItem(QString("TOA"));
    ui->comboBox->addItem(QString("TOA_ns"));


}


Control_panel::~Control_panel()
{
    delete ui;
}

void Control_panel::on_pushButton_clicked()
{
    if(ui->comboBox->currentText()=="TOT"){
        if (tot_set){
            qDebug()<<"TOT rule already set !!";
        }else{
            ui->listWidget->addItem(QString("tot(")+ui->lineEdit->text()+QString(",")+ui->lineEdit_2->text()+QString(")"));
            tot_set=true;
        }
    }else if (ui->comboBox->currentText()=="col"){
        if (col_set){
            qDebug()<<"col rule already set !!";
        }else{
            if (ui->lineEdit->text().toInt()>=0 && ui->lineEdit->text().toInt()<=255 && ui->lineEdit_2->text().toInt()>=0 && ui->lineEdit_2->text().toInt()<=255 && ui->lineEdit_2->text().toInt() > ui->lineEdit->text().toInt())
            {
                ui->listWidget->addItem(QString("x(")+ui->lineEdit->text()+QString(",")+ui->lineEdit_2->text()+QString(")"));
                col_set=true;
            }else{
                qDebug() << "inapropriate col arguments (int from 0 to 255, max > min)";
            }


        }
    }else if (ui->comboBox->currentText()=="row"){
        if (row_set){
            qDebug()<<"row rule already set !!";
        }else{
            if (ui->lineEdit->text().toInt()>=0 && ui->lineEdit->text().toInt()<=255 && ui->lineEdit_2->text().toInt()>=0 && ui->lineEdit_2->text().toInt()<=255 && ui->lineEdit_2->text().toInt() > ui->lineEdit->text().toInt())
            {
                ui->listWidget->addItem(QString("y(")+ui->lineEdit->text()+QString(",")+ui->lineEdit_2->text()+QString(")"));
                row_set=true;
            }else{
                qDebug() << "inapropriate row arguments (int from 0 to 255, max > min)";
            }

        }
    }else if (ui->comboBox->currentText()=="TOA"){
        if (toa_set){
            qDebug()<<"TOA rule already set !!";
        }else{
            ui->listWidget->addItem(QString("toa(")+ui->lineEdit->text()+QString(",")+ui->lineEdit_2->text()+QString(")"));
            toa_set=true;

        }
    }else if (ui->comboBox->currentText()=="TOA_ns"){
    if (toa_set){
        qDebug()<<"TOA rule already set !!";
    }else{
        ui->listWidget->addItem(QString("toa(")+QString::number(ulong(ui->lineEdit->text().toDouble()*4096.0/25.0))+QString(",")+QString::number(ulong(ui->lineEdit_2->text().toDouble()*4096.0/25.0))+QString(")"));
        toa_set=true;
    }
}
}

void Control_panel::on_pushButton_2_clicked()
{
    if( ui->listWidget->currentItem()->text() .contains("tot"))
    {
        tot_set=false;
    }else if (  ui->listWidget->currentItem()->text() .contains("x")  )
    {
        col_set=false;
    }else if (  ui->listWidget->currentItem()->text() .contains("y")  )
    {
        row_set=false;
    }else if (  ui->listWidget->currentItem()->text() .contains("toa")  )
    {
        toa_set=false;
    }
    ui->listWidget->takeItem(ui->listWidget->currentRow());
}




QString Control_panel::countRules()
{
    QString ruleset("");
    //Assemble the ruleset.
    for (int i=0;i<ui->listWidget->count();i++){
        ruleset.append(ui->listWidget->item(i)->text());
        ruleset.append(QString(";"));
    }
//ruleset.remove(ruleset.back());
    qDebug()<<ruleset;
    return ruleset;
}
