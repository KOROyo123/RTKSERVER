/*
 * rtklib函数测试与分析
 *
 *
*/

#include <QCoreApplication>
#include<QDebug>
#include "rtklib.h"
#include "korolib.h"


/* multiply matrix -----------------------------------------------------------
* multiply matrix by matrix (C=alpha*A*B+beta*C)
* args   : char   *tr       I  transpose flags ("N":normal,"T":transpose)
*          int    n,k,m     I  size of (transposed) matrix A,B
*          double alpha     I  alpha
*          double *A,*B     I  (transposed) matrix A (n x m), B (m x k)
*          double beta      I  beta
*          double *C        IO matrix C (n x k)
* return : none
*-----------------------------------------------------------------------------*/
//n代表第一个矩阵的行，k代表第二个矩阵的列，m代表第一个矩阵的列和第二个矩阵的行。作者之所以把n和k放在m前面，看起来顺序反逻辑，其实是矩阵相乘一共要循环n*k次，从编程的顺序来排的。
extern void matmul2(const char *tr, int n, int k, int m, double alpha,
                   const double *A, const double *B, double beta, double *C)
{
    double d;
    int i,j,x,f=tr[0]=='N'?(tr[1]=='N'?1:2):(tr[1]=='N'?3:4);//NN:1  NT:2 TN:3 TT:4   N表示原始矩阵，T表示转置

    for (i=0;i<n;i++) for (j=0;j<k;j++) {
        d=0.0;
        switch (f) {
            case 1: for (x=0;x<m;x++) {d+=A[i+x*n]*B[x+j*m];qDebug()<<i+x*n<<x+j*m<<i+j*n;} break;
            case 2: for (x=0;x<m;x++) {d+=A[i+x*n]*B[j+x*k];qDebug()<<i+x*n<<j+x*k<<i+j*n;} break;
            case 3: for (x=0;x<m;x++) {d+=A[x+i*m]*B[x+j*m];qDebug()<<x+i*m<<x+j*m<<i+j*n;} break;
            case 4: for (x=0;x<m;x++) {d+=A[x+i*m]*B[j+x*k];qDebug()<<x+i*m<<j+x*k<<i+j*n;} break;
        }
        if (beta==0.0) C[i+j*n]=alpha*d; else C[i+j*n]=alpha*d+beta*C[i+j*n];
    }
}


#include <iostream>

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

int main()
{
//    double x = 120.102335386;
//    double y = 35.999456018;
//    double z = 93.7639;

    double x=-2591041.8651;
    double y=4469361.8954;
    double z=3728197.9560;

    //printf("原大地经纬度坐标：%.10lf\t%.10lf\t%.10lf\n", x, y, z);
    //Blh2Xyz(x, y, z);

    printf("地心地固直角坐标：%.10lf\t%.10lf\t%.10lf\n", x, y, z);
    Xyz2Blh(x, y, z);
    printf("转回大地经纬度坐标：%.10lf\t%.10lf\t%.10lf\n", x, y, z);
}




