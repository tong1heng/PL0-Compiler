#include <iostream>
#include "syntax_analyzer.h"

extern Word *word;                 //识别的单词
extern int wordIndex;              //识别的单词索引（i.e. 识别的单词数量）




SyntaxAnalyzer::SyntaxAnalyzer() {
    ip = 0;
    cntPro = 0;
    dx = 3;

    synErr = false;
    smnErr = false;
}

SyntaxAnalyzer::~SyntaxAnalyzer() {

}

void SyntaxAnalyzer::Run() {
    Program();
    std::cout << "====================================================\n";
    if(synErr) {
        std::cout << "syntax analysis error!!!\n";
    }
    else if(smnErr) {
        std::cout << "semantic analysis error!!!\n";
    }
    else {
        std::cout << "syntax and semantic analysis success!!!\n";
    }
    std::cout << "====================================================\n";
}

void SyntaxAnalyzer::Show() {
    symbolTbl.PrintSymbolTable();
}

void SyntaxAnalyzer::Program() {   //程序
    SubProgram();

    if (word[ip].NAME == ".") {
        ip++;
//        std::cout << "syntax and semantic analysis success!!!\n";
    }
    else {
        synErr = true;
        std::cout << ip << ": " << word[ip].NAME << "\n";
        std::cout << "syntax error\n";
    }
}

void SyntaxAnalyzer::SubProgram() {
    dx = 3;

    if (word[ip].NAME == "const") ConstDeclare();           //常量说明部分
    if (word[ip].NAME == "var") VarDeclare();               //变量说明部分
    if (word[ip].NAME == "procedure") ProcedureDeclare();   //过程说明部分
    Statement();                                            //语句

    //Todo: emit 四元式

}

void SyntaxAnalyzer::ConstDeclare() {   //常量说明部分
    if (word[ip].NAME == "const") {
        ip++;
        ConstDefine();
        while (word[ip].NAME == ",") {
            ip++;
            ConstDefine();
        }
        if (word[ip].NAME == ";")   ip++;
        else {
            synErr = true;
            std::cout << "syntax error in ConstDeclare\n";
        }
    }
    else {
        synErr = true;
        std::cout << "syntax error in ConstDeclare\n";
    }
}

void SyntaxAnalyzer::VarDeclare() { //变量说明语句
    SymbolNode snode;													// snode: record symbol
    snode.kind = "VARIABLE";											// kind
    snode.para1 = cntPro;												// para1: LEVEL = cntPro

    if (word[ip].NAME == "var") {
        ip++;

        Identifier(snode.name);
        snode.para2 = dx++;												// para2: adr
        bool res = symbolTbl.insert(snode, cntPro);				// insert to the table
        if (!res)   smnErr = true;

        while (word[ip].NAME == ",") {
            ip++;

            Identifier(snode.name);
            snode.para2 = dx++;											// para2: adr
            bool res = symbolTbl.insert(snode, cntPro);			// insert to the table
            if (!res)   smnErr = true;
        }
        if (word[ip].NAME == ";")
            ip++;
        else {
            synErr = true;
            std::cout << "syntax error in VarDeclare\n";
        }
    }
    else {
        synErr = true;
        std::cout << "syntax error in VarDeclare\n";
    }
}

void SyntaxAnalyzer::ProcedureDeclare() {   //过程说明部分
    cntPro++;           // 创建一层
    if (cntPro > 3) {   // 不大于3层procedure
        smnErr = true;
        std::cout << "procedure depth out of 3\n";
    }

    ProcedureHead();
    SubProgram();

    if (word[ip].NAME == ";") {
        cntPro--;       // 分号完成一层
        ip++;
        while (word[ip].NAME == "procedure") {
            ProcedureDeclare();
        }
    }
    else {
        synErr = true;
        std::cout << "syntax error in ProcedureDeclare\n";
    }
}

void SyntaxAnalyzer::Statement() {  //语句
//    if (isexists(word[ip].NAME))
    if (word[ip+1].NAME == ":=")    //赋值语句
        AssignmentStatement();
    if (word[ip].NAME == "if")      //条件语句
        ConditionStatement();
    if (word[ip].NAME == "while")   //当型循环语句
        DoWhileStatement();
    if (word[ip].NAME == "call")    //过程调用语句
        CallStatement();
    if (word[ip].NAME == "read")    //读语句
        ReadStatement();
    if (word[ip].NAME == "write")   //写语句
        WriteStatement();
    if (word[ip].NAME == "begin")   //复合语句
        CompoundStatement();
}

void SyntaxAnalyzer::ConstDefine() {    //常量定义
    SymbolNode snode;													// snode: record symbol
    snode.kind = "CONSTANT";											// kind
    Identifier(snode.name);                                         // name

    if (word[ip].NAME == "=") {
        ip++;
        UnsignedInt(snode.para1);                                   // para1: val
        snode.para2 = cntPro;                                           // para1: level
        bool res = symbolTbl.insert(snode, cntPro);				// insert to the table
        if (!res)   smnErr = true;
    }
    else {
        synErr = true;
        std::cout << "syntax error in ConstDefine\n";
    }
}

