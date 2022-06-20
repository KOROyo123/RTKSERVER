#include "sys_ctrl.h"


//创建并初始化一个新的svrs结构体
int SysCtrl::newNaviSvrs(navilist_t *new_svrs)
{

    //申请空间
    new_svrs=new(std::nothrow) navilist_t;

    if(new_svrs==0){
        //申请内存失败
        return -1;
    }

    //初始化参数
//    new_svrs->data->baseline[0]={'\0'};
//    new_svrs->baseline[0]={'\0'};
    new_svrs->data=nullptr;
    new_svrs->moni=new stream_t;
    new_svrs->last=last_svrs;
    new_svrs->next=nullptr;



    //sys 记录更新
    if(last_svrs!=nullptr){
        last_svrs->next=new_svrs;
    }

    last_svrs=new_svrs;

    if(navilist==nullptr){
        navilist=last_svrs;
    }

    return 0;
}

int SysCtrl::delNaviSvrs(navilist_t *del_svrs)
{
    if(navilist==del_svrs){
        navilist=nullptr;
    }

    delete del_svrs;

    return 0;
}

int SysCtrl::ctrlsvrinit(ctrlsvr_t *svr)
{

    svr->stream=new(std::nothrow) stream_t;

    svr->state=0;
    svr->cycle=0;
    svr->buffsize=0;
    svr->npb=0;
    svr->buff=svr->pbuf=NULL;
    svr->tick=0;

    strinit(svr->stream);
    svr->thread=0;
    initlock(&svr->lock);

    return 0;
}

int SysCtrl::coresvrinit(coresvr_t *svr)
{

   //设置 svrcore的基本参数

    return 0;
}

int SysCtrl::printSysState()
{


    //ctrl运行状态
    if(conctrl->ctrlsvr.state==1)
    {
        qDebug()<<"系统控制模块：运行中  指令接收端口"<<conctrl->strpath;
    }
    else if(conctrl->ctrlsvr.state==0)
    {
        qDebug()<<"系统控制模块：已停止";
    }










    //core运行状态
    if(svrcore->coresvr->state==1)
    {

        qDebug()<<"数据融合模块：运行中  Navi任务数量:"<<svrcore->coresvr->navicount;
    }
    else if(svrcore->coresvr->state==1==0)
    {
        qDebug()<<"数据融合模块：已停止";
    }











    //SQL运行状态
    //core运行状态
    if(sqlinfo->sqlsvr->state==1)
    {
        printf("数据库模块：运行中");
    }
    else if(conctrl->ctrlsvr.state==0)
    {
        printf("数据库模块：已停止");
    }


    qDebug()<<"-----------------------------------------";



}

int SysCtrl::printNavilist()
{
    navilist_t *navis=navilist;

    qDebug()<<"Navilist:";

    while(navis!=nullptr)
    {

        //

        char *roverstr=navis->navi->StrPath[0];
        char *basestr=navis->navi->StrPath[1];

        double *ecef=navis->navi->svr->rtk.sol.rr;
        int state=navis->navi->svr->rtk.sol.stat;
        int ns=navis->navi->svr->rtk.sol.ns;

        int moni=navis->navi->MonitorOpenPort;


        printf("RoverStream: %s  BaseStream: %s\n",roverstr,basestr);
        printf("sol:%lf  %lf  %lf state:%d  ns:%d\n",ecef[0],ecef[1],ecef[2],state,ns);
        printf("moniport:%d\n",moni);

        printf("\n");

        if(navis->next==nullptr)
        {
            printf("\n");
            break;
        }
        else
        {
            navis=navis->next;
        }

    }


}




SysCtrl::SysCtrl()
{

    printlogo();

    //创建对象
    //申请空间
    conctrl=new(std::nothrow) conctrl_t;

    svrcore=new(std::nothrow) svrcore_t;

    sqlinfo=new(std::nothrow) sqlinfo_t;



    //子对象空间分配
    svrcore->core=new(std::nothrow) Core;
    svrcore->coresvr=new(std::nothrow) coresvr_t;
    svrcore->moni=new(std::nothrow) stream_t;


    sqlinfo->tosql=new(std::nothrow) ToSql;
    sqlinfo->sqlsvr=new(std::nothrow)sqlsvr_t;
    sqlinfo->moni=new(std::nothrow)stream_t;

    //类内成员初始化
    navilist=last_svrs=nullptr;

    svrcore->coresvr->navilist=&navilist;

    sqlinfo->sqlsvr->navilist=&navilist;


}

