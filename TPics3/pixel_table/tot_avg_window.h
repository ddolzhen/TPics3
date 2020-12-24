#ifndef TOT_AVG_WINDOW_H
#define TOT_AVG_WINDOW_H

#include <QWidget>
#include "mainwindow.h"
namespace Ui {
class tot_avg_window;
}

class tot_avg_window : public QWidget
{
    Q_OBJECT

public:
    explicit tot_avg_window(QWidget *parent = nullptr);
    void init(pixel_table* data_table);
    ~tot_avg_window();

private slots:
    void on_pushButton_clicked();

private:
    Ui::tot_avg_window *ui;
    pixel_table* pixel_data;
};

#endif // TOT_AVG_WINDOW_H
