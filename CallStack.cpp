#include <ctime>
#include "CallStack.h"

const unsigned long MAX_LINES = 5;

//�򿪼�¼�ļ�, �����������Ϊ16���Ƹ�ʽ
CallStack::CallStack(string outFileName):_outFileName(outFileName)
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

