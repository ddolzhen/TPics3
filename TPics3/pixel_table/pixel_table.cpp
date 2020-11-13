#include "pixel_table.h"
#include <QDir>
#include <QStringList>
pixel_table::pixel_table(QFile& pixel_file)
{
    if (pixel_file.open(QFile::ReadOnly))
    {
        QTextStream in_stream(&pixel_file);


        //Count lines in file
        line_ct=0;
        bool trigData=false;
        bool headerFound=false;
        while (!in_stream.atEnd()){

            QString line;
            line=in_stream.readLine();
            if (line[0]=='#')
            {
                if (line[1]=='C')
                {
                    trigData=false;
                    headerFound=true;
                }else if (line[1]=='T')
                {
                    trigData=true;
                    headerFound=true;
                }
            }else{
                line_ct++;
            }
        }
        if (!headerFound)
        {
            qDebug()<<"ERROR, NO HEADER LINE FOUND";
        }
        in_stream.seek(0);

        qDebug()<< "\tFirst pass done, allocating memory..";
        //Allocate storage

        trigID=     new ushort      [line_ct];
        trigTime=   new ulong       [line_ct];
        col=        new uchar       [line_ct];
        row=        new uchar       [line_ct];
        tot=        new uint32_t    [line_ct];
        tot_total=  new ulong       [line_ct];
        tof=        new ulong       [line_ct];
        toa=        new ulong       [line_ct];


        //Parse data
        line_ct=0;
        toa_max=0;
        tot_max=0;
        qDebug() << "\tMemory allocated, filling data";
        if (headerFound){
            if (!trigData){
                while (!in_stream.atEnd()){

                        QString line;
                        line=in_stream.readLine();
                        QStringList columns = line.split(QLatin1Char(','));
                        if ((columns[0])[0]=='#')
                        {

                        }else{


                                col         [line_ct]=columns[0].toUShort();
                                row         [line_ct]=columns[1].toUShort();
                                toa         [line_ct]=columns[2].toULong();
                                tot         [line_ct]=columns[3].toUInt();
                                tot_total   [line_ct]=columns[4].toULong();

                                //Setting toa_min
                                if (line_ct==1)
                                {
                                    toa_min= toa[line_ct];
                                }else{
                                    if (toa[line_ct]<toa_min){
                                        toa_min=toa[line_ct];
                                    }
                                }

                                //Setting toa_max
                                if (toa[line_ct]>toa_max){
                                    toa_max=toa[line_ct];
                                }

                                if (tot[line_ct]>tot_max){
                                    tot_max=tot[line_ct];
                                }


                                line_ct++;
                        }
                    }
            }else{

                while (!in_stream.atEnd()){

                        QString line;
                        line=in_stream.readLine();
                        QStringList columns = line.split(QLatin1Char(','));
                        if ((columns[0])[0]=='#')
                        {

                        }else{

                                trigID      [line_ct]=columns[0].toUShort();
                                trigTime    [line_ct]=columns[1].toUShort();
                                col         [line_ct]=columns[2].toUShort();
                                row         [line_ct]=columns[3].toUShort();
                                toa         [line_ct]=columns[4].toULong();
                                tot         [line_ct]=columns[5].toUInt();
                                tot_total   [line_ct]=columns[6].toULong();


                                //Setting toa_min
                                if (line_ct==1)
                                {
                                    toa_min= toa[line_ct];
                                }else{
                                    if (toa[line_ct]<toa_min){
                                        toa_min=toa[line_ct];
                                    }
                                }

                                //Setting toa_max
                                if (toa[line_ct]>toa_max){
                                    toa_max=toa[line_ct];
                                }

                                if (tot[line_ct]>tot_max){
                                    tot_max=tot[line_ct];
                                }





                            line_ct++;
                        }
                    }
            }
        }


    }else{
        qDebug() <<"File not found";

    }
    qDebug()<< "pixel_table constructed. Length="<<line_ct;

}

//Constructor to use after filtering
pixel_table::pixel_table(    ushort* trigID_p,  ulong* trigTime_p,  uchar* col_p, uchar* row_p,  ulong* toa_p, uint32_t* tot_p, ulong* tot_total_p, ulong* tof_p, uint len,ulong toa_min_t,ulong toa_max_t,ulong tot_max_t){
    trigID=     trigID_p;
    trigTime=   trigTime_p;
    col=        col_p;
    row=        row_p;
    tot=        tot_p;
    tot_total=  tot_total_p;
    tof=        tof_p;
    toa=        toa_p;
    line_ct=len;
    toa_min=toa_min_t;
    toa_max=toa_max_t;
    tot_max=tot_max_t;




}



