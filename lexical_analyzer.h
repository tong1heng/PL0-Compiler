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
    std::string input;          // �����Դ��������
    int index;                  // ��ǰinput���±�
    char ch;                    // ��ǰ�����Դ�����ַ�
    std::vector<Word> word;     // ʶ��ĵ���
    std::string token;          // ��ǰʶ��ĵ���
    // �ؼ���
    std::string keyLabel[29] = {"+", "-", "*", "/", "=", "#", "<", "<=", ">",
                                ">=", ",", ";", ":=", "(", ")", ".", "const",
                                "var","procedure", "begin", "end", "odd", "if",
                                "then", "call", "while", "do", "read", "write"};
    State *state[7];            // state: 0 ~ 6
    State *currentState;        // ��ǰ��state

    void GetChar() ;            // ����һ�����ַ�����ch��
    void Backspace();           // ����һ���ַ����������ո�
    void Concat();              // ����ǰ�ַ����ӵ�token��
    int IsKey();                // �ж��Ƿ��ǹؼ��֣�����Ƿ��عؼ��ֵ����������򷵻�-1
    State* ChangeState(State *old, char c); // ״̬ת��
};

#endif //MYCOMPILER_LEXICAL_ANALYZER_H
