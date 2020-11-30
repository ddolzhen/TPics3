#ifndef ANIMATION_H
#define ANIMATION_H
#include <QVector>

class Animation
{
public:
    Animation();
    int nframes;
    QVector<unsigned int*> frames;
    unsigned int* frameOut(int);
    void addFrame(unsigned int* frame);
};

#endif // ANIMATION_H
