#include <fstream>
#include <iostream>
#include <string>
#include "pin.H"

#include <stack>
#include <sstream>

#ifndef __CALLSTACK_H__
#define __CALLSTACK_H__


const string LOG_NAME = "Log/axiba.log";

/*------------------------------------------------------------*
 *������¼��--------------------------------------------------*
 *������Ա: ������, ������ʼ��ַ, ���ε��ñ������ĺ�����EBP---*
 *------------------------------------------------------------*/
class FuncItem
{
public:
    string funcName;
    ADDRINT funcAddr;
    ADDRINT upperFuncBP;

    FuncItem(string name, ADDRINT addr, ADDRINT upperBP)
        :funcName(name), funcAddr(addr), upperFuncBP(upperBP) {}
};
/*
 *��¼�������ù�ϵ��ջ, ������Ӧ��־�����
 *ͨ���Ƚϵ�ǰ����������EBP, �ж�ǰ�溯����ֹ���
*/
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
