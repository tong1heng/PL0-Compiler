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
    std::cout << "\n========================================\n";
    if(synErr) {
        std::cout << "syntax analysis error!!!\n";
    }
    else if(smnErr) {
        std::cout << "semantic analysis error!!!\n";
    }
    else {
        std::cout << "syntax and semantic analysis success!!!\n";
    }
    std::cout << "========================================\n\n";
}

void SyntaxAnalyzer::Show() {
    symbolTbl.PrintSymbolTable();
    quaternary.Print();
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
        if (!res) {
            smnErr = true;
            return ;
        }

        while (word[ip].NAME == ",") {
            ip++;

            Identifier(snode.name);
            snode.para2 = dx++;											// para2: adr
            bool res = symbolTbl.insert(snode, cntPro);			// insert to the table
            if (!res) {
                smnErr = true;
                return ;
            }
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
        return ;
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
    SymbolNode snode;												// snode: record symbol
    snode.kind = "CONSTANT";										// kind
    Identifier(snode.name);                                     // name

    if (word[ip].NAME == "=") {
        ip++;
        UnsignedInt(snode.para1);                               // para1: val
        snode.para2 = cntPro;                                       // para1: level
        bool res = symbolTbl.insert(snode, cntPro);	        // insert to the table
        if (!res) {
            smnErr = true;
            return ;
        }
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

        SymbolNode snode;											// snode: record symbol
        snode.kind = "PROCEDURE";									// kind
        Identifier(snode.name);								    // name
        snode.para1 = cntPro - 1;									// para1: LEVEL=cntPro-1 因为上一个判断已经提前层数+1，但是这个过程名属于上一层
        snode.para2 = 0;											// para2: 0 indicates it doesn't have addr yet.

        bool res =symbolTbl.insert(snode, cntPro);			// insert to the table
        if (!res) {
            smnErr = true;
            return ;
        }

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
    Code code;

    Identifier(tmp_id);

    if (word[ip].NAME == ":=") {
        ip++;
        code.op = "=";
        Expression(code.arg1);

        int pos = symbolTbl.checkPos(tmp_id, { 2 }, cntPro);
        if (pos < 0) {
            smnErr = true;
            return ;
        }

        // Done: emit 四元式
        SymbolNode sym = symbolTbl.getNode(pos);
        code.arg2 = "-";
        code.result = sym.name;
        quaternary.emit(code);
    }
    else {
        synErr = true;
        std::cout << "syntax error in AssignmentStatement\n";
    }
}

void SyntaxAnalyzer::ConditionStatement() { //条件语句
    if (word[ip].NAME == "if") {
        ip++;

        Code trueCode, falseCode;
        Condition(trueCode);
        falseCode.op = "j";
        falseCode.arg1 = "-";
        falseCode.arg2 = "-";
        // Done: emit 四元式 下方statement反填
        int tnxq = quaternary.NextCodeId();
        quaternary.emit(trueCode);
        int fnxq = quaternary.NextCodeId();
        quaternary.emit(falseCode);

        if (word[ip].NAME == "then") {
            ip++;

            // Done: 反填真出口
            quaternary.BackPatch(tnxq, quaternary.NextCodeId());
            Statement();

            // Done: 反填假出口
            quaternary.BackPatch(fnxq,quaternary.NextCodeId());
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

        Code trueCode, falseCode;
        Condition(trueCode);
        falseCode.op = "j";
        falseCode.arg1 = "-";
        falseCode.arg2 = "-";

        // Done: emit四元式，下方statement反填
        int tnxq = quaternary.NextCodeId();
        quaternary.emit(trueCode);
        int fnxq = quaternary.NextCodeId();
        quaternary.emit(falseCode);

        if (word[ip].NAME == "do") {
            ip++;

            // Done: 反填真出口
            quaternary.BackPatch(tnxq, quaternary.NextCodeId());
            Statement();

            // Done: 反填假出口
            quaternary.BackPatch(fnxq,quaternary.NextCodeId());
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
        if (pos < 0) {
            smnErr = true;
            return ;
        }

        // Done: emit四元式
        SymbolNode tmp_n = symbolTbl.getNode(pos);
        Code code;
        code.op = "call";
        code.arg1 = "-";
        code.arg2 = "-";
        code.result = tmp_n.name;
        quaternary.emit(code);
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
                return ;
            }

            SymbolNode tmp_n = symbolTbl.getNode(pos);
            // Done: emit 四元式
            Code code;
            code.op = "read";
            code.arg1 = "-";
            code.arg2 = "-";
            code.result = tmp_n.name;
            quaternary.emit(code);

            while (word[ip].NAME == ",") {
                ip++;

                std::string temp_id;
                Identifier(temp_id);

                int tpos = symbolTbl.checkPos(temp_id, { 2 }, cntPro);
                if (tpos < 0) {
                    smnErr = true;
                    return ;
                }

                SymbolNode temp_n = symbolTbl.getNode(tpos);
                // Done: emit 四元式
                Code tcode;
                tcode.op = "read";
                tcode.arg1 = "-";
                tcode.arg2 = "-";
                tcode.result = temp_n.name;
                quaternary.emit(tcode);

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
                return ;
            }

            SymbolNode tmp_n = symbolTbl.getNode(pos);
            // Done: emit 四元式
            Code code;
            code.op = "write";
            code.arg1 = "-";
            code.arg2 = "-";
            code.result = tmp_n.name;
            quaternary.emit(code);

            while (word[ip].NAME == ",") {
                ip++;

                std::string temp_id;
                Identifier(temp_id);

                int tpos = symbolTbl.checkPos(temp_id, { 1, 2 }, cntPro);
                if (tpos < 0) {
                    smnErr = true;
                    return ;
                }

                SymbolNode temp_n = symbolTbl.getNode(tpos);
                // Done: emit 四元式
                Code tcode;
                tcode.op = "write";
                tcode.arg1 = "-";
                tcode.arg2 = "-";
                tcode.result = temp_n.name;
                quaternary.emit(tcode);
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

void SyntaxAnalyzer::Expression(std::string &ret) { //表达式
    bool isMinus = false;
    Code code;

    if (word[ip].NAME == "+" || word[ip].NAME == "-") {
        isMinus = true;         //取负的"-"
        ip++;
    }

    Item(code.arg1);
//    ret = code.arg1;

    //Done: 如果取负 emit四元式
    if(isMinus) {   //取负
        code.arg2 = "-";
        code.op = "uminus"; // uminus 和 - 进行区分
        code.result = quaternary.NewTemp();
        quaternary.emit(code);
        code.arg1 = code.result;
    }

    while (word[ip].NAME == "+" || word[ip].NAME == "-") {
        code.op = word[ip].NAME;
        AddSub();
        Item(code.arg2);

        //Done: 获取表达式符号 emit四元式
        code.result = quaternary.NewTemp();
        quaternary.emit(code);
        code.arg1 = code.result;
    }
    ret = code.arg1;
}

void SyntaxAnalyzer::Condition(Code &code) { //条件
    if (word[ip].NAME == "odd") {
        ip++;
        Expression(code.arg1);
        //Done: emit和odd相关的四元式
        code.op = "odd";
    }
    else {
        Expression(code.arg1);
        Relationship(code.op);
        Expression(code.arg2);
        //Done: 根据关系运算符emit四元式 需要获取Relationship()识别的运算符
    }
}

void SyntaxAnalyzer::Item(std::string &ret) {   //项
    Code code;

    Factor(code.arg1);
//    ret = code.arg1;

    while (word[ip].NAME == "*" || word[ip].NAME == "/") {
        //Done: 获取乘除号 emit四元式
        code.op = word[ip].NAME;
        MulDiv();
        Factor(code.arg2);
        code.result = quaternary.NewTemp();
        quaternary.emit(code);
        code.arg1 = code.result;
    }
    ret = code.arg1;
}

void SyntaxAnalyzer::Factor(std::string &ret) { //因子
    if ((word[ip].NAME.at(0) <= 122 && word[ip].NAME.at(0) >= 97)) { //标识符
//    if (word[ip].NAME == "IDENT") { //标识符
        std::string tmp_id;
        Identifier(tmp_id);
        //Done: 四元式
        ret = tmp_id;
    }
    else if ((word[ip].NAME.at(0) <= 57 && word[ip].NAME.at(0) >= 48)) { //无符号整数
//    else if(word[ip].NAME == "NUMBER") { //无符号整数
        int tmp_num;
        UnsignedInt(tmp_num);
        //Done: 四元式
        ret = std::to_string(tmp_num);
    }
    else if (word[ip].NAME == "(") {
        Expression(ret);
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

void SyntaxAnalyzer::Relationship(std::string &ret) {   //关系运算符
    if (word[ip].NAME == "=" || word[ip].NAME == "#" || word[ip].NAME=="<" ||
        word[ip].NAME == "<=" ||word[ip].NAME == ">" || word[ip].NAME == ">=") {
        ret = "j" + word[ip].NAME;
        ip++;
    }
    else {
        synErr = true;
        std::cout << "syntax error in Relationship\n";
    }
}


