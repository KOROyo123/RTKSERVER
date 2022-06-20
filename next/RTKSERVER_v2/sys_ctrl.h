/*
*   主要功能
*       1、控制系统的各项功能
*       2、记录和展示当前程序的运行状态、资源消耗
*       3、受接口控制
*
*/
#ifndef SYS_CTRL_H
#define SYS_CTRL_H
#include <QObject>
#include <QTimer>
#include <QString>
//#include "koro.h"
#include "rtklib.h"
#include "navi.h"
#include "sql_trans.h"
#include "svr_core.h"





//指令接口具体设置结构体
struct ctrlsvr_t
{
    int state;
    int cycle;
    int buffsize;
    int npb;
    unsigned char *buff; /* input buffers */
    unsigned char *pbuf; /* peek buffer */
    unsigned int tick;  /* start tick */
    stream_t *stream;

    thread_t thread;    /* server thread */
    lock_t lock;        /* lock flag */
};

//控制接口参数结构体
struct conctrl_t
{
    //连接
    int strtype;
    char strpath[MAXSTR];

    int cycle;
    int buffsize;

    ctrlsvr_t ctrlsvr;

};



//数据库接口参数结构体
struct sqlinfo_t
{
    ToSql *tosql;

    sqlsvr_t *sqlsvr;
    stream_t *moni;
};



//系统控制类  负责各个模块的成员和方法
class SysCtrl:public QObject
{
private:

    //Navi相关
    navilist_t *last_svrs;

    int newNaviSvrs(navilist_t *new_svrs);
    int delNaviSvrs(navilist_t *del_svrs);

    //控制接口相关
    int ctrlsvrinit(ctrlsvr_t *svr);


    //core相关
    int coresvrinit(coresvr_t *svr);
    int svrcoreinit(svrcore_t *svrcore);


    //sql相关



    //控制台显示相关

    int printSysState();

    int printNavilist();


public:
    SysCtrl();

    //定时器  周期性输出用







    /* ---------------整个程序的基础配置文件路径---------------------*/

    char inipath[MAXPATH]; //配置路径

    /* ------------------各个模块的参数设置------------------------*/

    //外部通信/控制 连接参数
    conctrl_t *conctrl;
    //core设置 处理参数
    svrcore_t *svrcore;
    //数据库 连接设置
    sqlinfo_t *sqlinfo;

    /* ---------------基线信息记录与各模块间数据交换-----------------*/

    //rtk线程状态记录（双向链表） 记录所有GNSS解算线程的信息
    navilist_t *navilist;

    //core线程数据与状态记录   记录core线程的状态，数据融合的结果，和解算质量、错误信息等内容

    //sql线程数据与状态记录    记录当前SQL线程的状态，GNSS基线输出到数据库是否成功和错误信息等


    /* ------------------------主线程方法------------------------*/

    //程序初始化相关
    int Init(int argc, char *argv[]);
    int Start();


    /* ------------------------控制接口方法------------------------*/

    //外部通信/控制相关
    int createCtrlThread(conctrl_t *conctrl);

    //指令执行函数
    int cmdProcess(char *cmd);


    //int ctrlThread();


    /* -----------------------Navi线程方法------------------------*/
    //Navi相关

    int createNaviThread(char *confpath);
    int createNaviThread(navilist_t svrtask);


    /* -----------------------Core线程方法------------------------*/
    //Core相关
    int loadCoreOpt(char *coreopt);
    int createCoreThread(svrcore_t *svrcore);
    int stopCoreThread(svrcore_t *svrcore);


    /* -----------------------SQL线程方法-------------------------*/
    //SQL相关
    int createSqlThread(sqlinfo_t *sqlinfo);
        //SQL输出设置 输出内容控制、表名格式控制、输出间隔控制



    //控制台输出函数
    void printlogo();

    void printstate();

public:
    QTimer Timer;


public slots:
    void printInfo();

};


extern void navilistlock  (navilist_t *naviitem);
extern void navilistunlock(navilist_t *naviitem);


#endif // SYS_CTRL_H
