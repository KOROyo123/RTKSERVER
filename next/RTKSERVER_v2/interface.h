/*
*   主要功能
*       1、提供一个或多个读取外部配置的接口（形式可以是file/TCP/COM）
*       2、任务创建线程：接收外部接口配置并解析、放入任务队列
*       3、任务执行线程：读取任务队列中的任务，依次执行
*
*/



#ifndef INTERFACE_H
#define INTERFACE_H


#include "sys_ctrl.h"



class TaskQueue
{
public:
    TaskQueue();





    int A_Navi_Task(char *conf);
};















#endif // INTERFACE_H