int SysCtrl::Init(int argc, char *argv[])
{

    //控制连接参数与设置
    conctrl->cycle=10;
    conctrl->buffsize=32768;
    conctrl->strtype=STR_TCPSVR;

    char default_stream[]={":20005"};
    strncpy(conctrl->strpath,default_stream,MAXPATH);


    //设置core线程参数
    //svrcore->



    //设置数据库参数
    //sqlinfo->



    //连接信号和槽
    connect(&Timer,&QTimer::timeout,this,&SysCtrl::printInfo);


    //设置定时器
    Timer.setInterval(2000);
    Timer.setSingleShot(false);


    return 0;
}

int SysCtrl::Start()
{

     //根据设置的参数


    //开启control通讯线程
    ;

    if(createCtrlThread(conctrl))
    {
        qDebug()<<"指令控制模块正在启动";
    }


    //开启core处理线程
    if(createCoreThread(svrcore))
    {
        qDebug()<<"数据融合模块正在启动";
    }


    //开启数据库输出线程
    if(createSqlThread(sqlinfo))
    {
        qDebug()<<"数据库融合模块正在启动";
    }



    //打开定时器
    sleepms(2000);
    Timer.start();

    return 0;
}





/* stearm server thread ------------------------------------------------------*/
#ifdef WIN32
static DWORD WINAPI ctrlsvrthread(void *arg)
#else
static void *ctrlsvrthread(void *arg)
#endif
{
    qDebug()<<"指令控制线程已启动";

    SysCtrl *sys=(SysCtrl *)arg;

    ctrlsvr_t *svr=&sys->conctrl->ctrlsvr;

    uint32_t tick,tick_nmea;
    uint8_t buff[1024];
    int i,n,cyc;


    svr->tick=tickget();
    tick_nmea=svr->tick-1000;

    for (cyc=0;svr->state;cyc++) {
        tick=tickget();

        /* read data from input stream */
        while ((n=strread(svr->stream,svr->buff,svr->buffsize))>0&&svr->state) {

            /* write data to log stream */
            //strwrite(svr->strlog,svr->buff,n);


            //数据解码与对应处理
            lock(&svr->lock);

            char txt[n+1];
            for(int i=0;i<n;i++){
                txt[i]=svr->buff[i];
            }
            txt[n]='\0';



            sys->cmdProcess(txt);

            unlock(&svr->lock);


            strwrite(svr->stream,svr->buff,n);

            lock(&svr->lock);
            for (i=0;i<n&&svr->npb<svr->buffsize;i++) {
                svr->pbuf[svr->npb++]=svr->buff[i];
            }
            unlock(&svr->lock);
        }


        sleepms(svr->cycle-(int)(tickget()-tick));
    }

    free(svr->buff); svr->buff=NULL;
    free(svr->pbuf); svr->pbuf=NULL;

    return 0;

}











