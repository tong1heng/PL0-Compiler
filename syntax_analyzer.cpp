#include <iostream>
#include "syntax_analyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer() {
    ip = 0;
    procedureDepth = 0;
    dx = 3;

    syntaxError = false;
    semanticError = false;
}

SyntaxAnalyzer::~SyntaxAnalyzer() {

}

void SyntaxAnalyzer::Run() {
    Program();
    std::cout << "\n========================================\n";
    if(syntaxError) {
        std::cout << "syntax analysis error!!!\n";
    }
    else if(semanticError) {
        std::cout << "semantic analysis error!!!\n";
    }
    else {
        std::cout << "syntax and semantic analysis success!!!\n";
    }
    std::cout << "========================================\n\n";
}

void SyntaxAnalyzer::Show() {
    symbolTable.PrintSymbolTable();
    quaternary.Print();
    targetCode.PrintTargetCode();

//    std::cout << incmpltCode.size() << "\n";
}

void SyntaxAnalyzer::SetWord(std::vector<Word>& w) {    // 获取词法分析的结果
    word = w;
}

std::vector<TargetCodeNode> &SyntaxAnalyzer::GetTargetCode() {  // 返回生成的目标代码
    return targetCode.GetTargetCode();
}

void SyntaxAnalyzer::Program() {   // 程序
    SubProgram(-1);

    if (word[ip].NAME == ".") {
        ip++;
//        std::cout << "syntax and semantic analysis success!!!\n";
    }
    else {
        syntaxError = true;
        std::cout << ip << ": " << word[ip].NAME << "\n";
        std::cout << "syntax error\n";
    }
}

void SyntaxAnalyzer::SubProgram(int px) { // px标识当前子程序的过程名p在符号表中的位置下标；-1标识main
    int nxq = quaternary.NextCodeId();
    int varNum = 0, bpIdx = targetCode.GetSize();	// 记录第一条jmp的代码行号

    if(px == -1) {
        // 生成四元式
        Code code;
        code.op = "j";
        code.arg1 = "-";
        code.arg2 = "-";
        code.result = "";                   // jmp 等待反填
        quaternary.Emit(code);
        // 生成目标代码
        targetCode.Emit(6, 0, 0);   // jmp 0 a 等待反填
    }

    dx = 3;

    if (word[ip].NAME == "const") ConstDeclare();           // 常量说明部分
    if (word[ip].NAME == "var") {   // 变量说明部分
        int varPre = symbolTable.GetSize();
        VarDeclare();
        varNum = symbolTable.GetSize() - varPre;
    }
    if (word[ip].NAME == "procedure") ProcedureDeclare();   // 过程说明部分

    // Done: Emit 四元式 & 目标代码
    if(px == -1) {  // 子程序main
        quaternary.BackPatch(nxq, quaternary.NextCodeId());     // 反填第一条jmp语句
        targetCode.BackPatch(bpIdx, targetCode.GetSize());   // 反填第一条jmp语句
    }
    else {  // 非main子程序
//        symbolTable.SetPara2(px, quaternary.NextCodeId());   // 反填符号表中的位置
        symbolTable.SetPara2(px, targetCode.GetSize());   // 反填符号表中的位置
//        // incomplete code
//        SymbolNode tmp_n = symbolTable.GetNode(px);
//        for (int i = 0; i < incmpltCode.size(); i++) {
//            if (incmpltCode[i].second == tmp_n) {
//                targetCode.BackPatch(incmpltCode[i].first, tmp_n.para2);
//            }
//        }
    }

    targetCode.Emit(5, 0, 3 + varNum);             // INT 0 a 在栈中开辟数据区

    Statement();                                            // 语句

    // Done: Emit return code after each procedure
    // 四元式
    Code retCode;
    retCode.op = "ret";
    retCode.arg1 = "-";
    retCode.arg2 = "-";
    retCode.result = "-";
    quaternary.Emit(retCode);
    // 目标代码
    targetCode.Emit(8, 0, 0);                       // OPR 0 0 子程序返回
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
            syntaxError = true;
            std::cout << "syntax error in ConstDeclare\n";
        }
    }
    else {
        syntaxError = true;
        std::cout << "syntax error in ConstDeclare\n";
    }
}

