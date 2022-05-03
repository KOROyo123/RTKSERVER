/*
 * rtklib函数测试与分析
 *
 *
*/

#include <QCoreApplication>
#include<QDebug>
#include "rtklib.h"
#include "korolib.h"
#include <QFile>
#include <QTextStream>
#include "mult_ant.h"












int main()
{


 /*------------------------文件输入---------------------------*/

    //设置文件流  输入文件  输出文件  日志文件

    QFile POS1("G:/data_analysis/SK02_220410_100.pos");
    QFile POS2("G:/data_analysis/SK03_220410_100.pos");
    QFile POS3("G:/data_analysis/SK04_220410_100.pos");

    QFile trace("G:/data_analysis/trace.txt");
    QFile avgpos("G:/data_analysis/avgpos.txt");
    QFile pose("G:/data_analysis/poseDEBUG.txt");



    if(!POS1.open(QIODevice::ReadOnly|QIODevice::Text)){
        qDebug()<<&POS1<<"文件打开失败";
        return 0;
    }
    if(!POS2.open(QIODevice::ReadOnly|QIODevice::Text)){
        qDebug()<<&POS2<<"文件打开失败";
        return 0;
    }
    if(!POS3.open(QIODevice::ReadOnly|QIODevice::Text)){
        qDebug()<<&POS3<<"文件打开失败";
        return 0;
    }

//    if(!trace.open(QIODevice::ReadOnly|QIODevice::Text)){
//        qDebug()<<&trace<<"文件打开失败";
//        return 0;
//    }
//    if(!avgpos.open(QIODevice::ReadOnly|QIODevice::Text)){
//        qDebug()<<&avgpos<<"文件打开失败";
//        return 0;
//    }
    if(!pose.open(QIODevice::WriteOnly|QIODevice::Text)){
        qDebug()<<&pose<<"文件打开失败";
        return 0;
    }



    QTextStream outpose(&pose);
    outpose.setFieldWidth(16);
    outpose.setFieldAlignment(QTextStream::AlignLeft);


/*----------------------POS文件处理部分---------------------------*/

    pos_t pos[3];

    pos[0].file=&POS1;
    pos[1].file=&POS2;
    pos[2].file=&POS3;


    //读取pos文件开始时间和结束时间，
    for(int i=0;i<3;i++){
        pos[i].Init();
    }

    //统计最晚开始时间和最早结束时间，确定数据统计区间

    int week[3]={0};
    //double sec[2]={0.0};
    int sec[3]={0};  //暂时不处理1hz以上数据


    week[0]=pos[0].week[0];
    week[1]=pos[0].week[1];
    sec[0]=pos[0].sec[0];
    sec[1]=pos[0].sec[1];
    for(int i=1;i<3;i++){
        week[0]=week[0]<pos[i].week[0]?pos[i].week[0]:week[0];
        week[1]=week[1]>pos[i].week[1]?pos[i].week[1]:week[1];

        sec[0]=sec[0]<pos[i].sec[0]?pos[i].sec[0]:sec[0];
        sec[1]=sec[1]<pos[i].sec[1]?pos[i].sec[1]:sec[1];
    }

    if(week[1]-week[0]<0){
        //文件时间区间不匹配，
        return 0;
    }
    else if(week[1]-week[0]==0 && sec[1]-sec[0]<0){
        //文件时间区间不匹配，
        return 0;
    }
    week[2]=week[0];
    sec[2]=sec[0];

    //文件处理

    //时间匹配  +缺失数据统计
int n=70000;
while(n){

    for(int i=0;i<3;i++){
        if(pos[i].timeMatch(week[2],sec[2])){
            //出现数据缺失
            qDebug()<<"文件"<<i<<week[2]<<sec[2]<<"数据缺失";

            if(sec[2]==604759){
                week[2]++;
                sec[2]=0;
            }
            else{
                sec[2]++;
            }
            i=0;
        }
    }



/*---------------多天线数据处理部分-----------------------*/

    //匹配数据读取
    mult_ant multant;

    multant.readpos(pos,3);

    //数据分析函数

    //计算坐标平均值

    multant.cal_avgpos();

    multant.cal_enu();

    multant.cal_pose();

    //double llh_avg[3];

    //ecef2pos(ecef_avg,llh_avg);


    //ecef2enu()

    //double avg+=(x-avg)/n

    //结果输出函数

   // qDebug()<<sec[2]<<multant.pose[0]*R2D<<multant.pose[1]*R2D<<multant.pose[2]*R2D;

    outpose<<sec[2]<<multant.pose[0]*R2D<<multant.pose[1]*R2D<<multant.pose[2]*R2D<<"\n";

    sec[2]++;
    n--;
}
    return 0;

}