int SysCtrl::createCtrlThread(conctrl_t *conctrl)
{
//    int i,rw,stropt[8]={0};
    char file1[MAXSTRPATH],file2[MAXSTRPATH],*p;


    ctrlsvr_t *svr=&conctrl->ctrlsvr;

    ctrlsvrinit(svr);

    svr->cycle=conctrl->cycle;
    svr->buffsize=conctrl->buffsize;

    if (!(svr->buff=(unsigned char *)malloc(svr->buffsize))||
        !(svr->pbuf=(unsigned char *)malloc(svr->buffsize))) {
        free(svr->buff); free(svr->pbuf);
        return 0;
    }


    /* open streams */
    strcpy(file1,conctrl->strpath); if ((p=strstr(file1,"::"))) *p='\0';
    strcpy(file2,conctrl->strpath); if ((p=strstr(file2,"::"))) *p='\0';

    if (!stropen(svr->stream,conctrl->strtype,STR_MODE_RW,conctrl->strpath)){
        strclose(svr->stream);
        qDebug()<<"数据流打开失败";
        return 0;
    }
    qDebug()<<"control link is open, port"<<conctrl->strpath;


//    strwrite(svr->stream,(unsigned char *)"",0); /* for connect */


    svr->state=1;

    /* create control stream server thread */
#ifdef WIN32
    if (!(svr->thread=CreateThread(NULL,0,ctrlsvrthread,this,0,NULL))) {
#else
    if (pthread_create(&svr->thread,NULL,ctrlsvrthread,this)) {
#endif

        svr->state=0;
        return 0;
    }
    return 1;

}

int SysCtrl::cmdProcess(char *cmd)
{
    qDebug()<<cmd;


    if (strstr(cmd, "start a task") != NULL)
    {
        createNaviThread("D:/RTKSERVER/conf/server_mult/test1.conf");
        qDebug()<<"one task is create";
    }

    if (strstr(cmd, "start core") != NULL)
    {
        if(svrcore->coresvr->state==1)
        qDebug()<<"数据融合模块正在运行";
        else if(svrcore->coresvr->state==0)
        {
            //开启core处理线程
            if(createCoreThread(svrcore))
            {
                qDebug()<<"数据融合模块正在启动";
            }
        }

    }

    if (strstr(cmd, "stop core") != NULL)
    {
        if(svrcore->coresvr->state==0)
        qDebug()<<"数据融合模块已停止";
        else if(svrcore->coresvr->state==1)
        {
            //开启core处理线程
            if(stopCoreThread(svrcore))
            {
                qDebug()<<"数据融合模块正在停止";
            }
        }

    }



    return 0;
}



int SysCtrl::createNaviThread(char *confpath)
{
    navilist_t *asvr = nullptr;

    newNaviSvrs(asvr);

    last_svrs->navi = new(std::nothrow) Navi;

    if(last_svrs->navi==0){
        //申请内存失败
        return -1;
    }

    last_svrs->navi->Init(confpath);
    last_svrs->navi->svrStart();

    //last_svrs->svr=last_svrs->navi->svr;

    //asvr->svr=anavi->svr;
    //asvr->moni=anavi->MonitorOpenPort;

    return 0;

}

 int SysCtrl::createNaviThread(navilist_t svrtask)
{
     return 0;
 }

 int SysCtrl::loadCoreOpt(char *coreopt)
 {
     //从ini文件读取并设置svrcore的参数

     return 0;
 }

int SysCtrl::createCoreThread(svrcore_t *svrcore)
{

    coresvr_t *svr=svrcore->coresvr;


    //设置svr的具体参数哦
    svr->state=0;


    svrcore->core->Init(svrcore->coresvr);
    //svrcore->core->svrStart();


    return svrcore->core->svrStart();;
}

int SysCtrl::stopCoreThread(svrcore_t *svrcore)
{
    svrcore->coresvr->state=0;
}

int SysCtrl::createSqlThread(sqlinfo_t *sqlinfo)
{

    sqlinfo->sqlsvr->state=-1;


    return 0;
}

void SysCtrl::printlogo()
{
    qDebug()<<"+-------------------------------------------------------------------------------------------------------------------------------+\n"
              "|                                                                                                                               |\n"
              "|  XXXXXXXXXX     XXXXXXXXXXXXXXX   XXX       XX     XXXXXXXXX                                                                  |\n"
              "|  XXXXXXXXXXX    XXXXXXXXXXXXXXX   XXX     XXX     XXX    XXXX                                                                 |\n"
              "|  XXX      XXX   X     XXX     X   XXX    XXX     XXX        X                                                                 |\n"
              "|  XXX      XX          XXX         XXX  XXX        XXX             XXXXX     XX    XXXX XXX       XXX    XXXXX     XX    XXXX  |\n"
              "|  XXXXXXXXXX           XXX         XXXXXX          XXXXXXXX      XXX    XXX   XX  XX     XXX     XXX   XXX    XXX   XX  XX     |\n"
              "|  XXXXXXXX             XXX         XXXXXXX           XXXXXXXXX  XX       XXX   XXXX       XXX   XXX   XX       XXX   XXXX      |\n"
              "|  XXX   XXX            XXX         XXX XXXX                XXXX XX  XXXXXXX    XX          XX   XX    XX  XXXXXXX    XX        |\n"
              "|  XXX    XXX           XXX         XXX   XXXX               XXX XX             XX           XX XX     XX             XX        |\n"
              "|  XXX     XXX          XXX         XXX     XXX     XXX    XXXX   XXX    XXX    XX            XXX       XXX    XXX    XX        |\n"
              "|  XXX      XXXX       XXXXX        XXX      XXX     XXXXXXXXX      XXXXXX      XX             X          XXXXXX      XX        |\n"
              "|                                                                                                                               |\n"
              "+-------------------------------------------------------------------------------------------------------------------------------+\n";
}

void SysCtrl::printstate()
{

    printSysState();



}

void SysCtrl::printInfo()
{
#ifdef WIN32
        system("cls");
#else
        system("clear");
#endif

    printlogo();

    printstate();

    printNavilist();

}


extern void navilistlock  (navilist_t *naviitem) {lock  (&naviitem->lock);}
extern void navilistunlock(navilist_t *naviitem) {unlock(&naviitem->lock);}