void SyntaxAnalyzer::VarDeclare() { //变量说明语句
    SymbolNode snode;													// snode: record symbol
    snode.kind = "VARIABLE";											// kind
    snode.para1 = procedureDepth;										// para1: LEVEL = procedureDepth

    if (word[ip].NAME == "var") {
        ip++;

        Identifier(snode.name);
        snode.para2 = dx++;												// para2: adr
        bool res = symbolTable.Insert(snode, procedureDepth);	// Insert to the table
        if (!res) {
            semanticError = true;
            return ;
        }

        while (word[ip].NAME == ",") {
            ip++;

            Identifier(snode.name);
            snode.para2 = dx++;											    // para2: adr
            bool res = symbolTable.Insert(snode, procedureDepth);   // Insert to the table
            if (!res) {
                semanticError = true;
                return ;
            }
        }
        if (word[ip].NAME == ";")
            ip++;
        else {
            syntaxError = true;
            std::cout << "syntax error in VarDeclare\n";
        }
    }
    else {
        syntaxError = true;
        std::cout << "syntax error in VarDeclare\n";
    }
}

void SyntaxAnalyzer::ProcedureDeclare() {   //过程说明部分
    procedureDepth++;           // 创建一层
    if (procedureDepth > 3) {   // 不大于3层procedure
        semanticError = true;
        std::cout << "procedure depth out of 3\n";
        return ;
    }

    ProcedureHead();
    int pos = symbolTable.GetSize() - 1;	// 当前procedure标识符在符号表中的位置
    SubProgram(pos);

    if (word[ip].NAME == ";") {
        procedureDepth--;       // 分号完成一层
        ip++;
        while (word[ip].NAME == "procedure") {
            ProcedureDeclare();
        }
    }
    else {
        syntaxError = true;
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
        UnsignedInt(snode.para1);                                   // para1: val
        snode.para2 = procedureDepth;                                   // para2: level
        bool res = symbolTable.Insert(snode, procedureDepth);   // Insert to the table
        if (!res) {
            semanticError = true;
            return ;
        }
    }
    else {
        syntaxError = true;
        std::cout << "syntax error in ConstDefine\n";
    }
}

void SyntaxAnalyzer::Identifier(std::string &id) { //标识符
    if(word[ip].SYM == "IDENT") {
        id = word[ip].NAME;
        ip++;
    }
    else {
        syntaxError = true;
        std::cout << "syntax error in Identifier\n";
    }
}

void SyntaxAnalyzer::UnsignedInt(int &num) {    //无符号整数
    if(word[ip].SYM == "NUMBER") {
        num = stoi(word[ip].NAME);
        ip++;
    }
    else {
        syntaxError = true;
        std::cout << "syntax error in Identifier\n";
    }
}

void SyntaxAnalyzer::ProcedureHead() {  //过程首部
    if (word[ip].NAME == "procedure") {
        ip++;

        SymbolNode snode;											// snode: record symbol
        snode.kind = "PROCEDURE";									// kind
        Identifier(snode.name);								    // name
        snode.para1 = procedureDepth - 1;							// para1: LEVEL=procedureDepth-1
        snode.para2 = 0;											// para2: 0 indicates it doesn't have addr yet.

        bool res = symbolTable.Insert(snode, procedureDepth);	// Insert to the table
        if (!res) {
            semanticError = true;
            return ;
        }

        if (word[ip].NAME == ";")
            ip++;
        else {
            syntaxError = true;
            std::cout << "syntax error in ProcedureHead\n";
        }
    }
    else {
        syntaxError = true;
        std::cout << "syntax error in ProcedureHead\n";
    }
}

void SyntaxAnalyzer::AssignmentStatement() {    // 赋值语句
    std::string tmp_id;
    Code code;

    Identifier(tmp_id);

    if (word[ip].NAME == ":=") {
        ip++;
        code.op = "=";
        Expression(code.arg1);

        int pos = symbolTable.CheckPos(tmp_id, {2}, procedureDepth);
        if (pos < 0) {
            semanticError = true;
            return ;
        }

        // Done: Emit 四元式 & 目标代码
        SymbolNode sym = symbolTable.GetNode(pos);
        code.arg2 = "-";
        code.result = sym.name;
        quaternary.Emit(code);
        targetCode.Emit(3, procedureDepth - sym.para1, sym.para2);  // STO lev-para1 para2
    }
    else {
        syntaxError = true;
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

        // Done: Emit 四元式 下方statement反填
        int tnxq = quaternary.NextCodeId();
        quaternary.Emit(trueCode);
        int fnxq = quaternary.NextCodeId();
        quaternary.Emit(falseCode);
        // Done: Emit 目标代码
        int bpIdx = targetCode.GetSize();
        targetCode.Emit(7, 0, 0);   // JPC 0 a 下方statement之后反填a

        if (word[ip].NAME == "then") {
            ip++;

            // Done: 反填四元式真出口
            quaternary.BackPatch(tnxq, quaternary.NextCodeId());

            Statement();

            // Done: 反填四元式假出口
            quaternary.BackPatch(fnxq,quaternary.NextCodeId());

            // Done: 反填目标代码
            targetCode.BackPatch(bpIdx, targetCode.GetSize());
        }
        else {
            syntaxError = true;
            std::cout << "syntax error in ConditionStatement\n";
        }
    }
    else {
        syntaxError = true;
        std::cout << "syntax error in ConditionStatement\n";
    }
}