pixel_table::~pixel_table(){
    delete trigID;
    delete trigTime;
    delete col;
    delete row;
    delete tot;
    delete tot_total;
    delete tof;
}

//  TODO: allow multiple intervals per variable.
// format: String containing the ranges of variables.

// Example:

// "x(13,46);y(67,64);toa(157873212160,257873212160)"
//    ^   ^
//    |   |
//    |   |
//    |   |
//  x from 13 to 26 (including 13 and 26)

// variables are :
// trigID,trigTime,x,y,tot,tot_total,tof

pixel_table* pixel_table::filter(QString format){
    QStringList commands=format.split(QLatin1Char(';'));
    QStringList temp;
    QStringList bounds;

    uint x_min=0;
    uint x_max=0;
    uint y_min=0;
    uint y_max=0;
    uint tot_min_f=0;
    uint tot_max_f=0;
    uint tot_total_min=0;
    uint tot_total_max=0;
    ulong tof_min=0;
    ulong tof_max=0;
    ulong trigID_min=0;
    ulong trigID_max=0;
    ulong trigTime_min=0;
    ulong trigTime_max=0;
    ulong toa_min_f=0;
    ulong toa_max_f=0;


    // get rules
    for (int lcv=0;lcv<commands.length();lcv++)
    {
        if (!commands[lcv].isEmpty())
        {
            if (commands[lcv].split(QLatin1Char('('))[0]=="x"){
                bounds=(commands[lcv].split(QLatin1Char('('))[1]).split(QLatin1Char(')'),Qt::SkipEmptyParts)[0].split(QLatin1Char(','));
                x_min=bounds[0].toUInt();
                x_max=bounds[1].toUInt();
            }

            if (commands[lcv].split(QLatin1Char('('))[0]=="y"){
                bounds=(commands[lcv].split(QLatin1Char('('))[1]).split(QLatin1Char(')'),Qt::SkipEmptyParts)[0].split(QLatin1Char(','));
                y_min=bounds[0].toUInt();
                y_max=bounds[1].toUInt();

            }

            if (commands[lcv].split(QLatin1Char('('))[0]=="tot"){
                bounds=(commands[lcv].split(QLatin1Char('('))[1]).split(QLatin1Char(')'),Qt::SkipEmptyParts)[0].split(QLatin1Char(','));
                tot_min_f=bounds[0].toUInt();
                tot_max_f=bounds[1].toUInt();

            }

            if (commands[lcv].split(QLatin1Char('('))[0]=="tot_total"){
                bounds=(commands[lcv].split(QLatin1Char('('))[1]).split(QLatin1Char(')'),Qt::SkipEmptyParts)[0].split(QLatin1Char(','));
                tot_total_min=bounds[0].toUInt();
                tot_total_max=bounds[1].toUInt();
            }

            if (commands[lcv].split(QLatin1Char('('))[0]=="trigID"){
                bounds=(commands[lcv].split(QLatin1Char('('))[1]).split(QLatin1Char(')'),Qt::SkipEmptyParts)[0].split(QLatin1Char(','));
                trigID_min=bounds[0].toULong();
                trigID_max=bounds[1].toULong();
            }
            if (commands[lcv].split(QLatin1Char('('))[0]=="toa"){
                bounds=(commands[lcv].split(QLatin1Char('('))[1]).split(QLatin1Char(')'),Qt::SkipEmptyParts)[0].split(QLatin1Char(','));
                toa_min_f=bounds[0].toULong();
                toa_max_f=bounds[1].toULong();
            }
        }
    }


    bool x_in_bounds;
    bool y_in_bounds;
    bool tot_in_bounds;
    bool tot_total_in_bounds;
    bool trigID_in_bounds;
    bool toa_in_bounds;
    //  Filter data
    //  TODO: make data into linked list so you don't have to make 2 runs
    //      1.Count length

    uint len=0;
    for (uint32_t i=0;i<line_ct;i++)
    {
        x_in_bounds=            (col[i]>=x_min                  && (col[i]<=x_max               ||      x_max==0        ));
        y_in_bounds=            (row[i]>=y_min                  && (row[i]<=y_max               ||      y_max==0        ));
        tot_in_bounds=          (tot[i]>=tot_min_f                && (tot[i]<=tot_max_f             ||      tot_max_f==0      ));
        tot_total_in_bounds=    (tot_total[i]>=tot_total_min    && (tot_total[i]<=tot_total_max ||      tot_total_max==0));
        trigID_in_bounds=       (trigID[i]>=trigID_min          && (trigID[i]<=trigID_max       ||      trigID_max==0   ));
        toa_in_bounds=          (toa[i]>=toa_min_f                && (toa[i]<=toa_max_f             ||      toa_max_f==0      ));

        if (x_in_bounds && y_in_bounds && tot_in_bounds && tot_total_in_bounds && trigID_in_bounds && toa_in_bounds)
        {
            len++;
        }
    }

    ushort* trigID_f=    new ushort      [len];
    ulong* trigTime_f=   new ulong       [len];
    uchar* col_f=        new uchar       [len];
    uchar* row_f=        new uchar       [len];
    uint32_t* tot_f=     new uint32_t    [len];
    ulong* tot_total_f=  new ulong       [len];
    ulong* tof_f=        new ulong       [len];
    ulong* toa_f=        new ulong       [len];


    ulong toa_min_t=0;
    ulong toa_max_t=0;
    ulong tot_max_t=0;

    //      2. Fill new data
    len=0;
    for (uint32_t i=0;i<line_ct;i++)
    {
        x_in_bounds=            (col[i]>=x_min                  && (col[i]<=x_max               ||      x_max==0        ));
        y_in_bounds=            (row[i]>=y_min                  && (row[i]<=y_max               ||      y_max==0        ));
        tot_in_bounds=          (tot[i]>=tot_min_f                && (tot[i]<=tot_max_f             ||      tot_max_f==0      ));
        tot_total_in_bounds=    (tot_total[i]>=tot_total_min    && (tot_total[i]<=tot_total_max ||      tot_total_max==0));
        trigID_in_bounds=       (trigID[i]>=trigID_min          && (trigID[i]<=trigID_max       ||      trigID_max==0   ));
        toa_in_bounds=          (toa[i]>=toa_min_f                && (toa[i]<=toa_max_f             ||      toa_max_f==0      ));

        if (x_in_bounds && y_in_bounds && tot_in_bounds && tot_total_in_bounds && trigID_in_bounds && toa_in_bounds)
        {
            trigID_f[len]=trigID[i];
            trigTime_f[len]=trigTime[i];
            col_f[len]=col[i];
            row_f[len]=row[i];
            tot_f[len]=tot[i];
            tot_total_f[len]=tot_total[i];
            tof_f[len]=tof[i];
            toa_f[len]=toa[i];


            //Setting toa_min
            if (len==0)
            {
                toa_min_t= toa_f[len];
            }else{
                if (toa_f[len]<toa_min_t){
                    toa_min_t=toa_f[len];
                }
            }


            if (toa_f[len]>toa_max_t)
            {
                toa_max_t=toa_f[len];
            }
            if (tot_f[len]>tot_max_t){
                tot_max_t=tot_f[len];
            }

            len++;




        }
    }


    return (new pixel_table(trigID_f,trigTime_f,col_f,row_f,toa_f,tot_f,tot_total_f,tof_f,len,toa_min_t,toa_max_t,tot_max_t));


}





