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
              CallStack.addFunc
              CallStack.CallStack
              CallStack.clearStack
              CallStack.~CallStack
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
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include "pin.H"

#include <stack>
#include <sstream>
#include <ctime>

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/
const string EXCEPT_SEC = "/lib/";
const string LOG_NAME = "Log/axiba.log";
const unsigned long MAX_LINES = 5;

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
    CallStack(string outFileName);
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

//�򿪼�¼�ļ�, �����������Ϊ16���Ƹ�ʽ
CallStack::CallStack(string outFileName = LOG_NAME):_outFileName(outFileName)
{
    //open(const char*, std::ios_base::openmode)
    _outFile.open(_outFileName.c_str());
    //! �жϴ򿪳ɹ�
    _outFile << hex;

    _outTestFile.open(("test_"+_outFileName).c_str());
    //! �жϴ򿪳ɹ�
    _outTestFile << hex;

    _pLogStream= new ostringstream();
    (*_pLogStream) << hex;
    _logLines = 0;
    _logFileNum = 0;
}

//��պ������ù�ϵջ, �رռ�¼�ļ�
CallStack::~CallStack()
{
    clearStack();

    _outFile.close();
    _outTestFile.close();

    (*_pLogStream) << "/*The end*/";
    out2logFiles ();
    delete _pLogStream;
    _pLogStream = NULL;
}

//��պ������ù�ϵջ, ��ӡ�����Ϣ
void CallStack::clearStack()
{
    while(!_funcs.empty())
    {
        _outFile << "}//" << _funcs.top().funcName << std::endl;
        (*_pLogStream) << "}//" << _funcs.top().funcName << std::endl;

        _funcs.pop();
    }
}

//���һ���������ý�ջ, ͨ���Ƚ�������ǰ�����ĺ�����EBP���Ƴ�ջ, ��������Ϣ
void CallStack::addFunc(const char* funcName, ADDRINT funcAddr,
                        ADDRINT upperFuncBP)
{
    if(0 == upperFuncBP)
    {
        clearStack();
        //return;
    }
    else
    {
        //!clear the BP little than now BP
        while((!_funcs.empty()) && (0!=_funcs.top().upperFuncBP)
                && (_funcs.top().upperFuncBP<=upperFuncBP))
        {
            _outFile << "}//" << _funcs.top().funcName << std::endl;
            (*_pLogStream) << "}//" << _funcs.top().funcName << std::endl;

            _funcs.pop();
        }
    }
    //�ж��Ƿ񵽴�MAX_LINES��, �������̯���µ��ļ�
    if(MAX_LINES <= _logLines)
    {
        out2logFiles ();

        delete _pLogStream;
        _pLogStream = new ostringstream();
    }

    time_t nowTime = time(NULL);
    string nowTimeStr = ctime(&nowTime);
    //output
    _outFile << funcName << "{//" << funcAddr << ", " << nowTimeStr
             << std::endl;
    (*_pLogStream) << funcName << "{//" << funcAddr << ", " << nowTimeStr
                   << std::endl;
    ++ _logLines;

    _funcs.push(FuncItem(funcName, funcAddr, upperFuncBP));
}

void CallStack::out2logFiles()
{
    ostringstream osstream;
    osstream << _outFileName << "." << _logFileNum;

    ofstream splitFile;
    splitFile.open(osstream.str().c_str());
    //������ϴ���־�ļ�, ��--��β�ı�ʾ����δ��ֹ
    splitFile << (*_pLogStream).str();
    splitFile.close();

    ++ _logFileNum;
    _logLines = 0;
}

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
    cerr << "init pin tools failed.." << std::endl;
    return -1;
}

/*****************************************************************************
 �� �� ��  : main
 ��������  : main����; ��ʼ��pintool, ����RTN������
 �������  : int argc
             char **argv
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��5��16��
    ��    ��   : @zhi
    �޸�����   : �����ɺ���

*****************************************************************************/
int main(int argc, char **argv)
{
    //initial pin tool
    if (PIN_Init(argc, argv))
    {
        return usage();
    }
    //initial pin symbols table
    PIN_InitSymbols();

    //rtn level
    RTN_AddInstrumentFunction(rtn, 0);

    //call when finish
    PIN_AddFiniFunction(finish, 0);

    //start pin program
    PIN_StartProgram();

    return 0;
}
