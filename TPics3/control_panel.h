#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H

#include <QDialog>
#include <QDebug>
#include <QString>
namespace Ui {
class Control_panel;
}

class Control_panel : public QDialog
{
    Q_OBJECT

public:
    explicit Control_panel(QWidget *parent = nullptr);
    ~Control_panel();
    QString countRules();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();


private:

    bool tot_set=false;
    bool col_set=false;
    bool row_set=false;
    bool toa_set=false;
    Ui::Control_panel *ui;
};

#endif // CONTROL_PANEL_H