void SyntaxAnalyzer::DoWhileStatement() {    // 当型循环语句
    if (word[ip].NAME == "while") {
        ip++;
        int qConIdx = quaternary.NextCodeId();
        int tConIdx = targetCode.GetSize();
        Code trueCode, falseCode;
        Condition(trueCode);
        falseCode.op = "j";
        falseCode.arg1 = "-";
        falseCode.arg2 = "-";

        // Done: Emit 四元式 下方statement反填
        int tnxq = quaternary.NextCodeId();
        quaternary.Emit(trueCode);
        int fnxq = quaternary.NextCodeId();
        quaternary.Emit(falseCode);

        // Done: Emit 目标代码
        int bpIdx = targetCode.GetSize();
        targetCode.Emit(7, 0, 0);   // JPC 0 a 下方statement之后反填a

        if (word[ip].NAME == "do") {
            ip++;

            // Done: 反填真出口
            quaternary.BackPatch(tnxq, quaternary.NextCodeId());

            Statement();

            // Done: 跳转至条件语句 反填假出口
            Code jmpCode;
            jmpCode.op = "j";
            jmpCode.arg1 = "-";
            jmpCode.arg2 = "-";
            jmpCode.result = std::to_string(qConIdx);
            quaternary.Emit(jmpCode);
            quaternary.BackPatch(fnxq,quaternary.NextCodeId());

            // Done: 跳转至条件语句 反填目标代码
            targetCode.Emit(6, 0, tConIdx);    // JMP 0 a 跳转到条件判断语句
            targetCode.BackPatch(bpIdx, targetCode.GetSize());
        }
        else {
            syntaxError = true;
            std::cout << "syntax error in DoWhileStatement\n";
        }
    }
    else {
        syntaxError = true;
        std::cout << "syntax error in DoWhileStatement\n";
    }
}

void SyntaxAnalyzer::CallStatement() {    // 过程调用语句
    if (word[ip].NAME == "call") {
        ip++;

        std::string tmp_id;
        Identifier(tmp_id);

        int pos = symbolTable.CheckPos(tmp_id, {3}, procedureDepth);
        if (pos < 0) {
            semanticError = true;
            return ;
        }

        SymbolNode tmp_n = symbolTable.GetNode(pos);
//        // incomplete code
//        if (tmp_n.para2 == 0) {  // 当前call的procedure还没有反填第一句位置，此条code的a等待tmp_n信息反填
//            incmpltCode.push_back(std::make_pair(targetCode.GetSize(), tmp_n));
//        }

        // Done: Emit 四元式 & 目标代码
        Code code;
        code.op = "call";
        code.arg1 = "-";
        code.arg2 = "-";
        code.result = tmp_n.name;
        quaternary.Emit(code);
        targetCode.Emit(4, procedureDepth - tmp_n.para1, tmp_n.para2);  // CAL lev-para1 para2
    }
    else {
        syntaxError = true;
        std::cout << "syntax error in CallStatement\n";
    }
}

