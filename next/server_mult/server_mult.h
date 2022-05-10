#ifndef SERVER_MULT_H
#define SERVER_MULT_H

#include <QObject>
#include<stdio.h>
#include <string.h>
#include"rtklib.h"
#include <QString>
#include <QTimer>
#include <QDebug>

/* 宏定义 -----------------------------------------------------------------*/

#define MAXSTR      1024                /* max length of a stream */
#define MAXPATH     1024                /* max length of a filepath */

#define DEFAULTPORT 52001               // default monitor port number
#define MAXPORTOFF  9                   // max port number offset



/* system options table ------------------------------------------------------*/
#define SWTOPT  "0:off,1:on"
#define MODOPT  "0:single,1:dgps,2:kinematic,3:static,4:movingbase,5:fixed,6:ppp-kine,7:ppp-static,8:ppp-fixed"
#define FRQOPT  "1:l1,2:l1+2,3:l1+2+3,4:l1+2+3+4,5:l1+2+3+4+5"
#define TYPOPT  "0:forward,1:backward,2:combined"
#define IONOPT  "0:off,1:brdc,2:sbas,3:dual-freq,4:est-stec,5:ionex-tec,6:qzs-brdc"
#define TRPOPT  "0:off,1:saas,2:sbas,3:est-ztd,4:est-ztdgrad"
#define EPHOPT  "0:brdc,1:precise,2:brdc+sbas,3:brdc+ssrapc,4:brdc+ssrcom"
#define NAVOPT  "1:gps+2:sbas+4:glo+8:gal+16:qzs+32:bds+64:navic"
#define GAROPT  "0:off,1:on"
#define SOLOPT  "0:llh,1:xyz,2:enu,3:nmea"
#define TSYOPT  "0:gpst,1:utc,2:jst"
#define TFTOPT  "0:tow,1:hms"
#define DFTOPT  "0:deg,1:dms"
#define HGTOPT  "0:ellipsoidal,1:geodetic"
#define GEOOPT  "0:internal,1:egm96,2:egm08_2.5,3:egm08_1,4:gsi2000"
#define STAOPT  "0:all,1:single"
#define STSOPT  "0:off,1:state,2:residual"
#define ARMOPT  "0:off,1:continuous,2:instantaneous,3:fix-and-hold"
#define POSOPT  "0:llh,1:xyz,2:single,3:posfile,4:rinexhead,5:rtcm,6:raw"
#define TIDEOPT "0:off,1:on,2:otl"
#define PHWOPT  "0:off,1:on,2:precise"






#define TIMOPT  "0:gpst,1:utc,2:jst,3:tow"
#define CONOPT  "0:dms,1:deg,2:xyz,3:enu,4:pyl"
#define FLGOPT  "0:off,1:std+2:age/ratio/ns"
#define ISTOPT  "0:off,1:serial,2:file,3:tcpsvr,4:tcpcli,6:ntripcli,7:ftp,8:http"
#define OSTOPT  "0:off,1:serial,2:file,3:tcpsvr,4:tcpcli,5:ntripsvr,9:ntrcaster"
#define FMTOPT  "0:rtcm2,1:rtcm3,2:oem4,3:oem3,4:ubx,5:ss2,6:hemis,7:skytraq,8:javad,9:nvs,10:binex,11:rt17,12:spt,13:rnx,14:sp3,15:clk,16:sbas,17:nmea"
#define NMEOPT  "0:off,1:latlon,2:single"
#define SOLOPT  "0:llh,1:xyz,2:enu,3:nmea"
#define MSGOPT  "0:all,1:rover,2:base,3:corr"







/* 类 ------------------------ -------------------------------------------*/


class Navi_t
{
public:
    Navi_t();
    Navi_t(int argc,char**argv);

    ~Navi_t();
protected:

private:

    int stste; //线程状态 运行还是停止

    int step;//数据处理已经完成的流程

    int openMoni();

    int setDefaultSvr();

    int setDefaultOpt();

    int resetAll();

    int optCheck();

    void buff2sysopt();


public:

    //定时器  周期性输出用
    QTimer Timer;

    rtksvr_t *svr;
    stream_t *moni;

    prcopt_t prcopt;
    solopt_t solopt;
    filopt_t filopt;

    int antpostype[2];
    double antpos[2][3];


    double elmask;
    char snrmask[3][1024];
    char exsats[1024];
    double elmaskar;
    double elmaskhold;

    char optpath[MAXPATH];



    int SvrCycle,
        SvrBuffSize,
        SvrTimeout,
        SvrReconnect;

    int SvrNmeaCycle,
        SvrNmeaReq;
    double SvrNmeaPos[3];

    int SolBuffSize;
    int SvrLogSize;

    int NavSel;

    int SbasSel;

    int MonitorSetPort;
    int MonitorOpenPort;
    char proxyaddr[256]=""; /* http/ntrip/ftp proxy address */ //无效
    char localdir[1024]=""; /* local directory for ftp/http */ //无效
    uint32_t tick_master=0; /* time tick master for replay */  //无效


    //int StrSwitch[MAXSTRRTK];//数据流开关
    int StrType[MAXSTRRTK];//数据流类型
    int StrFormat[MAXSTRRTK];//数据格式
    char *StrPath[MAXSTRRTK];//数据流地址

    char *cmd[3],*cmd_periodic[3],*rcvopt[3];

    int toinact,           //inactive timeout (ms) (0: no timeout)
        ticonnect,         //interval to reconnect (ms)
        tirate,            //averaging time of data rate (ms)
        buffsize,          //receive/send buffer size (bytes);
        fswapmargin;       //file swap margin (s)


    char errmsg[20148];


    int Init(char *default_conf);

    int svrStart();
    int svrStop();
    int resetState();
    int clearAll();
    int setSql();


    int setTempOpt();

    int outstat();


};


/* 结构体 -----------------------------------------------------------------*/



struct svrio_t{           /* svr input/sol/log stream type */
    int type[8];           /* svr input(0-2)/sol(3-4)/log(5-7) stream type */
    int format[8];         /* svr input(0-2) / sol(3-4) / log(5-7) stream formats */
                           /*   ( STRFMT_??? / SOLF_??? / 0, 0 ,0  )   */

    char paths[8][MAXSTR]; /*svr input(0-2) / sol(3-4) / log(5-7) stream path*/
    int refpos;            /* base position for relative mode */
                           /* (0:pos in prcopt,  1:average of single pos, */
                           /*  2:read from file, 3:rinex header, 4:rtcm pos) */
    double rb[3];          /* base position */

};


/* 全局变量 ---------------------------------------------------------------*/

extern const prcopt_t svr_prcopt_defult;

extern const solopt_t svr_solopt_default;

extern const int strtype[8],strfmt[8],svrcycle,
                 timeout,reconnect,nmeacycle,
                 fswapmargin,buffsize,navmsgsel,
                 nmeareq;

extern const double nmeapos[3];

extern char strpath[8][MAXSTR],
            proxyaddr[MAXSTR],
            *paths[8],
            *cmds[3],
            *cmds_periodic[3],
            *ropts[3],
            errmsg[20148];



/* 函数 ------------------------------------------------------------------*/

//主循环函数
extern int svrMain();

//任务输入函数



//任务初始化与线程创建函数
extern int svrInit(rtksvr_t *svr,stream_t *moni);

extern int optInit(svrio_t *svrio);

extern int optLoad(svrio_t *svr);

extern int moniOpen(stream_t *moni,int moniport);

extern int svrStart(rtksvr_t *svr,svrio_t *svrio);

//输出函数







#endif // SERVER_MULT_H

