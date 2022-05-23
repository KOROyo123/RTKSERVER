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




SysCtrl::SysCtrl()
{
    //类内成员初始化
    svrs=last_svrs=nullptr;


    //创建对象


}

int SysCtrl::Init(int argc, char *argv[])
{
    return 0;
}

int SysCtrl::Start()
{
    return 0;
}

int SysCtrl::creatNaviThread(char *confpath)
{
    svrs_t *asvr = nullptr;

    newEmptySvrs(asvr);

    last_svrs->navi = new(std::nothrow) Navi_t;

//    if(anavi==0){
//        //申请内存失败
//        return -1;
//    }

    last_svrs->navi->Init(confpath);
    last_svrs->navi->svrStart();

    //asvr->svr=anavi->svr;
    //asvr->moni=anavi->MonitorOpenPort;

    return 0;

}
