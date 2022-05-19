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
//    Word *word;                 //识别的单词
//    int wordIndex;              //识别的单词索引（i.e. 识别的单词数量）
    int index;                  //当前input的下标
    char ch;                    //当前读入的源程序字符
    std::string token;          //当前识别的单词

    //关键字
    std::string keyLabel[29] = {"+", "-", "*", "/", "=", "#", "<", "<=", ">",
                                ">=", ",", ";", ":=", "(", ")", ".", "const",
                                "var","procedure", "begin", "end", "odd", "if",
                                "then", "call", "while", "do", "read", "write"};
    std::string identifyLabel[100] = {};        //标识符
    std::string constLabel[100] = {};           //常量
    int identifyIndex;          //标识符表的下标
    int constIndex;             //常量表的下标

    State *state[7];             //state: 0~6
    State *currentState;

    void GetChar() ;        //将下一输入字符读到ch中
    void Backspace();       //回退一个字符，并跳过空格
    void Concat();          //将当前字符连接到token中
    int IsKey();            //判断是否是关键字，如果是返回关键字的索引，否则返回-1

    State* ChangeState(State *old, char c); //状态转换
};


#endif //MYCOMPILER_LEXICAL_ANALYZER_H
