#ifndef MYCOMPILER_LEXICAL_ANALYZER_H
#define MYCOMPILER_LEXICAL_ANALYZER_H

#include <string>
#include "word.h"

class State {
public:
    int id;
    State(int i) {
        id = i;
    }
};

class LexicalAnalyzer {
public:
    LexicalAnalyzer();
    ~LexicalAnalyzer();

    void Run();
    void Show();

private:
//    Word *word;                 //ʶ��ĵ���
//    int wordIndex;              //ʶ��ĵ���������i.e. ʶ��ĵ���������
    int index;                  //��ǰinput���±�
    char ch;                    //��ǰ�����Դ�����ַ�
    std::string token;          //��ǰʶ��ĵ���

    //�ؼ���
    std::string keyLabel[29] = {"+", "-", "*", "/", "=", "#", "<", "<=", ">",
                                ">=", ",", ";", ":=", "(", ")", ".", "const",
                                "var","procedure", "begin", "end", "odd", "if",
                                "then", "call", "while", "do", "read", "write"};
    std::string identifyLabel[100] = {};        //��ʶ��
    std::string constLabel[100] = {};           //����
    int identifyIndex;          //��ʶ������±�
    int constIndex;             //��������±�

    State *state[7];             //state: 0~6
    State *currentState;

    void GetChar() ;        //����һ�����ַ�����ch��
    void Backspace();       //����һ���ַ����������ո�
    void Concat();          //����ǰ�ַ����ӵ�token��
    int IsKey();            //�ж��Ƿ��ǹؼ��֣�����Ƿ��عؼ��ֵ����������򷵻�-1

    State* ChangeState(State *old, char c); //״̬ת��
};


#endif //MYCOMPILER_LEXICAL_ANALYZER_H
