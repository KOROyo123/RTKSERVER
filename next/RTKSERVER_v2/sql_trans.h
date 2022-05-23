/*
*   主要功能
*       1、建立一个与数据库的连接
*       2、根据设置周期性的向数据库写入数据
*       3、
*
*/
#ifndef TOSQL_H
#define TOSQL_H

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include "koro.h"
#include "rtklib.h"

//存放一个测站的所有结果的结构体
struct sols
{

    gtime_t time;       /* time (GPST) */
    double rr[6];       /* position/velocity (m|m/s) */
                        /* {x,y,z,vx,vy,vz} or {e,n,u,ve,vn,vu} */
    float  qr[6];       /* position variance/covariance (m^2) */
                        /* {c_xx,c_yy,c_zz,c_xy,c_yz,c_zx} or */
                        /* {c_ee,c_nn,c_uu,c_en,c_nu,c_ue} */
    float  qv[6];       /* velocity variance/covariance (m^2/s^2) */
    double dtr[6];      /* receiver clock bias to time systems (s) */
    uint8_t type;       /* type (0:xyz-ecef,1:enu-baseline) */
    uint8_t stat;       /* solution status (SOLQ_???) */
    uint8_t ns;         /* number of valid satellites */
    float age;          /* age of differential (s) */
    float ratio;        /* AR ratio factor for valiation */
    float thres;        /* AR ratio threshold for valiation */


    sols *next;
};

//数据交换用结构体
struct sqlData
{
    int step;//结构体状态  0:未使用 1:已填充 2;已使用 3；待清空

    int type;//传入数据的类型
    gtime_t time;//传入数据时间戳

    //多线程基线解结果
    int sol_num;
    sols *sol;

    //网平差结果


    //姿态结果


    //解算状态更新



    sqlData *next;
    lock_t  lock;
};



/* lock/unlock sql data ------------------------------------------------------
* lock/unlock sql data
* args   : sqlData *sqldata    IO sql data transfer
* return : status (1:ok 0:error)
*-----------------------------------------------------------------------------*/
// void sqlDataLock  (sqlData *sqldata) {lock  (&sqldata->lock);}
// void sqlDataUnlock(sqlData *sqldata) {unlock(&sqldata->lock);}

//创建的锁在开始的时候要initlock       initlock(&svr->lock);


class ToSql
{
public:
    ToSql();

    QSqlDatabase db=*new QSqlDatabase;
    QSqlQuery query;

    //数据库连接参数
    QString dbconnType;
    QString hostName;
    int port;
    QString dbName;
    QString userName;
    QString password;


    //连接状态
    int conn_state;
    //错误信息
    QString erromsg;


    //数据源设置
    int dataInput();



    //读取数据库的参数
    int Init();

    // 连接打开
    int Open();

    //连接检查与重连
    int Check();

    //数据表创建
    int tableCreate();

    //创建数据表
    int create_xxxx(char* tablename);



    //输出到数据库
    void dateExport();

    //输出数据表
    int export_xxxx();



    //更新数据表
    int update_xxxx();




};

#endif // TOSQL_H