void SyntaxAnalyzer::Identifier(std::string &id) { //标识符
    if(word[ip].SYM == "IDENT") {
        id = word[ip].NAME;
        ip++;
    }
    else {
        synErr = true;
        std::cout << "syntax error in Identifier\n";
    }

//    if (isexists(word[ip].NAME))
//        ip++;
//    else
//        std::cout << "syntax error\n";
}

void SyntaxAnalyzer::UnsignedInt(int &num) {    //无符号整数
    if(word[ip].SYM == "NUMBER") {
        num = stoi(word[ip].NAME);
        ip++;
    }
    else {
        synErr = true;
        std::cout << "syntax error in Identifier\n";
    }
//    if (isexistc(word[ip].NAME))
//        ip++;
//    else
//        std::cout << "syntax error\n";
}

void SyntaxAnalyzer::ProcedureHead() {  //过程首部
    if (word[ip].NAME == "procedure") {
        ip++;

        SymbolNode snode;													// snode: record symbol
        snode.kind = "PROCEDURE";											// kind
        Identifier(snode.name);										    // name
        snode.para1 = cntPro - 1;											// para1: LEVEL=cntPro-1 因为上一个判断已经提前层数+1，但是这个过程名属于上一层
        snode.para2 = 0;													// para2: 0 indicates it doesn't have addr yet.

        bool res =symbolTbl.insert(snode, cntPro);					// insert to the table
        if (!res)   smnErr = true;

        if (word[ip].NAME == ";")
            ip++;
        else {
            synErr = true;
            std::cout << "syntax error in ProcedureHead\n";
        }
    }
    else {
        synErr = true;
        std::cout << "syntax error in ProcedureHead\n";
    }
}

void SyntaxAnalyzer::AssignmentStatement() {    //赋值语句
    std::string tmp_id;
    Identifier(tmp_id);

    if (word[ip].NAME == ":=") {
        ip++;
        Expression();


        int pos = symbolTbl.checkPos(tmp_id, { 2 }, cntPro);
        if (pos < 0) {
            smnErr = true;
        }

        SymbolNode sym = symbolTbl.getNode(pos);
        // Todo: emit 四元式

    }
    else {
        synErr = true;
        std::cout << "syntax error in AssignmentStatement\n";
    }
}

void SyntaxAnalyzer::ConditionStatement() { //条件语句
    if (word[ip].NAME == "if") {
        ip++;
        Condition();

        // Todo: emit 四元式 下方statement之后反填

        if (word[ip].NAME == "then") {
            ip++;
            Statement();

            // Todo: 反填
        }
        else {
            synErr = true;
            std::cout << "syntax error in ConditionStatement\n";
        }
    }
    else {
        synErr = true;
        std::cout << "syntax error in ConditionStatement\n";
    }
}

void SyntaxAnalyzer::DoWhileStatement() {    //当型循环语句
    if (word[ip].NAME == "while") {
        ip++;
        Condition();

        // Todo: emit四元式，下方反填

        if (word[ip].NAME == "do") {
            ip++;
            Statement();

            // Todo: 反填
        }
        else {
            synErr = true;
            std::cout << "syntax error in DoWhileStatement\n";
        }
    }
    else {
        synErr = true;
        std::cout << "syntax error in DoWhileStatement\n";
    }
}

void SyntaxAnalyzer::CallStatement() {    //过程调用语句
    if (word[ip].NAME == "call") {
        ip++;

        std::string tmp_id;
        Identifier(tmp_id);

        int pos = symbolTbl.checkPos(tmp_id, { 3 }, cntPro);
        if (pos < 0)    smnErr = true;

        // Todo: 四元式和反填
        SymbolNode tmp_n = symbolTbl.getNode(pos);

    }
    else {
        synErr = true;
        std::cout << "syntax error in CallStatement\n";
    }
}

void SyntaxAnalyzer::ReadStatement() {  //读语句
    if (word[ip].NAME == "read") {
        ip++;
        if (word[ip].NAME == "(") {
            ip++;

            std::string tmp_id;
            Identifier(tmp_id);

            int pos = symbolTbl.checkPos(tmp_id, { 2 }, cntPro);
            if (pos < 0) {
                smnErr = true;
            }
            SymbolNode tmp_n = symbolTbl.getNode(pos);
            // Todo: emit 四元式


            while (word[ip].NAME == ",") {
                ip++;

                std::string temp_id;
                Identifier(temp_id);

                int tpos = symbolTbl.checkPos(temp_id, { 2 }, cntPro);
                if (tpos < 0) {
                    smnErr = true;
                }
                SymbolNode temp_n = symbolTbl.getNode(tpos);
                // Todo: emit 四元式

            }
            if (word[ip].NAME == ")")   ip++;
            else {
                synErr = true;
                std::cout << "syntax error in ReadStatement\n";
            }
        }
        else {
            synErr = true;
            std::cout << "syntax error in ReadStatement\n";
        }
    }
    else {
        synErr = true;
        std::cout << "syntax error in ReadStatement\n";
    }
}

