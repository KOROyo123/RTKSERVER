/*
*   主要功能
*       1、从各个RTK线程主动获取解算结果和过程信息
*       2、进行各个解结果的时间同步
*       3、解算结果的二次处理
*       4、输出结果到数据库任务队列
*
*/
#ifndef SVR_CORE_H
#define SVR_CORE_H

#include "rtklib.h"
#include "navi.h"

//多数据融合接口参数结构体,coreThread线程的参数


struct coreexchange_t
{
    //历元时间
    time_t time;


    //程序标记

    //融合结果写入完成标记
    int core_flag;

    //数据库线程读取完成标记
    int sql_flag;


    //指针

    int sqlcount;
    struct coreexchange_t *last;
    struct coreexchange_t *next;

};


struct coresvr_t
{
    navilist_t **navilist;


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





    int navicount;




    //dataExchangeCycle

    struct coreexchange_t *exchangelist;

    struct coreexchange_t *coreanchor;

    struct coreexchange_t *sqlanchor;


};




class Core
{
public:
    Core();

    ~Core();
protected:


private:
    int stste; //线程状态 运行还是停止
    int step;//数据处理已经完成的流程





public:

    coresvr_t *svr;
    stream_t *moni;



    int Init(coresvr_t *coresvr);

    int svrStart();
    int svrStop();

    int resetState();



};

struct corectrl_t
{

    Core *core;

    coresvr_t *coresvr;
    stream_t *moni;
};




#endif // SVR_CORE_H
