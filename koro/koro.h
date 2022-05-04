#ifndef KORO_H
#define KORO_H


#include <math.h>

namespace koro
{
    int char2arg(char *str, int *argc, char **argv, int max_argc);

    void llh2xyz(double &x, double &y, double &z);
    void xyz2llh(double &x, double &y, double &z);

    void enu2pose(double *p0, double *p1, double *p2,double *pose);
};

#endif // KORO_H


