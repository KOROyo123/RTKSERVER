#include "server_mult.h"

Navi_t::Navi_t()
{
    resetState();
}

Navi_t::Navi_t(int argc, char **argv)
{
    resetState();
}

/* 类初始化 ---------------------------------------------------
* 将默认设置读取进类内，
* args   : char  *default_conf       I   默认设置的路径
*
* return : none
*-----------------------------------------------------------------------------*/
int Navi_t::Init( char *default_conf)
{

}



//构建类和重置类时使用，重置状态变量的值
int Navi_t::resetState()
{
    stste=0;
}

int Navi_t::setSql()
{

}

