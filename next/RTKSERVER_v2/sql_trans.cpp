#include "sql_trans.h"






/* sql fusion thread ------------------------------------------------------*/
#ifdef WIN32
static DWORD WINAPI sqlsvrthread(void *arg)
#else
static void *sqlsvrthread(void *arg)
#endif
{

    qDebug()<<"数据库输出线程已启动";

    //arg 接收到所有Navi 的数据  navisvrs_t
    sqlsvr_t *svr=(sqlsvr_t *)arg;

    navilist_t *navilist=*svr->navilist;
//    navilist_t *navis=svr->navilist;

    corectrl_t *corectrl=*svr->corectrl;
    uint32_t tick;

    int cycle,cputime;


    svr->state=1;



    for (cycle=0;svr->state;cycle++) {

        tick=tickget();

//        if(*navilist==nullptr)
//        {
//           printf("emmmm");
//        }

//        svr->navicount=0;

//        navilist_t *navis=navilist;

//        while(navis!=nullptr)
//        {

//            svr->navicount++;

//            int ss=navis->navi->navisvr->svr->rtk.sol.stat;

//            //printf("%d  ",ss);

//            if(navis->next==nullptr)
//            {
//                printf("\n");
//                break;
//            }
//            else
//            {
//                navis=navis->next;
//            }

//        }

        cputime=(int)(tickget()-tick);

        /* sleep until next cycle */
        //sleepms(1000-cputime);


        printf("sqlsvr is runing\n");
        sleepms(5000);

    }


    //分配空间，读取每个navi线程的数据，


    //判断是否获取所有线程数据

        //不满足则sleep 继续读取数据

    //数据全部获取的历元进行数据的融合处理，超时未获取的数据也同样进行处理，舍弃无数据的测站


    //处理完成生成 放入exchange队列/循环链表  用于写入数据库


    qDebug()<<"数据库输出线程已退出";

    return 0;

}






ToSql::ToSql()
{

}

int ToSql::Init(sqlsvr_t *sqlsvr)
{

    svr=sqlsvr;

    return 0;
}

int ToSql::svrStart()
{
    /* create sql stream server thread */
#ifdef WIN32
    if (!(svr->thread=CreateThread(NULL,0,sqlsvrthread,svr,0,NULL))) {
#else
    if (pthread_create(&svr->thread,NULL,sqlsvrthread,svr)) {
#endif

        svr->state=0;
        return 0;
    }
    return 1;


}











