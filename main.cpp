#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "compiler.h"
#include "interpreter.h"

int main() {
    std::ifstream in("PL0_code0.in");
    std::string input;                      // �����ļ���i.e. Դ�������ݵ��ַ���
    std::ostringstream tmp;
    tmp << in.rdbuf();
    input = tmp.str();
    std::cout << input << "\n";

    Compiler *compiler = new Compiler(input);
    compiler->LexicalAnalysis();
    compiler->SyntaxAnalysis();

    Interpreter *interpreter = new Interpreter(compiler->GetTargetCode());
    interpreter->Run();

    delete compiler;
    delete interpreter;
    return 0;
}