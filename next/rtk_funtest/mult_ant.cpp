#include "mult_ant.h"
#include <QDebug>


mult_ant::mult_ant()
{

}

int mult_ant::readpos(pos_t *pos, int n)
{
    ant_num=n;

    for(int i=0;i<n;i++)
    {
        char ecefx[14],ecefy[14],ecefz[14],q;
        strncpy(ecefx,pos[i].line+16,14);
        strncpy(ecefy,pos[i].line+31,14);
        strncpy(ecefz,pos[i].line+46,14);
        strncpy(&q,pos[i].line+63,1);

        xyz[i][0]=atof(ecefx);
        xyz[i][1]=atof(ecefy);
        xyz[i][2]=atof(ecefz);
        Q[i]=atoi(&q);
    }
}

int mult_ant::cal_avgpos()
{

    for(int i=0;i<3;i++){
        avg_xyz[i]=0;
        for(int j=0;j<3;j++){
            avg_xyz[i]+=(xyz[j][i]-avg_xyz[i])/(j+1);
        }
    }
    ecef2pos(avg_xyz,avg_llh);
    return 0;
}

int mult_ant::cal_enu()
{
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
           delta_xyz[i][j]=xyz[i][j]-avg_xyz[j];
        }

    }

    for(int i=0;i<3;i++){
        ecef2enu(avg_llh,delta_xyz[i],enu[i]);
    }
    return 0;
}

int mult_ant::cal_pose()
{
    double op[3]={0,0,0};
    enu2pose(op,enu[0],enu[1],pose);

    return 0;
}

int mult_ant::enu2pose(double *p0, double *p1, double *p2,double *pose)
{
//    double enu_a[3];
//    //double P1[3],P2[3],alpha,beta,gama;
//    for(int i=0;i<3;i++){
//        enu_a[i]=p1[i]+p2[i]+p3[i];
//    }
//    qDebug()<<enu_a[0]<<enu_a[1]<<enu_a[2];


    double P1[3],P2[3],alpha,beta,gama;
    //double P3[3],alpha2;
    for(int i=0;i<3;i++){
        P1[i]=p1[i]-p0[i];
        P2[i]=p2[i]-p0[i];
        //P3[i]=p3[i]-p0[i];
    }
    gama=atan(P1[0]/P1[1]);

    beta=atan(P1[2]/sqrt(P1[0]*P1[0]+P1[1]*P1[1]));

    double p2e=P2[0]*cos(gama)-P2[1]*sin(gama);
    double p2u=-P2[0]*sin(beta)*sin(gama)-P2[1]*sin(beta)*cos(gama)+P2[2]*cos(beta);

//    double p2e2=P3[0]*cos(gama)-P3[1]*sin(gama);
//    double p2u2=-P3[0]*sin(beta)*sin(gama)-P3[1]*sin(beta)*cos(gama)+P3[2]*cos(beta);


//    double a1=-p2u/p2e;
//    double a2=-p2u2/p2e2;
//    alpha=atan(a1);

//    alpha2=atan(a2);

    alpha=atan(-p2u/p2e);

    //alpha2=atan(-p2u2/p2e2);


    //qDebug()<<alpha<<alpha2<<beta<<gama;


    pose[0]=alpha;             //横滚角 输出 -90° - 90°
    pose[1]=beta;              //俯仰角 输出 -90° - 90°
    pose[2]=atan2(P1[0],P1[1]);//航向角 输出-180° - 180°


    return 0;




}

pos_t::pos_t()
{
    flag=0;

    for(int i=0;i<3;i++){
        avg_pos[i]=0.0;
        expect_value[i]=0.0;
        variance[i]=0.0;
        std[i]=0;
    }
    CEP99=DDRMS=CEP95=RMS=CEP=0.0;
}

void pos_t::Init()
{
   //file->seek(0);//跳转到指定位置

    //读取开始时间和结束时间

   while(file->readLine(line,MAXLINE)){
       if(!strstr(line,"%")){
           break;
       }
       if(strstr(line,"obs start")){
           char start_week[4],start_sec[8];
           strncpy(start_week,line+46,4);
           week[0]=atoi(start_week);
           strncpy(start_sec,line+51,8);
           sec[0]=atoi(start_sec);
       }
       if(strstr(line,"obs end")){
           char end_week[4],end_sec[8];
           strncpy(end_week,line+46,4);
           week[1]=atoi(end_week);
           strncpy(end_sec,line+51,8);
           sec[1]=atoi(end_sec);
       }
       if(strstr(line,"GPST")){
           time_type=1;
       }
       else if(strstr(line,"UTC")){
           time_type=2;
       }
   }
   if(strstr(line,"/"))
   {
       time_type++;
   }
}

void pos_t::Analysis()
{

}

int pos_t::timeMatch(int week, int sec)
{
    char Wek[5]={0},Sec[7]={0};
    int GPSW=0,GPSS=0;

    while(1){
        strncpy(Wek,line,4);
        strncpy(Sec,line+5,6);
        GPSW=atoi(Wek);
        GPSS=atoi(Sec);
        if(GPSW==week && GPSS==sec){
            return 0; //匹配完成
        }
        else if(GPSW==week && GPSS>sec){
            return 1;
        }
        else{
            file->readLine(line,MAXLINE);
        }
    }

}
