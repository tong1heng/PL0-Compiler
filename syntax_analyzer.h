#ifndef MYCOMPILER_SYNTAX_ANALYZER_H
#define MYCOMPILER_SYNTAX_ANALYZER_H

#include "word.h"
#include "node.h"
#include "symbol_table.h"
#include "quaternary.h"
#include "target_code.h"
#include <string>

class SyntaxAnalyzer {
public:
    SyntaxAnalyzer();
    ~SyntaxAnalyzer();

    void Run();
    void Show();
    void SetWord(std::vector<Word>& word);          // 获取词法分析的结果
    std::vector<TargetCodeNode>& GetTargetCode();   // 返回生成的目标代码

private:
    std::vector<Word> word;             // 词法分析得到的单词
    int ip;                             // 当前分析的单词索引

    /* syntax analysis */

//    Node *ASTroot;					// 语法分析树
//    std::string _resStr;			    // 语法分析结果字符串
    bool syntaxError;					// 语法分析错误

    void Program();                     // 程序
    void SubProgram(int px);            // 子程序

    void ConstDeclare();                // 常量说明部分
    void VarDeclare();                  // 变量说明部分
    void ProcedureDeclare();            // 过程说明部分
    void Statement();                   // 语句

    void ConstDefine();                 // 常量定义
    void Identifier(std::string &id);   // 标识符
    void UnsignedInt(int &num);         // 无符号整数
    void ProcedureHead();               // 过程首部

    void AssignmentStatement();         // 赋值语句
    void ConditionStatement();          // 条件语句
    void DoWhileStatement();            // 当型循环语句
    void CallStatement();               // 过程调用语句
    void ReadStatement();               // 读语句
    void WriteStatement();              // 写语句
    void CompoundStatement();           // 复合语句

    void Expression(std::string &ret);  // 表达式
    void Condition(Code &code);         // 条件
    void Item(std::string &ret);        // 项
    void Factor(std::string &ret);      // 因子
    void AddSub();                      // 加减运算符
    void MulDiv();                      // 乘除运算符
    void Relationship(std::string &ret);//关系运算符

    /* semantic analysis */

    int procedureDepth;					// procedure嵌套计数
    SymbolTable symbolTable;			// 符号表
    int dx;							    // 在procedure和构造函数中 = 3; + 1 when var declared
    bool semanticError;					// 语义错误

    Quaternary quaternary;
    TargetCode targetCode;
//    std::vector<std::pair<int, SymbolNode> > incmpltCode;
};


#endif //MYCOMPILER_SYNTAX_ANALYZER_H