void SyntaxAnalyzer::WriteStatement() { //写语句
    if (word[ip].NAME == "write") {
        ip++;
        if (word[ip].NAME == "(") {
            ip++;

            std::string tmp_id;
            Identifier(tmp_id);

            int pos = symbolTbl.checkPos(tmp_id, { 1, 2 }, cntPro);
            if (pos < 0) {
                smnErr = true;
            }
            SymbolNode tmp_n = symbolTbl.getNode(pos);
            // Todo: emit 四元式

            while (word[ip].NAME == ",") {
                ip++;

                std::string temp_id;
                Identifier(temp_id);

                int tpos = symbolTbl.checkPos(temp_id, { 1, 2 }, cntPro);
                if (tpos < 0) {
                    smnErr = true;
                }
                SymbolNode temp_n = symbolTbl.getNode(tpos);
                // Todo: emit 四元式

            }
            if (word[ip].NAME == ")")   ip++;
            else {
                synErr = true;
                std::cout << "syntax error in WriteStatement\n";
            }
        }
        else {
            synErr = true;
            std::cout << "syntax error in WriteStatement\n";
        }
    }
    else {
        synErr = true;
        std::cout << "syntax error in WriteStatement\n";
    }
}

void SyntaxAnalyzer::CompoundStatement() {  //复合语句
    if (word[ip].NAME == "begin") {
        ip++;
        Statement();
        while (word[ip].NAME == ";") {
            ip++;
            Statement();
        }
        if (word[ip].NAME == "end") ip++;
        else {
            synErr = true;
            std::cout << "syntax error in CompoundStatement\n";
        }
    }
    else {
        synErr = true;
        std::cout << "syntax error in CompoundStatement\n";
    }
}

void SyntaxAnalyzer::Expression() { //表达式
    if (word[ip].NAME == "+" || word[ip].NAME == "-") {
        ip++;
    }
    Item();

    //Todo: 获取表达式符号 emit四元式

    while (word[ip].NAME == "+" || word[ip].NAME == "-") {
        AddSub();
        Item();

        //Todo: 获取表达式符号 emit四元式

    }
}

void SyntaxAnalyzer::Condition() { //条件
    if (word[ip].NAME == "odd") {
        ip++;
        Expression();
        //Todo: emit和odd相关的四元式
    }
    else {
        Expression();
        Relationship();
        Expression();

        //Todo: 根据关系运算符emit四元式 需要获取Relationship()识别的运算符
    }
}

void SyntaxAnalyzer::Item() {   //项
    Factor();
    while (word[ip].NAME=="*"|| word[ip].NAME == "/") {
        MulDiv();
        Factor();

        //Todo: 获取 * or / 符号 emit四元式

    }
}

void SyntaxAnalyzer::Factor() { //因子
    if ((word[ip].NAME.at(0) <= 122 && word[ip].NAME.at(0) >= 97)) { //标识符
//    if (word[ip].NAME == "IDENT") { //标识符
        std::string tmp_id;
        Identifier(tmp_id);
        //Todo: 四元式

    }
    else if ((word[ip].NAME.at(0) <= 57 && word[ip].NAME.at(0) >= 48)) { //无符号整数
//    else if(word[ip].NAME == "NUMBER") { //无符号整数
        int tmp_num;
        UnsignedInt(tmp_num);
        //Todo: 四元式

    }
    else if (word[ip].NAME == "(") {
        Expression();
        if (word[ip].NAME == ")")   ip++;
        else {
            synErr = true;
            std::cout << "syntax error in Factor\n";
        }
    }
    else {
        synErr = true;
        std::cout << "syntax error in Factor\n";
    }
}

void SyntaxAnalyzer::AddSub() { //加减运算符
    if (word[ip].NAME == "+" || word[ip].NAME == "-")   ip++;
    else    std::cout << "syntax error in AddSub\n";
}

void SyntaxAnalyzer::MulDiv() {   //乘除运算符
    if (word[ip].NAME == "*" || word[ip].NAME == "/")   ip++;
    else    std::cout << "syntax error in MulDiv\n";
}

void SyntaxAnalyzer::Relationship() {   //关系运算符
    if (word[ip].NAME == "=" || word[ip].NAME == "#" || word[ip].NAME=="<" ||
        word[ip].NAME == "<=" ||word[ip].NAME == ">" || word[ip].NAME == ">=") {
        ip++;
    }
    else {
        synErr = true;
        std::cout << "syntax error in Relationship\n";
    }
}





////添加code，运算类型
//void SyntaxAnalyzer::codeAdd(std::string str,int lev,int place,std::string con) {
//    code[codeIdx].funcCode = str;
//    code[codeIdx].levelDiff = lev;
//    code[codeIdx].displacement = place;
//    code[codeIdx].content = con;
//    codeIdx++;
//}
////添加code，非运算
//void SyntaxAnalyzer::codeAdd(std::string str, int lev, int place) {
//    code[codeIdx].funcCode = str;
//    code[codeIdx].levelDiff = lev;
//    code[codeIdx].displacement = place;
//    codeIdx++;
//}
