/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ־�����޹�˾

 ******************************************************************************
  �� �� ��   : CallStack.cpp
  �� �� ��   : ����
  ��    ��   : @zhi
  ��������   : 2012��5��21��
  ����޸�   :
  ��������   : CallStack���������ļ�
  �����б�   :
              CallStack.addFunc
              CallStack.CallStack
              CallStack.clearStack
              CallStack.out2logFiles
              CallStack.~CallStack
  �޸���ʷ   :
  1.��    ��   : 2012��5��21��
    ��    ��   : @zhi
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include <ctime>
#include "CallStack.h"

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
 * ��������                                     *
 *----------------------------------------------*/
const unsigned long MAX_LINES = 5;

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/

/*****************************************************************************
 �� �� ��  : CallStack.CallStack
 ��������  : �򿪼�¼�ļ�, �����������Ϊ16����
 �������  : string outFileName
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��5��21��
    ��    ��   : @zhi
    �޸�����   : �����ɺ���

*****************************************************************************/
CallStack::CallStack(string outFileName):_outFileName(outFileName)
{
    _outFile.open(_outFileName.c_str());
    //�жϴ�ʧ��
    if ( _outFile.fail())
    {
        std::cerr << "file:" << __FILE__<< ", line:" << __LINE__
                << ", open log file failed" << std::endl;
        return;
    }
    _outFile << hex;

    _outTestFile.open((_outFileName+".test").c_str());
    if ( _outTestFile.fail())
    {
        std::cerr << "file:" << __FILE__<< ", line:" << __LINE__
                << ", open test log file failed" << std::endl;
        return;
    }
    _outTestFile << hex;

    _pLogStream= new ostringstream();
    if ( NULL == _pLogStream )
    {
        std::cerr << "file:" << __FILE__<< ", line:" << __LINE__
                << ", new string stream failed" << std::endl;
        return;
    }
    (*_pLogStream) << hex;
    _logLines = 0;
    _logFileNum = 0;
}

/*****************************************************************************
 �� �� ��  : CallStack.~CallStack
 ��������  : ��պ������ù�ϵջ, �رռ�¼�ļ�
 �������  : ��
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��4��21��
    ��    ��   : @zhi
    �޸�����   : �����ɺ���

*****************************************************************************/
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

/*****************************************************************************
 �� �� ��  : CallStack.clearStack
 ��������  : ��պ������ù�ϵջ, ��ӡ�����Ϣ
 �������  : ��
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��4��21��
    ��    ��   : @zhi
    �޸�����   : �����ɺ���

*****************************************************************************/
void CallStack::clearStack()
{
    while(!_funcs.empty())
    {
        _outFile << "}//" << _funcs.top().funcName << std::endl;
        (*_pLogStream) << "}//" << _funcs.top().funcName << std::endl;

        _funcs.pop();
    }
}

/*****************************************************************************
 �� �� ��  : CallStack.addFunc
 ��������  : ���һ������������ջ, ͨ���Ƚϵ��õ�ǰ�����ĺ�����EBP����ջ����
             ��
 �������  : const char* funcName
             ADDRINT funcAddr
             ADDRINT upperFuncBP
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��4��21��
    ��    ��   : @zhi
    �޸�����   : �����ɺ���

*****************************************************************************/
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
        if ( NULL == _pLogStream )
        {
            std::cerr << "file:" << __FILE__<< ", line:" << __LINE__
                    << ", new string stream failed" << std::endl;
            return;
        }
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
/*****************************************************************************
 �� �� ��  : CallStack.out2logFiles
 ��������  : �ֲ���־����ʱ���ϴ����ļ�
 �������  : ��
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��4��21��
    ��    ��   : @zhi
    �޸�����   : �����ɺ���

*****************************************************************************/
void CallStack::out2logFiles()
{
    ostringstream osstream;
    osstream << _outFileName << "." << _logFileNum;

    ofstream splitFile;
    splitFile.open(osstream.str().c_str());
    if ( splitFile.fail())
    {
        std::cerr << "file:" << __FILE__<< ", line:" << __LINE__
                << ", open split log file failed" << std::endl;
        return;
    }
    //������ϴ���־�ļ�, ��--��β�ı�ʾ����δ��ֹ
    splitFile << (*_pLogStream).str();
    splitFile.close();

    ++ _logFileNum;
    _logLines = 0;
}

/*
static CallStack* CallStackFactory::pCallStack = NULL;
static CallStack& CallStackFactory::getCallStack()
{
	pCallStack = new CallStack();
	return *pCallStack;
}
static void CallStackFactory::delCallStack()
{
	if(NULL != pCallStack)
	{
		delete pCallStack;
		pCallStack = NULL;
	}
}
*/

