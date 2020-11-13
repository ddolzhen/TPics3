#ifndef PIXEL_TABLE_H
#define PIXEL_TABLE_H
#include <QtGlobal>
#include <QFile>
#include <QTextStream>
#include<QDebug>
#include <qcustomplot.h>
class pixel_table
{
public:
    pixel_table(QFile&);
    pixel_table(    ushort*,  ulong* ,  uchar* , uchar* ,  ulong* , uint32_t*, ulong* , ulong* , uint ,ulong,ulong,ulong);



    ushort* trigID;
    ulong* trigTime;
    uchar* col;
    uchar* row;
    ulong* toa;
    uint32_t* tot;
    ulong* tot_total;
    ulong* tof;
    uint line_ct;


    ulong toa_min;
    ulong toa_max;
    ulong tot_max;
    ~pixel_table();
    uint* matrixOut();
    ulong* matrixOut_TOA();
    pixel_table* filter(QString);
    void toa_hist(int nbins,QCustomPlot* customPlot);
    void tot_hist(int nbins,QCustomPlot* customPlot);



};

#endif // PIXEL_TABLE_H
