#ifndef MYCOMPILER_COMPILER_H
#define MYCOMPILER_COMPILER_H

#include "lexical_analyzer.h"
#include "syntax_analyzer.h"
#include <string>

class Compiler {
public:
    Compiler(std::string& in);
    ~Compiler();

    void LexicalAnalysis();                             // �ʷ�����
    void SyntaxAnalysis();                              // �﷨���� & �������
    std::vector<TargetCodeNode>& GetTargetCode();       // ��ȡ���ɵ�Ŀ�����

private:
    LexicalAnalyzer *LA;
    SyntaxAnalyzer *SA;
};


#endif //MYCOMPILER_COMPILER_H