void SyntaxAnalyzer::ReadStatement() {  // 读语句
    if (word[ip].NAME == "read") {
        ip++;
        if (word[ip].NAME == "(") {
            ip++;

            std::string tmp_id;
            Identifier(tmp_id);

            int pos = symbolTable.CheckPos(tmp_id, {2}, procedureDepth);
            if (pos < 0) {
                semanticError = true;
                return ;
            }

            SymbolNode tmp_n = symbolTable.GetNode(pos);
            // Done: Emit 四元式 & 目标代码
            Code code;
            code.op = "read";
            code.arg1 = "-";
            code.arg2 = "-";
            code.result = tmp_n.name;
            quaternary.Emit(code);
            targetCode.Emit(8, 0, 13);  // OPR 0 13 for read
            targetCode.Emit(3, procedureDepth - tmp_n.para1, tmp_n.para2);  // STO l a

            while (word[ip].NAME == ",") {
                ip++;

                std::string temp_id;
                Identifier(temp_id);

                int tpos = symbolTable.CheckPos(temp_id, {2}, procedureDepth);
                if (tpos < 0) {
                    semanticError = true;
                    return ;
                }

                SymbolNode temp_n = symbolTable.GetNode(tpos);
                // Done: Emit 四元式 & 目标代码
                Code tcode;
                tcode.op = "read";
                tcode.arg1 = "-";
                tcode.arg2 = "-";
                tcode.result = temp_n.name;
                quaternary.Emit(tcode);
                targetCode.Emit(8, 0, 13);  // OPR 0 13 for read
                targetCode.Emit(3, procedureDepth - temp_n.para1, temp_n.para2);  // STO
            }
            if (word[ip].NAME == ")")   ip++;
            else {
                syntaxError = true;
                std::cout << "syntax error in ReadStatement\n";
            }
        }
        else {
            syntaxError = true;
            std::cout << "syntax error in ReadStatement\n";
        }
    }
    else {
        syntaxError = true;
        std::cout << "syntax error in ReadStatement\n";
    }
}

void SyntaxAnalyzer::WriteStatement() { // 写语句
    if (word[ip].NAME == "write") {
        ip++;
        if (word[ip].NAME == "(") {
            ip++;

            std::string tmp_id;
            Identifier(tmp_id);

            int pos = symbolTable.CheckPos(tmp_id, {1, 2}, procedureDepth);
            if (pos < 0) {
                semanticError = true;
                return ;
            }

            SymbolNode tmp_n = symbolTable.GetNode(pos);
            // Done: Emit 四元式
            Code code;
            code.op = "write";
            code.arg1 = "-";
            code.arg2 = "-";
            code.result = tmp_n.name;
            quaternary.Emit(code);
            // Done: Emit 目标代码
            if (tmp_n.kind == "CONSTANT") {
                targetCode.Emit(1, 0, tmp_n.para1);  // LIT
            }
            else if (tmp_n.kind == "VARIABLE") {
                targetCode.Emit(2, procedureDepth - tmp_n.para1, tmp_n.para2);  // LOD
            }
            targetCode.Emit(8, 0, 14);  // OPR 0 14 for write

            while (word[ip].NAME == ",") {
                ip++;

                std::string temp_id;
                Identifier(temp_id);

                int tpos = symbolTable.CheckPos(temp_id, {1, 2}, procedureDepth);
                if (tpos < 0) {
                    semanticError = true;
                    return ;
                }

                SymbolNode temp_n = symbolTable.GetNode(tpos);
                // Done: Emit 四元式
                Code tcode;
                tcode.op = "write";
                tcode.arg1 = "-";
                tcode.arg2 = "-";
                tcode.result = temp_n.name;
                quaternary.Emit(tcode);
                // Done: Emit 目标代码
                if (temp_n.kind == "CONSTANT") {
                    targetCode.Emit(1, 0, temp_n.para1);  // LIT
                }
                else if (temp_n.kind == "VARIABLE") {
                    targetCode.Emit(2, procedureDepth - temp_n.para1, temp_n.para2);  // LOD
                }
                targetCode.Emit(8, 0, 14);  // OPR 0 14 for write
            }
            if (word[ip].NAME == ")")   ip++;
            else {
                syntaxError = true;
                std::cout << "syntax error in WriteStatement\n";
            }
        }
        else {
            syntaxError = true;
            std::cout << "syntax error in WriteStatement\n";
        }
    }
    else {
        syntaxError = true;
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
            syntaxError = true;
            std::cout << "syntax error in CompoundStatement\n";
        }
    }
    else {
        syntaxError = true;
        std::cout << "syntax error in CompoundStatement\n";
    }
}

