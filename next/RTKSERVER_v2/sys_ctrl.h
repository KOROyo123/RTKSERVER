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

#include "interface.h"
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



class SysCtrl
{
private:

    svrs_t *last_svrs;



    int newEmptySvrs(svrs_t *new_svrs);

    int delExistSvrs(svrs_t *del_svrs);



public:
    SysCtrl();

    char inipath[MAXPATH]; //配置路径


    //rtk线程状态记录

    svrs_t *svrs;






    //sql控制部分
    //数据库连接参数
    QString dbconnType;
    QString hostName;
    int port;
    QString dbName;
    QString userName;
    QString password;


    //SQL输出设置 输出内容控制、表名格式控制、输出间隔控制


    int Init(int argc, char *argv[]);

    int Start();

    int creatNaviThread(char *confpath);

    int creatNaviThread(svrs_t svrtask);





};

#endif // SYS_CTRL_H
