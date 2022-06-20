#include "navi.h"

Navi::Navi()
{


    //空间分配
    navisvr=new navisvr_t;


    svr=new rtksvr_t;
    moni=new stream_t;

    navisvr->svr=svr;

    resetState();

}

Navi::Navi(rtksvr_t *asvr,stream_t *amoni)
{
    svr=asvr;
    moni=amoni;

    resetState();
}

Navi::Navi(int argc, char **argv)
{
    resetState();
}

Navi::~Navi()
{
    delete StrPath[MAXSTRRTK];
}

//打开监视数据流，MonitorSetPort为设置流，MonitorOpenPort为实际打开流
int Navi::openMoni()
{
    QString s;
    int i;
    char path[64];

    if (MonitorSetPort<=0) return 0;

    //trace(3,"OpenMoniPort: port=%d\n",MonitorPort);

    for (i=0;i<=MAXPORTOFF;i++) {

        sprintf(path,":%d",MonitorSetPort+i);

        if (stropen(moni,STR_TCPSVR,STR_MODE_RW,path)) {
            strsettimeout(moni,SvrTimeout,SvrReconnect);
            //if (i>0) setWindowTitle(QString(tr("%1 ver.%2 (%3)")).arg(PRGNAME).arg(VER_RTKLIB).arg(i+1));
            MonitorOpenPort=MonitorSetPort+i;
            return 1;
        }
    }

}

int Navi::setDefaultSvr()
{

}

