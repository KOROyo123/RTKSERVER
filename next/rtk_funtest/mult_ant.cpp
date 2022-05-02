#include "mult_ant.h"



mult_ant::mult_ant()
{

}

int mult_ant::readpos(pos_t *pos, int n)
{
    ant_num=n;

    for(int i=0;i<n;i++)
    {
        char ecefx[14],ecefy[14],ecefz[14],q;
        strncpy(ecefy,pos[i].line+16,14);
        strncpy(ecefz,pos[i].line+31,14);
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
        ecef2enu(avg_llh,xyz[i],enu[i]);
    }
    return 0;
}

int mult_ant::cal_pose()
{

}

int mult_ant::enu2pose(double *p0, double *p1, double *p2, double *pose)
{
    double P1[3],P2[3],alpha,beta,gama;
    for(int i=0;i<3;i++){
        P1[i]=p1[i]-p0[i];
        P2[i]=p2[i]-p0[i];
    }
    gama=atan(P1[0]/P1[1]);

    beta=atan((P1[2]*P1[2])/sqrt(P1[0]*P1[0]+P1[1]*P1[1]));

    double p2e=P2[0]*cos(gama)-P2[1]*sin(gama);
    double p2u=-P2[0]*sin(beta)*sin(gama)-P2[1]*sin(beta)*cos(gama)+P2[2]*cos(beta);

    alpha=-atan(p2u/p2e);


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
    char Wek[4],Sec[6];

    while(1){
        strncpy(Wek,line,4);
        strncpy(Sec,line+5,6);
        if(atoi(Wek)==week && atoi(Sec)==sec){
            return 1; //匹配完成
        }
        else if(atoi(Wek)==week && atoi(Sec)>sec){
            return 0;
        }
        else{
            file->readLine(line,MAXLINE);
        }
    }

}
