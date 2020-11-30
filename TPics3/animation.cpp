#include "animation.h"


Animation::Animation()
{
    nframes=0;
    frames=QVector<unsigned int*> (0);
}

unsigned int* Animation::frameOut(int framenum){

    return (frames[framenum]);
}


void Animation::addFrame(unsigned int* frame){
   frames.append(frame);
}
