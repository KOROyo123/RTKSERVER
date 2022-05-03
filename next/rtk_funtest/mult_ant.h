#ifndef MULT_ANT_H
#define MULT_ANT_H
#include <QFile>
#include <QTextStream>
#include "rtklib.h"




#define MAXLINE 256




class pos_t
{
public:
    pos_t();

protected:


private:


    int flag; //数据处理标记

public:
    QFile *file;//文件首地址
    char *line=new char[MAXLINE];//读取一行数据用



    //文件头信息
    int week[2];
    int sec[2];
    //double sec[2];
    int time_type;//时间的格式 1:GPST week+sec  2:GPST YMD+HMS 3:UTC YMD+HMS

    int lack_count;//缺失数据计数

    double avg_pos[3];//ECEF坐标系下的平均坐标  期望值
    double expect_value[3];
    double variance[3];//方差
    double std[3];//标准差

    double CEP;//               50%
    double RMS;//均方根误差  DRMS 63%-68%
    double CEP95;//             95%
    double DDRMS;//2DRMS        96%-98%
    double CEP99;//             99%

                 //误差椭球



    void Init();//获取文件头信息
    void Analysis();//数据分析


    int timeMatch(int week, int sec);
};

class mult_ant
{
public:
    mult_ant();


    int ant_num;//多天线文件数量

    double xyz[3][3];
    int Q[3];

    double avg_xyz[3];
    double avg_llh[3];
    double delta_xyz[3][3];
    double enu[3][3];
    double pose[3];


    int readpos(pos_t *pos, int n);
    int cal_avgpos();
    int cal_enu();
    int cal_pose();

    int enu2pose(double *p0,double *p1,double *p2,double *pose);

};

struct output_t
{
    QFile *trace;
    QFile *avgpos;
    QFile *attitude;

    int count;
};





#endif // MULT_ANT_H
