#include "korolib.h"

#include <iostream>
#include <math.h>

using namespace std;

const double epsilon = 0.000000000000001;
const double pi = 3.14159265358979323846;
const double d2r = pi / 180;
const double r2d = 180 / pi;

const double a = 6378137.0;		//椭球长半轴
const double f_inverse = 298.257223563;			//扁率倒数
const double b = a - a / f_inverse;
//const double b = 6356752.314245;			//椭球短半轴

const double e = sqrt(a * a - b * b) / a;



Korolib::Korolib()
{
}




void Blh2Xyz(double &x, double &y, double &z)
{
    double L = x * d2r;
    double B = y * d2r;
    double H = z;

    double N = a / sqrt(1 - e * e * sin(B) * sin(B));
    x = (N + H) * cos(B) * cos(L);
    y = (N + H) * cos(B) * sin(L);
    z = (N * (1 - e * e) + H) * sin(B);
}

void Xyz2Blh(double &x, double &y, double &z)
{
    double tmpX =  x;
    double temY = y ;
    double temZ = z;

    double curB = 0;
    double N = 0;
    double calB = atan2(temZ, sqrt(tmpX * tmpX + temY * temY));

    int counter = 0;
    while (abs(curB - calB) * r2d > epsilon  && counter < 25)
    {
        curB = calB;
        N = a / sqrt(1 - e * e * sin(curB) * sin(curB));
        calB = atan2(temZ + N * e * e * sin(curB), sqrt(tmpX * tmpX + temY * temY));
        counter++;
    }

    x = atan2(temY, tmpX) * r2d;
    y = curB * r2d;
    z = temZ / sin(curB) - N * (1 - e * e);
}

//int main()
//{
//    double x = 120.102335406;
//    double y = 35.999456003;
//    double z = 93.7490;

//    printf("原大地经纬度坐标：%.10lf\t%.10lf\t%.10lf\n", x, y, z);
//    Blh2Xyz(x, y, z);

//    printf("地心地固直角坐标：%.10lf\t%.10lf\t%.10lf\n", x, y, z);
//    Xyz2Blh(x, y, z);
//    printf("转回大地经纬度坐标：%.10lf\t%.10lf\t%.10lf\n", x, y, z);
//}


int char2arg(char* str, int* argc, char** argv, int number)
{
    char *p;
    int num=0;
    int word_start = 1;

    if(argc == NULL || argv == NULL)
        return -1;

    p=str;

    while(*p){
        if((*p == '\r') || (*p == '\n')){
            *p = '\0';
            break;
        }
        if((*p == ' ') || (*p == '\t')){
            word_start = 1;
            *p = '\0';
            p++;
            continue;
        }
        if(num >= number)
            break;

        if(word_start){
            argv[num++] = p;
            word_start = 0;
        }
        p++;
    }

    *argc = num;

    return 0;
}








