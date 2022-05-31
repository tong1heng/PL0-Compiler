#ifndef MYCOMPILER_LEXICAL_ANALYZER_H
#define MYCOMPILER_LEXICAL_ANALYZER_H

#include <string>
#include <vector>
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
    LexicalAnalyzer(std::string& in);
    ~LexicalAnalyzer();

    void Run();
    void Show();
    std::vector<Word>& GetWord();

private:
    std::string input;          // 读入的源程序内容
    int index;                  // 当前input的下标
    char ch;                    // 当前读入的源程序字符
    std::vector<Word> word;     // 识别的单词
    std::string token;          // 当前识别的单词
    // 关键字
    std::string keyLabel[29] = {"+", "-", "*", "/", "=", "#", "<", "<=", ">",
                                ">=", ",", ";", ":=", "(", ")", ".", "const",
                                "var","procedure", "begin", "end", "odd", "if",
                                "then", "call", "while", "do", "read", "write"};
    State *state[7];            // state: 0 ~ 6
    State *currentState;        // 当前的state

    void GetChar() ;            // 将下一输入字符读到ch中
    void Backspace();           // 回退一个字符，并跳过空格
    void Concat();              // 将当前字符连接到token中
    int IsKey();                // 判断是否是关键字，如果是返回关键字的索引，否则返回-1
    State* ChangeState(State *old, char c); // 状态转换
};

#endif //MYCOMPILER_LEXICAL_ANALYZER_H