uint* pixel_table::matrixOut()
{
    uint* pixels=new uint[256*256];
    for (int lcv=0;lcv<256*256;lcv++)
    {
        pixels[lcv]=0;
    }


    for (uint lcv=0;lcv<line_ct;lcv++)
    {
        pixels[ row[lcv]*256+col[lcv] ] += tot[lcv];
    }

    return pixels;
}

ulong* pixel_table::matrixOut_TOA()
{
    ulong* pixels=new ulong[256*256];
    for (int lcv=0;lcv<256*256;lcv++)
    {
        pixels[lcv]=0;
    }


    for (uint lcv=0;lcv<line_ct;lcv++)
    {

        if ( pixels[ row[lcv]*256+col[lcv] ] ==0 || pixels[ row[lcv]*256+col[lcv] ] > toa[lcv])
        {
            pixels[ row[lcv]*256+col[lcv] ] = toa[lcv];
        }

    }

    return pixels;
}


// BEWARE OF VERY UGLY CODE
void pixel_table::toa_hist(int nbins,QCustomPlot* customPlot){

    ulong* toa_array=new ulong[nbins];
    ulong* freq_array=new ulong[nbins];
    ulong width=(toa_max-toa_min)/nbins;
    ulong margins[nbins];

    margins[0]=toa_min+width;



    for (int i=1;i<nbins;i++)
    {
        margins[i]=margins[i-1]+width;
    }


    for (int i=0;i<nbins;i++)
    {
        toa_array[i]=margins[i]-(width/2);
        freq_array[i]=0;
    }
    ulong freq_max=0;
    for (int i=0;i<line_ct;i++)
    {
        ulong toa_curr=toa[i];
        for (int j=0;j<nbins;j++)
        {
            if (toa_curr<margins[j])
            {
                freq_array[j]+=1;
                if (freq_array[j]>freq_max)
                {
                    freq_max=freq_array[j];
                }
                break;

            }
        }
    }

    QVector<double> toa_vec(nbins+1),freq_vec(nbins+1);
    for (int i=0; i<nbins+1; ++i)
    {
      toa_vec[i] = double(toa_array[i])/4096.0*25.0;
      freq_vec[i] = double(freq_array[i]);
    }
    toa_vec.insert(0,(toa_min-width/2)/4096*25);
    freq_vec.insert(0,0);
    toa_vec.insert(0,(toa_max+width/2)/4096*25);
    freq_vec.insert(0,0);
    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(0)->setData(toa_vec, freq_vec);
    customPlot->graph(0)->setLineStyle(QCPGraph::lsStepCenter);
    customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone, Qt::red, Qt::white, 7));

    // give the axes some labels:
    //ui->customPlot->xAxis->setLabel("x");
    //ui->customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange((toa_min- (toa_max-toa_min)*0.2)/4096*25, (toa_max+ (toa_max-toa_min)*0.2)/4096*25);
    qDebug()<< "x axis from "<< (toa_min- (toa_max-toa_min)*0.2)/4096*25 << " to " <<(toa_max+ (toa_max-toa_min)*0.2)/4096*25;
    customPlot->yAxis->setRange(0, freq_max*1.3);
    customPlot->setVisible(true);
    customPlot->replot();

}


