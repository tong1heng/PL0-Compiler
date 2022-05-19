#include <iostream>
#include "lexical_analyzer.h"

extern std::string input;
extern Word *word;                 //识别的单词
extern int wordIndex;              //识别的单词索引（i.e. 识别的单词数量）

std::string toUpper(std::string s) {
    std::string ret(s);
    for(int i = 0; i <= ret.size(); i++) {
        if(ret[i] >= 'a' && ret[i] <= 'z') {
           ret[i] = ret[i] + 'A' - 'a';
        }
    }
    return ret;
}

LexicalAnalyzer::LexicalAnalyzer() {
//    word = new Word[1000];
//    wordIndex = 0;

    token = std::string();
    identifyIndex = 0;       //标识符表的下标
    constIndex = 0;          //常量表的下标

    index = 0;
    for(int i = 0; i < 7; i++) { //定义7个状态
        state[i] = new State(i);
    }
    currentState = state[0];
}

LexicalAnalyzer::~LexicalAnalyzer() {
    delete word;
    for(int i = 0; i < 7; i++) {
        delete state[i];
    }
}

void LexicalAnalyzer::GetChar() {   //将下一输入字符读到ch中
    ch = input[index];
    index++;
}

void LexicalAnalyzer::Backspace() { //回退一个字符，并跳过空格
    index--;
    if(input[index] == ' ') {
        index++;
    }
}

void LexicalAnalyzer::Concat() {    //将当前字符连接到token中
    token.insert(token.end(),ch);
}

int LexicalAnalyzer::IsKey() {      //判断是否是关键字，如果是返回关键字的索引，否则返回-1
    for (int i = 0; i < 29; i++) {
        if (keyLabel[i] == token)
            return i;
    }
    return -1;
}

State* LexicalAnalyzer::ChangeState(State *old, char c) {   //状态转换
    switch (old->id) {
        case 0: {
            if(c == ',' || c == ';' || c == '.') { //界符
                Concat();
                return state[1];
            }
            else if(c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == '=' || c == '#' ||
                    c == ':' || c == '>' || c == '<') { //算符
                Concat();
                return state[2];
            }
            else if(c >= '0' && c <= '9') { //常量
                Concat();
                return state[3];
            }
            else if(c >= 'a' && c <= 'z') { //标识符或关键字
                Concat();
                return state[4];
            }
            else {  //error
                return state[6];
            }
        }
        case 1: {   //识别出一个界符
            Backspace();
            word[wordIndex].SYM = token;
            word[wordIndex++].NAME = token;
            return state[5];
        }
        case 2: {
            if(c == '=' && (token == ">" || token == "<" || token == ":")) { // >= or <= or :=
                Concat();
                return state[2];
            }
            else {  //识别出一个算符
                Backspace();
                word[wordIndex].SYM = token;
                word[wordIndex++].NAME = token;
                return state[5];
            }
        }
        case 3: {
            if(c >= '0' && c <= '9') { //常量
                Concat();
                return state[3];
            }
            else {  //识别出一个常量
                Backspace();
                word[wordIndex].SYM = "NUMBER";
                word[wordIndex++].NAME = token;
                return state[5];
            }
        }
        case 4: {
            if((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z')) { //标识符或关键字
                Concat();
                return state[4];
            }
            else {
                Backspace();
                if(IsKey() >= 0) { //识别出一个关键字
                    word[wordIndex].SYM = toUpper(token) + "SYM";
                    word[wordIndex++].NAME = token;
                }
                else {  //识别出一个标识符
                    word[wordIndex].SYM = "IDENT";
                    word[wordIndex++].NAME = token;
                }
                return state[5];
            }
        }
    }
    return state[6];        // not reached
}

void LexicalAnalyzer::Run() {   //进行词法分析
    //预处理
    for(int i = 0; i < input.size() ; i++) {
        if(input[i] == '\n' || input[i] == '\t' || input[i] == '\r') {
            input[i] = ' ';
        }
        if(i > 0) {
            if(input[i-1] == ' ' && input[i] == ' ') {
                auto it=input.begin();
                it += i;
                input.erase(it);
                i--;
            }
        }
    }

    std::cout << "***Codes after preprocessing:***\n";
    std::cout << input << "\n";

    //根据当前状态和当前读入字符进行状态转换
//    while (input[index] != 0) {
    while (index <= input.size()) {
        GetChar();

        State *newState = ChangeState(currentState, ch);
        currentState = newState;

        if(currentState->id == 5) {
            token.clear();
            currentState = state[0];    // for new word
        }
        if(currentState->id == 6 && ch != 0) {
            std::cout << "Error: " << ch << " can not be identified.\n";
            break;
        }

//        std::cout << "*********DEBUG***********\n";
//        for (int i = 0; i < wordIndex; i++) {
//            std::cout << word[i].SYM << "\t\t" << word[i].NAME << "\n";
//        }
//        std::cout << "*************************\n";

    }
}

void LexicalAnalyzer::Show() {  //打印信息
    std::cout << "─────────────────────────────────────────────────────────────────\n";
    std::cout << "│\t\tSYM\t\t│\t\tNAME\t\t│\n";
    std::cout << "─────────────────────────────────────────────────────────────────\n";
    for (int i = 0; i < wordIndex; i++) {
        if(word[i].SYM.size() >= 8) {
            if(word[i].NAME.size() >= 8) {
                std::cout << "│\t\t" << word[i].SYM << "\t│\t\t" << word[i].NAME << "\t│" << "\n";
            }
            else {
                std::cout << "│\t\t" << word[i].SYM << "\t│\t\t" << word[i].NAME << "\t\t│" << "\n";
            }
        }
        else {
            if(word[i].NAME.size() >= 8) {
                std::cout << "│\t\t" << word[i].SYM << "\t\t│\t\t" << word[i].NAME << "\t│" << "\n";
            }
            else {
                std::cout << "│\t\t" << word[i].SYM << "\t\t│\t\t" << word[i].NAME << "\t\t│" << "\n";
            }
        }
    }
    std::cout << "─────────────────────────────────────────────────────────────────\n";
}
