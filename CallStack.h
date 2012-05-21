/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ־�����޹�˾

 ******************************************************************************
  �� �� ��   : CallStack.h
  �� �� ��   : ����
  ��    ��   : @zhi
  ��������   : 2012��4��21��
  ����޸�   :
  ��������   : CallStack���ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��4��21��
    ��    ��   : @zhi
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include <fstream>
#include <iostream>
#include <string>
#include "pin.H"

#include <stack>
#include <sstream>

/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/
#ifndef __CALLSTACK_H__
#define __CALLSTACK_H__

using namespace std;
/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/
const string LOG_NAME = "/home/e/getProcessFuncCallRelation/Log/axiba.log";

/*****************************************************************************
 �� ��  : FuncItem
 ��������  : ������¼��
 public����  : FuncItem

 �޸���ʷ      :
  1.��    ��   : 2012��4��21��
    ��    ��   : @zhi
    �޸�����   : ��������

*****************************************************************************/
class FuncItem
{
public:
    string funcName;
    ADDRINT funcAddr;
    ADDRINT upperFuncBP;

    FuncItem(string name, ADDRINT addr, ADDRINT upperBP)
        :funcName(name), funcAddr(addr), upperFuncBP(upperBP) {}
};

/*****************************************************************************
 �� ��  : CallStack
 ��������  : ��¼�������ù�ϵ��ջ��
 public����  :  CallStack
                ~CallStack
                addFunc
                clearStack
                getOutFile
                getTestFile

 �޸���ʷ      :
  1.��    ��   : 2012��4��21��
    ��    ��   : @zhi
    �޸�����   : ��������

*****************************************************************************/
class CallStack
{
private:
    std::stack<FuncItem>_funcs;
    string _outFileName;
    ofstream _outFile;
    ofstream  _outTestFile;

    unsigned long _logLines;
    unsigned long _logFileNum;
    ostringstream* _pLogStream;

    void out2logFiles();

public:
    //Ĭ�ϲ���д��.h
    CallStack(string outFileName=LOG_NAME);
    ~CallStack();
    //���һ���������ý�ջ
    void addFunc(const char* funcName, ADDRINT funcAddr, ADDRINT upperFuncBP);
    void clearStack();

    //������Ա����������ofstream����
    //��׼��־����ļ�
    ofstream& getOutFile()
    {
        return _outFile;
    }
    //������ר��Ϊ���Ե��ļ�
    ofstream& getTestFile()
    {
        return _outTestFile;
    }
};

#endif /* __CALLSTACK_H__ */
