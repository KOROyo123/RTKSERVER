#include "sys_ctrl.h"



//创建并初始化一个新的svrs结构体
int SysCtrl::newEmptySvrs(svrs_t *new_svrs)
{

    //申请空间
    new_svrs=new(std::nothrow) svrs_t;

    if(new_svrs==0){
        //申请内存失败
        return -1;
    }

    //初始化参数
    new_svrs->baseline[0]={'\0'};
    new_svrs->svr=nullptr;
    new_svrs->moni=new stream_t;
    new_svrs->last=last_svrs;
    new_svrs->next=nullptr;



    //sys 记录更新
    if(last_svrs!=nullptr){
        last_svrs->next=new_svrs;
    }

    last_svrs=new_svrs;

    if(svrs==nullptr){
        svrs=last_svrs;
    }

    return 0;
}

int SysCtrl::delExistSvrs(svrs_t *del_svrs)
{
    if(svrs==del_svrs){
        svrs=nullptr;
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




SysCtrl::SysCtrl()
{
    //类内成员初始化
    svrs=last_svrs=nullptr;


    //创建对象
    //申请空间
    conctrl=new(std::nothrow) conctrl_t;


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



    //设置数据库参数








    return 0;
}

int SysCtrl::Start()
{

     //根据设置的参数


    //开启control通讯线程
    createCtrlThread(conctrl);



    //开启core处理线程



    //开启数据库输出线程


    return 0;
}





/* stearm server thread ------------------------------------------------------*/
#ifdef WIN32
static DWORD WINAPI ctrlsvrthread(void *arg)
#else
static void *ctrlsvrthread(void *arg)
#endif
{
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



    return 0;
}



int SysCtrl::createNaviThread(char *confpath)
{
    svrs_t *asvr = nullptr;

    newEmptySvrs(asvr);

    last_svrs->navi = new(std::nothrow) Navi_t;

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


