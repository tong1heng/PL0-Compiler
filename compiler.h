#ifndef MYCOMPILER_COMPILER_H
#define MYCOMPILER_COMPILER_H

#include "lexical_analyzer.h"
#include "syntax_analyzer.h"

class Compiler {
public:
    Compiler();
    ~Compiler();

    void LexicalAnalysis();     //�ʷ�����
    void SyntaxAnalysis();      //�﷨����

private:
    LexicalAnalyzer *LA;
    SyntaxAnalyzer *SA;
};


#endif //MYCOMPILER_COMPILER_H
