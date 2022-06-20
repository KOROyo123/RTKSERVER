/*
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
*/

#include <QCoreApplication>
#include"server_mult.h"

#define SVR_DEFAULT_SETTING_PATH  "D:/RTKSERVER/conf/server_mult/default.conf"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    char conf1[]={"D:/RTKSERVER/conf/server_mult/test1.conf"};
    char conf2[]={"D:/RTKSERVER/conf/server_mult/test2.conf"};

    //创建类
    Navi Navi1,Navi2,Navi3,Navi4;


    //读取conf
    Navi1.Init(conf1);
//    Navi2.Init(conf1);
//    Navi3.Init(conf2);
//    Navi4.Init(conf2);

    //创建rtksvrthread
    Navi1.svrStart();
//    Navi2.svrStart();
//    Navi3.svrStart();
//    Navi4.svrStart();

    qDebug()<<"emmmm";

//    while(1){

//        Navi.outstat();

//        sleepms(100);
//    }




    return a.exec();
}
