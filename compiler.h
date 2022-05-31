#ifndef MYCOMPILER_COMPILER_H
#define MYCOMPILER_COMPILER_H

#include "lexical_analyzer.h"
#include "syntax_analyzer.h"
#include <string>

class Compiler {
public:
    Compiler(std::string& in);
    ~Compiler();

    void LexicalAnalysis();                             // 词法分析
    void SyntaxAnalysis();                              // 语法分析 & 语义分析
    std::vector<TargetCodeNode>& GetTargetCode();       // 获取生成的目标代码

private:
    LexicalAnalyzer *LA;
    SyntaxAnalyzer *SA;
};


#endif //MYCOMPILER_COMPILER_H
