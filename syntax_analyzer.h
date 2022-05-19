#ifndef MYCOMPILER_SYNTAX_ANALYZER_H
#define MYCOMPILER_SYNTAX_ANALYZER_H

#include "word.h"
#include "node.h"
#include "symbol_table.h"
#include <string>

//class TarInstruc {
//public:
//    std::string funcCode;
//    int levelDiff;
//    int displacement;
//    std::string content;
//    TarInstruc() {
//        funcCode=" ";
//        levelDiff = 0;
//        displacement = 0;
//        content = " ";
//    }
//};
//
//TarInstruc code[200];


class SyntaxAnalyzer {
public:
    SyntaxAnalyzer();
    ~SyntaxAnalyzer();

    void Run();
    void Show();

private:

    /* syntax analysis */

    int ip;

    void Program();             //程序
    void SubProgram();          //子程序

    void ConstDeclare();        //常量说明部分
    void VarDeclare();          //变量说明部分
    void ProcedureDeclare();    //过程说明部分
    void Statement();           //语句

    void ConstDefine();         //常量定义
    void Identifier(std::string &id);          //标识符
    void UnsignedInt(int &num);         //无符号整数
    void ProcedureHead();       //过程首部

    void AssignmentStatement(); //赋值语句
    void ConditionStatement();  //条件语句
    void DoWhileStatement();    //当型循环语句
    void CallStatement();       //过程调用语句
    void ReadStatement();       //读语句
    void WriteStatement();      //写语句
    void CompoundStatement();   //复合语句

    void Expression();          //表达式
    void Condition();           //条件
    void Item();                //项
    void Factor();              //因子
    void AddSub();              //加减运算符
    void MulDiv();              //乘除运算符
    void Relationship();        //关系运算符

    /* semantic analysis */

//    int procedureDepth;         //嵌套的过程深度
//    int tableIdx;               //符号表索引
//    int dx;                     //每层的局部变量的相对地址

    Node *ASTroot;					// 语法分析树
    std::string _resStr;			// 语法分析结果字符串
    bool synErr;					// 语法分析错误

    int cntPro;						// procedure嵌套计数
    SymbolTable symbolTbl;			// 符号表
    int dx;							// 在procedure和构造函数中 = 3;  and +1 when var declared
    bool smnErr;					// 语义错误

    int Find(std::string str);
    std::string Find(int *addr);

    int codeIdx;
    void codeAdd(std::string str,int lev,int place,std::string con);
    void codeAdd(std::string str, int lev, int place);

};


#endif //MYCOMPILER_SYNTAX_ANALYZER_H