int Navi::setDefaultOpt()
{

//    pcvs_t pcvr,pcvs;
//    pcv_t *pcv;

//    memset(&pcvr,0,sizeof(pcvs_t));
//    memset(&pcvs,0,sizeof(pcvs_t));


    //0 int 1 double 2 char 3宏定义转数字
    opt_t loadsysopts[]={
        {"pos1-posmode",    3,  (void *)&prcopt.mode,       MODOPT },
        {"pos1-frequency",  3,  (void *)&prcopt.nf,         FRQOPT },
        {"pos1-soltype",    3,  (void *)&prcopt.soltype,    TYPOPT },
        {"pos1-elmask",     1,  (void *)&elmask,            "deg"  },
        {"pos1-snrmask_r",  3,  (void *)&prcopt.snrmask.ena[0],SWTOPT},
        {"pos1-snrmask_b",  3,  (void *)&prcopt.snrmask.ena[1],SWTOPT},
        {"pos1-snrmask_L1", 2,  (void *)snrmask[0],         ""     },
        {"pos1-snrmask_L2", 2,  (void *)snrmask[1],         ""     },
        {"pos1-snrmask_L5", 2,  (void *)snrmask[2],         ""     },
        {"pos1-dynamics",   3,  (void *)&prcopt.dynamics,   SWTOPT },
        {"pos1-tidecorr",   3,  (void *)&prcopt.tidecorr,   TIDEOPT},
        {"pos1-ionoopt",    3,  (void *)&prcopt.ionoopt,    IONOPT },
        {"pos1-tropopt",    3,  (void *)&prcopt.tropopt,    TRPOPT },
        {"pos1-sateph",     3,  (void *)&prcopt.sateph,     EPHOPT },
        {"pos1-posopt1",    3,  (void *)&prcopt.posopt[0],  SWTOPT },
        {"pos1-posopt2",    3,  (void *)&prcopt.posopt[1],  SWTOPT },
        {"pos1-posopt3",    3,  (void *)&prcopt.posopt[2],  PHWOPT },
        {"pos1-posopt4",    3,  (void *)&prcopt.posopt[3],  SWTOPT },
        {"pos1-posopt5",    3,  (void *)&prcopt.posopt[4],  SWTOPT },
        {"pos1-posopt6",    3,  (void *)&prcopt.posopt[5],  SWTOPT },
        {"pos1-exclsats",   2,  (void *)exsats,             "prn ..."},
        {"pos1-navsys",     0,  (void *)&prcopt.navsys,     NAVOPT },

        {"pos2-armode",     3,  (void *)&prcopt.modear,     ARMOPT },
        {"pos2-gloarmode",  3,  (void *)&prcopt.glomodear,  GAROPT },
        {"pos2-bdsarmode",  3,  (void *)&prcopt.bdsmodear,  SWTOPT },
        {"pos2-arthres",    1,  (void *)&prcopt.thresar[0], ""     },
        {"pos2-arthres1",   1,  (void *)&prcopt.thresar[1], ""     },
        {"pos2-arthres2",   1,  (void *)&prcopt.thresar[2], ""     },
        {"pos2-arthres3",   1,  (void *)&prcopt.thresar[3], ""     },
        {"pos2-arthres4",   1,  (void *)&prcopt.thresar[4], ""     },
        {"pos2-arlockcnt",  0,  (void *)&prcopt.minlock,    ""     },
        {"pos2-arelmask",   1,  (void *)&elmaskar,          "deg"  },
        {"pos2-arminfix",   0,  (void *)&prcopt.minfix,     ""     },
        {"pos2-armaxiter",  0,  (void *)&prcopt.armaxiter,  ""     },
        {"pos2-elmaskhold", 1,  (void *)&elmaskhold,        "deg"  },
        {"pos2-aroutcnt",   0,  (void *)&prcopt.maxout,     ""     },
        {"pos2-maxage",     1,  (void *)&prcopt.maxtdiff,   "s"    },
        {"pos2-syncsol",    3,  (void *)&prcopt.syncsol,    SWTOPT },
        {"pos2-slipthres",  1,  (void *)&prcopt.thresslip,  "m"    },
        {"pos2-rejionno",   1,  (void *)&prcopt.maxinno,    "m"    },
        {"pos2-rejgdop",    1,  (void *)&prcopt.maxgdop,    ""     },
        {"pos2-niter",      0,  (void *)&prcopt.niter,      ""     },
        {"pos2-baselen",    1,  (void *)&prcopt.baseline[0],"m"    },
        {"pos2-basesig",    1,  (void *)&prcopt.baseline[1],"m"    },

        {"out-solformat",   3,  (void *)&solopt.posf,       SOLOPT },
        {"out-outhead",     3,  (void *)&solopt.outhead,    SWTOPT },
        {"out-outopt",      3,  (void *)&solopt.outopt,     SWTOPT },
        {"out-outvel",      3,  (void *)&solopt.outvel,     SWTOPT },
        {"out-timesys",     3,  (void *)&solopt.times,      TSYOPT },
        {"out-timeform",    3,  (void *)&solopt.timef,      TFTOPT },
        {"out-timendec",    0,  (void *)&solopt.timeu,      ""     },
        {"out-degform",     3,  (void *)&solopt.degf,       DFTOPT },
        {"out-fieldsep",    2,  (void *) solopt.sep,        ""     },
        {"out-outsingle",   3,  (void *)&prcopt.outsingle,  SWTOPT },
        {"out-maxsolstd",   1,  (void *)&solopt.maxsolstd,  "m"    },
        {"out-height",      3,  (void *)&solopt.height,     HGTOPT },
        {"out-geoid",       3,  (void *)&solopt.geoid,      GEOOPT },
        {"out-solstatic",   3,  (void *)&solopt.solstatic,  STAOPT },
        {"out-nmeaintv1",   1,  (void *)&solopt.nmeaintv[0],"s"    },
        {"out-nmeaintv2",   1,  (void *)&solopt.nmeaintv[1],"s"    },
        {"out-outstat",     3,  (void *)&solopt.sstat,      STSOPT },

        {"stats-eratio1",   1,  (void *)&prcopt.eratio[0],  ""     },
        {"stats-eratio2",   1,  (void *)&prcopt.eratio[1],  ""     },
        {"stats-errphase",  1,  (void *)&prcopt.err[1],     "m"    },
        {"stats-errphaseel",1,  (void *)&prcopt.err[2],     "m"    },
        {"stats-errphasebl",1,  (void *)&prcopt.err[3],     "m/10km"},
        {"stats-errdoppler",1,  (void *)&prcopt.err[4],     "Hz"   },
        {"stats-stdbias",   1,  (void *)&prcopt.std[0],     "m"    },
        {"stats-stdiono",   1,  (void *)&prcopt.std[1],     "m"    },
        {"stats-stdtrop",   1,  (void *)&prcopt.std[2],     "m"    },
        {"stats-prnaccelh", 1,  (void *)&prcopt.prn[3],     "m/s^2"},
        {"stats-prnaccelv", 1,  (void *)&prcopt.prn[4],     "m/s^2"},
        {"stats-prnbias",   1,  (void *)&prcopt.prn[0],     "m"    },
        {"stats-prniono",   1,  (void *)&prcopt.prn[1],     "m"    },
        {"stats-prntrop",   1,  (void *)&prcopt.prn[2],     "m"    },
        {"stats-prnpos",    1,  (void *)&prcopt.prn[5],     "m"    },
        {"stats-clkstab",   1,  (void *)&prcopt.sclkstab,   "s/s"  },

        {"ant1-postype",    3,  (void *)&antpostype[0],     POSOPT },
        {"ant1-pos1",       1,  (void *)&antpos[0][0],      "deg|m"},
        {"ant1-pos2",       1,  (void *)&antpos[0][1],      "deg|m"},
        {"ant1-pos3",       1,  (void *)&antpos[0][2],      "m|m"  },
        {"ant1-anttype",    2,  (void *)prcopt.anttype[0],  ""     },
        {"ant1-antdele",    1,  (void *)&prcopt.antdel[0][0],"m"   },
        {"ant1-antdeln",    1,  (void *)&prcopt.antdel[0][1],"m"   },
        {"ant1-antdelu",    1,  (void *)&prcopt.antdel[0][2],"m"   },

        {"ant2-postype",    3,  (void *)&antpostype[1],     POSOPT },
        {"ant2-pos1",       1,  (void *)&antpos[1][0],      "deg|m"},
        {"ant2-pos2",       1,  (void *)&antpos[1][1],      "deg|m"},
        {"ant2-pos3",       1,  (void *)&antpos[1][2],      "m|m"  },
        {"ant2-anttype",    2,  (void *)prcopt.anttype[1],  ""     },
        {"ant2-antdele",    1,  (void *)&prcopt.antdel[1][0],"m"   },
        {"ant2-antdeln",    1,  (void *)&prcopt.antdel[1][1],"m"   },
        {"ant2-antdelu",    1,  (void *)&prcopt.antdel[1][2],"m"   },
        {"ant2-maxaveep",   0,  (void *)&prcopt.maxaveep    ,""    },
        {"ant2-initrst",    3,  (void *)&prcopt.initrst,    SWTOPT },

        {"misc-timeinterp", 3,  (void *)&prcopt.intpref,    SWTOPT },
        {"misc-sbasatsel",  0,  (void *)&prcopt.sbassatsel, "0:all"},
        {"misc-rnxopt1",    2,  (void *)prcopt.rnxopt[0],   ""     },
        {"misc-rnxopt2",    2,  (void *)prcopt.rnxopt[1],   ""     },
        {"misc-pppopt",     2,  (void *)prcopt.pppopt,      ""     },

        {"file-satantfile", 2,  (void *)&filopt.satantp,    ""     },
        {"file-rcvantfile", 2,  (void *)&filopt.rcvantp,    ""     },
        {"file-staposfile", 2,  (void *)&filopt.stapos,     ""     },
        {"file-geoidfile",  2,  (void *)&filopt.geoid,      ""     },
        {"file-ionofile",   2,  (void *)&filopt.iono,       ""     },
        {"file-dcbfile",    2,  (void *)&filopt.dcb,        ""     },
        {"file-eopfile",    2,  (void *)&filopt.eop,        ""     },
        {"file-blqfile",    2,  (void *)&filopt.blq,        ""     },
        {"file-tempdir",    2,  (void *)&filopt.tempdir,    ""     },
        {"file-geexefile",  2,  (void *)&filopt.geexe,      ""     },
        {"file-solstatfile",2,  (void *)&filopt.solstat,    ""     },
        {"file-tracefile",  2,  (void *)&filopt.trace,      ""     },

        {"",0,NULL,""} /* terminator */
    };

    opt_t loadrcvopt[]={
        {"inpstr1-type",    3,  (void *)&StrType[0],         ISTOPT },
        {"inpstr2-type",    3,  (void *)&StrType[1],         ISTOPT },
        {"inpstr3-type",    3,  (void *)&StrType[2],         ISTOPT },
        {"inpstr1-path",    2,  (void *)StrPath [0],         ""     },
        {"inpstr2-path",    2,  (void *)StrPath [1],         ""     },
        {"inpstr3-path",    2,  (void *)StrPath [2],         ""     },
        {"inpstr1-format",  3,  (void *)&StrFormat [0],         FMTOPT },
        {"inpstr2-format",  3,  (void *)&StrFormat [1],         FMTOPT },
        {"inpstr3-format",  3,  (void *)&StrFormat [2],         FMTOPT },
        {"inpstr2-nmeareq", 3,  (void *)&SvrNmeaReq,            NMEOPT },
        {"inpstr2-nmealat", 1,  (void *)&SvrNmeaPos[0],         "deg"  },
        {"inpstr2-nmealon", 1,  (void *)&SvrNmeaPos[1],         "deg"  },
        {"outstr1-type",    3,  (void *)&StrType[3],         OSTOPT },
        {"outstr2-type",    3,  (void *)&StrType[4],         OSTOPT },
        {"outstr1-path",    2,  (void *)StrPath [3],         ""     },
        {"outstr2-path",    2,  (void *)StrPath [4],         ""     },
        {"outstr1-format",  3,  (void *)&StrFormat [3],         SOLOPT },
        {"outstr2-format",  3,  (void *)&StrFormat [4],         SOLOPT },
        {"logstr1-type",    3,  (void *)&StrFormat[5],         OSTOPT },
        {"logstr2-type",    3,  (void *)&StrType[6],         OSTOPT },
        {"logstr3-type",    3,  (void *)&StrType[7],         OSTOPT },
        {"logstr1-path",    2,  (void *)StrPath [5],         ""     },
        {"logstr2-path",    2,  (void *)StrPath [6],         ""     },
        {"logstr3-path",    2,  (void *)StrPath [7],         ""     },

        {"misc-svrcycle",   0,  (void *)&SvrCycle,           "ms"   },
        {"misc-timeout",    0,  (void *)&SvrTimeout,            "ms"   },
        {"misc-reconnect",  0,  (void *)&SvrReconnect,          "ms"   },
        {"misc-nmeacycle",  0,  (void *)&SvrNmeaCycle,          "ms"   },
        {"misc-buffsize",   0,  (void *)&SvrBuffSize,           "bytes"},
        {"misc-navmsgsel",  3,  (void *)&NavSel,          MSGOPT },
        {"misc-proxyaddr",  2,  (void *)proxyaddr,           ""     },
        {"misc-fswapmargin",0,  (void *)&fswapmargin,        "s"    },

        {"",0,NULL,""}
    };

    loadopts(optpath,loadsysopts);
    loadopts(optpath,loadrcvopt);

    buff2sysopt();


    //设置需要剔除的卫星
//    for (i=0;i<MAXSAT;i++) {
//        prcopt.exsats[i]=0;
//    }

    //读取PCV文件
//    readpcv(filopt.rcvantp,&pcvs);


    //设置星历
//    prcopt.sateph=EPHOPT_BRDC;


//    prcopt.baseline[0]=0.0;
//    prcopt.baseline[1]=0.0;


    return 0;

}


