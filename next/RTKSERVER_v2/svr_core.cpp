#include "svr_core.h"





/* multisensor data fusion thread ------------------------------------------------------*/
#ifdef WIN32
static DWORD WINAPI coresvrthread(void *arg)
#else
static void *coresvrthread(void *arg)
#endif
{

    qDebug()<<"数据融合线程已启动";

    //arg 接收到所有Navi 的数据  navisvrs_t
    coresvr_t *svr=(coresvr_t *)arg;

    navilist_t**navilist=svr->navilist;
//    navilist_t *navis=svr->navilist;


    uint32_t tick;

    int cycle,cputime;


    svr->state=1;



    for (cycle=0;svr->state;cycle++) {

        tick=tickget();

//        if(*navilist==nullptr)
//        {
//           printf("emmmm");
//        }

        svr->navicount=0;

        navilist_t *navis=*navilist;

        while(navis!=nullptr)
        {

            svr->navicount++;

            int ss=navis->navi->navisvr->svr->rtk.sol.stat;

            //printf("%d  ",ss);

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

        cputime=(int)(tickget()-tick);

        /* sleep until next cycle */
        //sleepms(1000-cputime);


        //printf("coresvr is runing\n");
        sleepms(5000);

    }


    //分配空间，读取每个navi线程的数据，


    //判断是否获取所有线程数据

        //不满足则sleep 继续读取数据

    //数据全部获取的历元进行数据的融合处理，超时未获取的数据也同样进行处理，舍弃无数据的测站


    //处理完成生成 放入exchange队列/循环链表  用于写入数据库


    qDebug()<<"数据融合线程已退出";

    return 0;

}





Core::Core()
{
    //空间分配
    //svr=new coresvr_t;
    moni=new stream_t;


    resetState();
}

int Core::Init(coresvr_t *coresvr)
{
    //类内对象初始化
    svr=coresvr;


    return 0;
}

int Core::svrStart()
{


    /* create control stream server thread */
#ifdef WIN32
    if (!(svr->thread=CreateThread(NULL,0,coresvrthread,svr,0,NULL))) {
#else
    if (pthread_create(&svr->thread,NULL,ctrlsvrthread,svr)) {
#endif

        svr->state=0;
        return 0;
    }
    return 1;


}

int Core::svrStop()
{

}

//构建类和重置类时使用，重置状态变量的值
int Core::resetState()
{
    strinit(moni);
}



