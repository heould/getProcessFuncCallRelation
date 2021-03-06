/******************************************************************************

                  版权所有 (C), 2001-2011, 志长有限公司

 ******************************************************************************
  文 件 名   : getProcessFuncCallRelation.h
  版 本 号   : 初稿
  作    者   : @zhi
  生成日期   : 2012年5月21日
  最近修改   :
  功能描述   : getProcessFuncCallRelation.cpp 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2012年5月21日
    作    者   : @zhi
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "pin.H"
/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

#ifndef __GETPROCESSFUNCCALLRELATION_H__
#define __GETPROCESSFUNCCALLRELATION_H__

/*****************************************************************************
 函 数 名  : finish
 功能描述  : pintool进程终止之前所调用函数; 此处清空函数调用栈
 输入参数  : INT32 code
             VOID * v
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年5月16日
    作    者   : @zhi
    修改内容   : 新生成函数

*****************************************************************************/
VOID finish(INT32 code, VOID * v);

/*****************************************************************************
 函 数 名  : funcPackage
 功能描述  : 添加函数调用到函数调用栈的接口
 输入参数  : const char* funcName
             ADDRINT funcAddr
             ADDRINT upperFuncBP
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年5月16日
    作    者   : @zhi
    修改内容   : 新生成函数

*****************************************************************************/
VOID funcPackage(const char* funcName, ADDRINT funcAddr, ADDRINT upperFuncBP);

/*****************************************************************************
 函 数 名  : rtn
 功能描述  : Pin calls this function every time a new rtn is executed
 输入参数  : RTN rtn
             VOID * v
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年5月16日
    作    者   : @zhi
    修改内容   : 新生成函数

*****************************************************************************/
VOID rtn(RTN rtn, VOID * v);

/*****************************************************************************
 函 数 名  : traceBack
 功能描述  : 回溯已经调用的函数栈,解析当前函数调用的信息
 输入参数  : const char* cfuncName
             ADDRINT funcAddr
             ADDRINT funcBP
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年5月16日
    作    者   : @zhi
    修改内容   : 新生成函数

*****************************************************************************/
VOID traceBack( ADDRINT funcCurSP, ADDRINT funcUpperBP );

/*****************************************************************************
 函 数 名  : usage
 功能描述  : 打印错误信息
 输入参数  : 无
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年5月16日
    作    者   : @zhi
    修改内容   : 新生成函数

*****************************************************************************/
INT32 usage();

#endif /* __GETPROCESSFUNCCALLRELATION_H__ */