int Navi::resetAll()
{
    //初始化设置
    resetState();

    rtksvrinit(svr);
    strinit(moni);

    prcopt=prcopt_default;
    solopt=solopt_default;
    filopt.satantp[0]='\0';
    filopt.rcvantp[0]='\0';
    filopt.stapos [0]='\0';
    filopt.geoid  [0]='\0';
    filopt.dcb    [0]='\0';
    filopt.blq    [0]='\0';
    filopt.solstat[0]='\0';
    filopt.trace  [0]='\0';

    for (int i=0;i<2;i++) {
        antpostype[i]=0;
    }
    elmask=15.0;
    elmaskar=0.0;
    elmaskhold=0.0;
    for (int i=0;i<2;i++) {
        for (int j=0;j<3;j++) {
            antpos[i][j]=0.0;
        }
    }
    exsats[0] ='\0';

    SvrCycle=1;
    SvrBuffSize=32768;
    SvrTimeout=10000;
    SvrReconnect=10000;

    SvrNmeaCycle=5000;
    SvrNmeaReq=0;
    SvrNmeaPos[2]=SvrNmeaPos[1]=SvrNmeaPos[0]=0.0;

    SolBuffSize=1000;
    SvrLogSize=100;

    NavSel=3;

    SbasSel=0;

    MonitorSetPort=0;
    MonitorOpenPort=0;

    //初始化数据流
    for (int i=0;i<MAXSTRRTK;i++) {
        //StrSwitch[i]=
        StrType[i]=StrFormat[i]=0;
    }
    for(int i=0;i<MAXSTRRTK;i++){
        StrPath[i]= new char[1024];
        StrPath[i][0]='\0';
    }


    //初始化CMD
    for (int i=0;i<3;i++) {
        cmd[i]=new char[1024];
        cmd_periodic[i]=new char[1024];
        rcvopt[i]=new char[1024];
        cmd[i][0]=cmd_periodic[i][0]=rcvopt[i][0]='\0';
    }

    //TCP流设置
    toinact    =1000; /* >=1s */
    ticonnect  =1000; /* >=1s */
    tirate     =100; /* >=0.1s */
    buffsize   =4096; /* >=4096byte */
    fswapmargin=30;

    errmsg[0]='\0';
}