void pixel_table::tot_hist(int nbins,QCustomPlot* customPlot){

    ulong* tot_array=new ulong[nbins];
    ulong* freq_array=new ulong[nbins];
    ulong width=(tot_max)/nbins;
    ulong margins[nbins];

    margins[0]=width;



    for (int i=1;i<nbins;i++)
    {
        margins[i]=margins[i-1]+width;
    }


    for (int i=0;i<nbins;i++)
    {
        tot_array[i]=margins[i]-(width/2);
        freq_array[i]=0;
    }
    ulong freq_max=0;
    for (int i=0;i<line_ct;i++)
    {
        ulong tot_curr=tot[i];
        for (int j=0;j<nbins;j++)
        {
            if (tot_curr<margins[j])
            {
                freq_array[j]++;
                if (freq_array[j]>freq_max)
                {
                    freq_max=freq_array[j];
                }
                break;

            }
        }
    }

    QVector<double> tot_vec(nbins+1),freq_vec(nbins+1);

    for (int i=0; i<nbins+1; ++i)
    {
      tot_vec[i] = double(tot_array[i]);
      freq_vec[i] = double(freq_array[i]);
    }
//    tot_vec.insert(0,(toa_min-width/2));
//    freq_vec.insert(0,0);
    tot_vec.insert(0,(tot_max+width/2));
    freq_vec.insert(0,0);
    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(0)->setData(tot_vec, freq_vec);
    customPlot->graph(0)->setLineStyle(QCPGraph::lsStepCenter);
    customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone, Qt::red, Qt::white, 7));

    // give the axes some labels:
    //ui->customPlot->xAxis->setLabel("x");
    //ui->customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(0, (tot_max*1.2));
    customPlot->yAxis->setRange(0, freq_max*1.3);
    customPlot->setVisible(true);
    customPlot->replot();

}