void SyntaxAnalyzer::Expression(std::string &ret) { // 表达式
    bool isMinus = false;
    Code code;

    if (word[ip].NAME == "+" || word[ip].NAME == "-") {
        isMinus = true;         // 取负的"-"
        ip++;
    }

    Item(code.arg1);
//    ret = code.arg1;

    if(isMinus) {   // 取负
        // Done: 如果取负 Emit 四元式
        code.arg2 = "-";
        code.op = "uminus"; // uminus 和 - 进行区分
        code.result = quaternary.GenTemp();
        quaternary.Emit(code);
        code.arg1 = code.result;
        // Done: 如果取负 Emit 目标代码
        targetCode.Emit(8, 0, 11);  // OPR 0 11 for uminus
    }

    while (word[ip].NAME == "+" || word[ip].NAME == "-") {
        code.op = word[ip].NAME;
        AddSub();
        Item(code.arg2);

        // Done: 获取表达式符号 Emit 四元式
        code.result = quaternary.GenTemp();
        quaternary.Emit(code);
        code.arg1 = code.result;
        // Done: Emit 目标代码
        if(code.op == "+") targetCode.Emit(8, 0, 1);
        if(code.op == "-") targetCode.Emit(8, 0, 2);
    }
    ret = code.arg1;
}

void SyntaxAnalyzer::Condition(Code &code) { // 条件
    if (word[ip].NAME == "odd") {
        ip++;
        Expression(code.arg1);
        // Done: 根据关系运算符emit四元式 获取odd
        code.op = "odd";
        // Done: Emit 目标代码
        targetCode.Emit(8, 0, 12);      // OPR 0 12 for odd
    }
    else {
        Expression(code.arg1);
        Relationship(code.op);              // Done: 根据关系运算符emit四元式 获取Relationship()识别的运算符
        Expression(code.arg2);

        // Done: Emit 目标代码
        if (code.op == "j=") {
            targetCode.Emit(8, 0, 5);
        }
        else if (code.op == "j#") {
            targetCode.Emit(8, 0, 6);
        }
        else if (code.op == "j<") {
            targetCode.Emit(8, 0, 7);
        }
        else if (code.op == "j<=") {
            targetCode.Emit(8, 0, 8);
        }
        else if (code.op == "j>") {
            targetCode.Emit(8, 0, 9);
        }
        else if (code.op == "j>=") {
            targetCode.Emit(8, 0, 10);
        }
    }
}

void SyntaxAnalyzer::Item(std::string &ret) {   //项
    Code code;

    Factor(code.arg1);
//    ret = code.arg1;

    while (word[ip].NAME == "*" || word[ip].NAME == "/") {
        // Done: 获取乘除号 emit四元式
        code.op = word[ip].NAME;
        MulDiv();
        Factor(code.arg2);
        code.result = quaternary.GenTemp();
        quaternary.Emit(code);
        code.arg1 = code.result;
        // Done: Emit 目标代码
        if(code.op == "*") targetCode.Emit(8, 0, 3);
        if(code.op == "/") targetCode.Emit(8, 0, 4);
    }
    ret = code.arg1;
}

void SyntaxAnalyzer::Factor(std::string &ret) { // 因子
//    if ((word[ip].NAME.at(0) <= 122 && word[ip].NAME.at(0) >= 97)) { // 标识符
    if (word[ip].SYM == "IDENT") { // 标识符
        std::string tmp_id;
        Identifier(tmp_id);

        int pos = symbolTable.CheckPos(tmp_id, {1, 2}, procedureDepth);
        if (pos < 0) {
            semanticError = true;
            return ;
        }

        // Done: 四元式
        ret = tmp_id;
        // Done: Emit 目标代码
        SymbolNode sym = symbolTable.GetNode(pos);
        if (sym.kind == "CONSTANT") {
            targetCode.Emit(1, 0, sym.para1);  // LIT 0 para1
        }
        else if (sym.kind == "VARIABLE") {
            targetCode.Emit(2, procedureDepth - sym.para1, sym.para2);  // LOD lev-para1 para2
        }
    }
//    else if ((word[ip].NAME.at(0) <= 57 && word[ip].NAME.at(0) >= 48)) { // 无符号整数
    else if(word[ip].SYM == "NUMBER") { // 无符号整数
        int tmp_num;
        UnsignedInt(tmp_num);
        // Done: 四元式
        ret = std::to_string(tmp_num);
        // Done: Emit 目标代码
        targetCode.Emit(1, 0, tmp_num);  // LIT 0 整数
    }
    else if (word[ip].NAME == "(") {
        Expression(ret);
        if (word[ip].NAME == ")")   ip++;
        else {
            syntaxError = true;
            std::cout << "syntax error in Factor\n";
        }
    }
    else {
        syntaxError = true;
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
        syntaxError = true;
        std::cout << "syntax error in Relationship\n";
    }
}


