/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ƴ�־

 ******************************************************************************
  �� �� ��   : proccount.cpp
  �� �� ��   : ����
  ��    ��   : @zhi
  ��������   : 2012��5��16��
  ����޸�   :
  ��������   : ʹ��pintool���뺯����, ͨ����¼ESP��ջ����ɺ������ù�ϵ�ļ�¼
              BP ����һλ�ĵ�Ī���������ֵ
  �����б�   :
              backTrace
              finish
              funcPackage
              main
              trace
              usage
  �޸���ʷ   :
  1.��    ��   : 2012��5��16��
    ��    ��   : @zhi
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include <iostream>
#include <string>
#include <stack>

#include "pin.H"
#include "CallStack.h"
#include "CallStack.cpp"//g++Ĭ�ϵ����Ӳ���ȥ��ͬ��.cpp, ���ֶ����룻 �ֶ������ˣ����ֱ�������ͬʱ����-c��-o�� ����ͬʱָ�����Դ�ļ�
#include "getProcessFuncCallRelation.h"
/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/
const string EXCEPT_SEC = "/lib/";

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/
CallStack g_callStack;
bool g_backTraceFlg = true;

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/

/*****************************************************************************
 �� �� ��  : funcPackage
 ��������  : ��Ӻ������õ���������ջ�Ľӿ�
 �������  : const char* funcName
             ADDRINT funcAddr
             ADDRINT upperFuncBP
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��5��16��
    ��    ��   : @zhi
    �޸�����   : �����ɺ���

*****************************************************************************/
void funcPackage(const char* funcName, ADDRINT funcAddr, ADDRINT upperFuncBP)
{
    //std::cout << "name:" << funcName << ", addr:" << funcAddr
    //  << ", BP:" << funcBP << std::endl;
    g_callStack.addFunc(funcName, funcAddr, upperFuncBP);
}

/*****************************************************************************
 �� �� ��  : traceBack
 ��������  : �����Ѿ����õĺ���ջ,������ǰ�������õ���Ϣ
 �������  : const char* cfuncName
             ADDRINT funcAddr
             ADDRINT funcBP
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��5��16��
    ��    ��   : @zhi
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID traceBack( ADDRINT funcCurSP, ADDRINT funcUpperBP )
{
    if(g_backTraceFlg)
    {
#if 1
        string funcName;
        std::stack<FuncItem>tmpFuncs;
        ADDRINT tmpAddr;
        ADDRINT funcAddr;
        //�����к�����ջ,����Ĳ���ֱ���ǴӸú�����ʼ
        //tmpFuncs.push(FuncItem(funcName, funcAddr, funcBP-1));
        tmpAddr = *((ADDRINT *)funcCurSP);
        funcName = RTN_FindNameByAddress(tmpAddr);
        //��ȡ����������
        if("" == funcName)
        {
            funcName  = "[unknown]";
        }
        PIN_LockClient();
        funcAddr = RTN_Address(RTN_FindByAddress(tmpAddr));
        PIN_UnlockClient();

        while(0 != funcUpperBP)
        {
            tmpFuncs.push(FuncItem(funcName, funcAddr, funcUpperBP));

            tmpAddr = *((ADDRINT *)funcUpperBP + 1);
            funcName = RTN_FindNameByAddress(tmpAddr);
            //��ȡ����������
            if("" == funcName)
            {
                funcName  = "[unknown]";
            }
            PIN_LockClient();
            funcAddr = RTN_Address(RTN_FindByAddress(tmpAddr));
            PIN_UnlockClient();

            funcUpperBP = *(ADDRINT*) funcUpperBP;
        }

#endif

#if 1
        tmpAddr =0;
        while(!tmpFuncs.empty())
        {
            funcPackage(tmpFuncs.top().funcName.c_str(), tmpFuncs.top().
                        funcAddr, tmpAddr);

            tmpAddr = tmpFuncs.top().upperFuncBP;
            tmpFuncs.pop();
        }
#endif
        g_backTraceFlg = false;
    }
}

/*****************************************************************************
 �� �� ��  : rtn
 ��������  : Pin calls this function every time a new rtn is executed
 �������  : RTN rtn
             VOID * v
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��5��16��
    ��    ��   : @zhi
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID rtn(RTN rtn, VOID * v)
{
    //�ɵ�ϵͳ�Ķ�̬���ӿ�, ��IMGΪ/lib/*
    if(EXCEPT_SEC == IMG_Name(SEC_Img(RTN_Sec(rtn)))
            .substr(0, EXCEPT_SEC.size()))
    {
        return;
    }

    RTN_Open(rtn);

    if ( g_backTraceFlg )
    {
        //��һ�ν���,�����ѵ��õĺ���(���ӵ�����ģʽ)
        //, ����Ϊ��ִ�к����ĺ�����,������ʼ��ַ,����EBP(������ʼδѹջ)
        RTN_InsertCall(rtn, IPOINT_BEFORE,
                       (AFUNPTR)traceBack,
                       //IARG_PTR, RTN_Name(rtn).c_str(),
                       //IARG_ADDRINT, RTN_Address(rtn),
                       IARG_REG_VALUE, REG_STACK_PTR,
                       IARG_REG_VALUE, REG_EBP, IARG_END);
    }
    RTN_InsertCall(rtn, IPOINT_BEFORE,
                   (AFUNPTR)funcPackage,
                   IARG_PTR, RTN_Name(rtn).c_str(),
                   IARG_ADDRINT, RTN_Address(rtn),
                   IARG_REG_VALUE, REG_EBP,
                   //IARG_REG_VALUE, REG_STACK_PTR, IARG_END);
                   IARG_END);
    RTN_Close(rtn);
}

/*****************************************************************************
 �� �� ��  : finish
 ��������  : pintool������ֹ֮ǰ�����ú���; �˴���պ�������ջ
 �������  : INT32 code
             VOID * v
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��5��16��
    ��    ��   : @zhi
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID finish(INT32 code, VOID * v)
{
    g_callStack.~CallStack();
}

/*****************************************************************************
 �� �� ��  : usage
 ��������  : ��ӡ������Ϣ
 �������  : ��
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��5��16��
    ��    ��   : @zhi
    �޸�����   : �����ɺ���

*****************************************************************************/
INT32 usage()
{
    std::cerr << "init pin tools failed.." << std::endl;
    return -1;
}