int Navi::optCheck()
{
    toinact    =0<toinact&&toinact<1000?1000:toinact; /* >=1s */
    ticonnect  =ticonnect<1000?1000:ticonnect; /* >=1s */
    tirate     =tirate<100 ?100 :tirate; /* >=0.1s */
    buffsize   =buffsize<4096?4096:buffsize; /* >=4096byte */
    fswapmargin=fswapmargin<0?0:fswapmargin;
}

/* system options buffer to options ------------------------------------------*/
void Navi::buff2sysopt()
{
    double pos[3],*rr;
    char buff[1024],*p,*id;
    int i,j,sat,*ps;

    prcopt.elmin     =elmask    *D2R;
    prcopt.elmaskar  =elmaskar  *D2R;
    prcopt.elmaskhold=elmaskhold*D2R;

    for (i=0;i<2;i++) {
        ps=i==0?&prcopt.rovpos:&prcopt.refpos;
        rr=i==0?prcopt.ru:prcopt.rb;

        if (antpostype[i]==0) { /* lat/lon/hgt */
            *ps=0;
            pos[0]=antpos[i][0]*D2R;
            pos[1]=antpos[i][1]*D2R;
            pos[2]=antpos[i][2];
            pos2ecef(pos,rr);
        }
        else if (antpostype[i]==1) { /* xyz-ecef */
            *ps=0;
            rr[0]=antpos[i][0];
            rr[1]=antpos[i][1];
            rr[2]=antpos[i][2];
        }
        else *ps=antpostype[i]-1;
    }
    /* excluded satellites */
    for (i=0;i<MAXSAT;i++) prcopt.exsats[i]=0;
    if (exsats[0]!='\0') {
        strcpy(buff,exsats);
        for (p=strtok(buff," ");p;p=strtok(NULL," ")) {
            if (*p=='+') id=p+1; else id=p;
            if (!(sat=satid2no(id))) continue;
            prcopt.exsats[sat-1]=*p=='+'?2:1;
        }
    }
    /* snrmask */
    for (i=0;i<NFREQ;i++) {
        for (j=0;j<9;j++) prcopt.snrmask.mask[i][j]=0.0;
        strcpy(buff,snrmask[i]);
        for (p=strtok(buff,","),j=0;p&&j<9;p=strtok(NULL,",")) {
            prcopt.snrmask.mask[i][j++]=atof(p);
        }
    }
    /* number of frequency (4:L1+L5) */
    if (prcopt.nf==4) {
        prcopt.nf=3;
        prcopt.freqopt=1;
    }

}


