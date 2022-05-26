/*
*   主要功能
*       1、控制系统的各项功能
*       2、记录和展示当前程序的运行状态、资源消耗
*       3、受接口控制
*
*/
#ifndef SYS_CTRL_H
#define SYS_CTRL_H
#include <QString>
#include "koro.h"
#include "rtklib.h"


#include "navi.h"
#include "sql_trans.h"
#include "svr_core.h"


struct svrs_t
{
    //基线名称
    char baseline[MAXCHAR];

    //基站地址
    //char base_str[MAXCHAR];

    //测站地址
    //char rover_str[MAXCHAR];

    //
    Navi_t *navi;

    //svr指针
    rtksvr_t *svr;

    //moni端口
    stream_t *moni;

    svrs_t *last,*next;

};


struct sqlinfo_t
{
    //sql控制部分
    //数据库连接参数
    QString dbconnType;
    QString hostName;
    int port;
    QString dbName;
    QString userName;
    QString password;
};

struct svrcore_t
{

};


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

struct conctrl_t
{
    //连接
    int strtype;
    char strpath[MAXSTR];

    int cycle;
    int buffsize;


    ctrlsvr_t ctrlsvr;

};

class SysCtrl
{
private:

    svrs_t *last_svrs;

    //Navi相关
    int newEmptySvrs(svrs_t *new_svrs);
    int delExistSvrs(svrs_t *del_svrs);


    int ctrlsvrinit(ctrlsvr_t *svr);

public:
    SysCtrl();

    char inipath[MAXPATH]; //配置路径

    //rtk线程状态记录（双向链表）
    svrs_t *svrs;
    //数据库 连接设置
    sqlinfo_t *sqlinfo;
    //core设置 处理参数
    svrcore_t *svrcore;
    //外部通信/控制 连接参数
    conctrl_t *conctrl;

    //SQL输出设置 输出内容控制、表名格式控制、输出间隔控制

    //程序初始化相关
    int Init(int argc, char *argv[]);
    int Start();

    //外部通信/控制相关
    int openControl();

    int createCtrlThread(conctrl_t *conctrl);

    int cmdProcess(char *cmd);


    //int ctrlThread();


    //Navi相关

    int createNaviThread(char *confpath);
    int createNaviThread(svrs_t svrtask);


    //Core相关




    //SQL相关




};

#endif // SYS_CTRL_H
