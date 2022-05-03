#include "koro.h"



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

/* 把字符串转换为argc 和 argv ------------------------------------------------------
* 利用三个点确定一个平面，输出平面相对于当前坐标系下的姿态信息
* args   : double *p0       I   载体中心点的坐标{x,y,z}或{e,n,u}
*          double *p1       I   航向坐标，P0→P1指向为航向
*          double *p2       I   P0-P1直线外任意一点坐标，P0,P1,P2构建出一个确定的平面
*          double *pose     O   attitude {alpha,beta,gama} (rad)  {横滚角，俯仰角，航向角}
* return : none
*-----------------------------------------------------------------------------*/
int koro::char2arg(char *str, int *argc, char **argv, int number)
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

void koro::llh2xyz(double &x, double &y, double &z)
{
    double L = x * d2r;
    double B = y * d2r;
    double H = z;

    double N = a / sqrt(1 - e * e * sin(B) * sin(B));
    x = (N + H) * cos(B) * cos(L);
    y = (N + H) * cos(B) * sin(L);
    z = (N * (1 - e * e) + H) * sin(B);

}

void koro::xyz2llh(double &x, double &y, double &z)
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
/* 三点计算姿态 ------------------------------------------------------
* 利用三个点确定一个平面，输出平面相对于当前坐标系下的姿态信息
* args   : double *p0       I   载体中心点的坐标{x,y,z}或{e,n,u}
*          double *p1       I   航向坐标，P0→P1指向为航向
*          double *p2       I   P0-P1直线外任意一点坐标，P0,P1,P2构建出一个确定的平面
*          double *pose     O   attitude {alpha,beta,gama} (rad)  {横滚角，俯仰角，航向角}
* return : none
*-----------------------------------------------------------------------------*/
void koro::enu2pose(double *p0, double *p1, double *p2,double *pose)
{

    double P1[3],P2[3],alpha,beta,gama;

    for(int i=0;i<3;i++){
        P1[i]=p1[i]-p0[i];
        P2[i]=p2[i]-p0[i];
    }
    gama=atan(P1[0]/P1[1]);

    beta=atan(P1[2]/sqrt(P1[0]*P1[0]+P1[1]*P1[1]));

    double p2e=P2[0]*cos(gama)-P2[1]*sin(gama);
    double p2u=-P2[0]*sin(beta)*sin(gama)-P2[1]*sin(beta)*cos(gama)+P2[2]*cos(beta);

    alpha=atan(-p2u/p2e);

    pose[0]=alpha;             //横滚角 输出 -90° - 90°
    pose[1]=beta;              //俯仰角 输出 -90° - 90°
    pose[2]=atan2(P1[0],P1[1]);//航向角 输出-180° - 180°

}

