#ifndef ANIMATIONWINDOW_H
#define ANIMATIONWINDOW_H

#include <QDialog>
#include <mainwindow.h>
#include <QString>
namespace Ui {
class AnimationWindow;
}

class AnimationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AnimationWindow(QWidget *parent = nullptr);
    ~AnimationWindow();
    void init(pixel_table* table_in);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::AnimationWindow *ui;
    pixel_table* table;
    Animation animation;
    QTimer* timer;
    uint framenum;
    void updateAnimation();
    void printPixels(uint*,int,int);
    QImage *image;
    QPixmap pixmap;
};

#endif // ANIMATIONWINDOW_H
