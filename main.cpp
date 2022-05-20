#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "word.h"
#include "compiler.h"

std::string input;          // �����ļ���i.e. Դ�������ݵ��ַ���
Word *word;                 // ʶ��ĵ���
int wordIndex;              // ʶ��ĵ���������i.e. ʶ��ĵ���������

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
