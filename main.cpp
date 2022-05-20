#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "word.h"
#include "compiler.h"

std::string input;          // 输入文件（i.e. 源程序）内容的字符串
Word *word;                 // 识别的单词
int wordIndex;              // 识别的单词索引（i.e. 识别的单词数量）

void Initialize();

int main() {
    std::ifstream in("PL0_code.in");
    std::ostringstream tmp;
    tmp << in.rdbuf();
    input = tmp.str();
    std::cout << input << "\n";

    Initialize();

    Compiler *compiler = new Compiler();
    compiler->LexicalAnalysis();
    compiler->SyntaxAnalysis();

    delete compiler;
    delete []word;
    return 0;
}

void Initialize() {
    word = new Word[1000];
    wordIndex = 0;
}
