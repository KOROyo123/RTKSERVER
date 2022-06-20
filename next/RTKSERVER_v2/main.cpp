/*
*
* interface 用于接收指令 并调用内部的函数和方法 实现交互和控制系统的功能
* sys_ctrl  封装用于控制系统各种操作和设置的函数
* svr_core  程序的核心代码，用于获取各个RTK线程的结果，并进行数据的二次处理和输出
* navi      实时rtk解算，创建一个rtksvrThread并进行实时解算
* sql_trans 数据库输出模块，用于向数据库写入数据
*
*/



#include <QCoreApplication>
#include "sys_ctrl.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    SysCtrl SYS;

    //初始化，读取基本的系统设置，并设置对应量   所有系统有关的参数在这里设置
    SYS.Init(argc,argv);


    //新建任务队列


    //开启任务队列创建线程
    SYS.Start();


    //开启任务队列处理线程


//功能测试------------------------------------------------------------------

//    SYS.createNaviThread("D:/RTKSERVER/conf/server_mult/test1.conf");

//    SYS.createNaviThread("D:/RTKSERVER/conf/server_mult/test1.conf");

//    SYS.createNaviThread("D:/RTKSERVER/conf/server_mult/test1.conf");

//    SYS.createNaviThread("D:/RTKSERVER/conf/server_mult/test1.conf");





    qDebug()<<"emmmmm问题就剩亿点点了";



    qDebug()<<"emmmmm问题就剩亿点点点了";

//------------------------------------------------------------------



    //进入循环函数

        //周期性输出当前系统的状态



    //程序结束退出循环


    //析构函数  关闭数据流和线程



    //程序结束


    return a.exec();
}