/* 类初始化 ---------------------------------------------------
* 将默认设置读取进类内，
* args   : char  *default_conf       I   默认设置的路径
*
* return : none
*-----------------------------------------------------------------------------*/
int Navi::Init( char *default_conf)
{
    strncpy(optpath,default_conf,MAXPATH);

    setDefaultOpt();

    optCheck();

    return 0;
}

int Navi::svrStart()
{

    /* set ftp/http directory and proxy */
    strsetdir(filopt.tempdir);
    strsetproxy(proxyaddr);

    /* execute start command */

    //打开moni端口
    openMoni();

    /* start rtk server */
    if (!navistart(navisvr,SvrCycle,SvrBuffSize,StrType,StrPath,StrFormat,NavSel,
                     cmd,cmd_periodic,rcvopt,SvrNmeaCycle,SvrNmeaReq,SvrNmeaPos,&prcopt,
                     &solopt,moni,errmsg)) {
        //trace(2,"rtk server start error (%s)\n",errmsg);
        //vt_printf(vt,"rtk server start error (%s)\n",errmsg);
        return 0;
    }
    return 1;

}

int Navi::svrStop()
{
    char **cmds = nullptr;

    rtksvrstop(svr,cmds);
}

//构建类和重置类时使用，重置状态变量的值
int Navi::resetState()
{
    stste=0;

    //svr初始化
    rtksvrinit(svr);
    strinit(moni);

    //sysopt初始化
    prcopt=prcopt_default;
    solopt=solopt_default;
    filopt={};

    optpath[0]='\0';

    //初始化设置
    SvrCycle=1;
    SvrBuffSize=32768;
    SvrTimeout=10000;
    SvrReconnect=10000;

    SvrNmeaCycle=5000;
    SvrNmeaReq=0;
    SvrNmeaPos[2]=SvrNmeaPos[1]=SvrNmeaPos[0]=0.0;

    SolBuffSize=1000;
    SvrLogSize=100;

    NavSel=3;

    SbasSel=0;

    MonitorSetPort=12200;
    MonitorOpenPort=0;

    //初始化数据流
    for (int i=0;i<MAXSTRRTK;i++) {
        //StrSwitch[i]=
        StrType[i]=StrFormat[i]=0;
    }
    for(int i=0;i<MAXSTRRTK;i++){
        StrPath[i]= new char[1024];
        StrPath[i][0]='\0';
    }


    //初始化CMD
    for (int i=0;i<3;i++) {
        cmd[i]=new char[1024];
        cmd_periodic[i]=new char[1024];
        rcvopt[i]=new char[1024];
        cmd[i][0]=cmd_periodic[i][0]=rcvopt[i][0]='\0';
    }

    //TCP流设置
    toinact    =1000; /* >=1s */
    ticonnect  =1000; /* >=1s */
    tirate     =100; /* >=0.1s */
    buffsize   =4096; /* >=4096byte */
    fswapmargin=30;

    errmsg[0]='\0';

    return 0;
}


int Navi::outstat()
{
    rtksvrlock(svr);
    for(int i=0;i<svr->nsol;i++){

        sol_t *sol=svr->solbuf+i;
        qDebug()<<svr->solbuf->time.time<<sol->stat;
    }

    rtksvrunlock(svr);

    return 0;
}


