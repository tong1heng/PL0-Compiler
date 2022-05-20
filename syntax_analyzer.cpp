#include <iostream>
#include "syntax_analyzer.h"

extern Word *word;                 //ʶ��ĵ���
extern int wordIndex;              //ʶ��ĵ���������i.e. ʶ��ĵ���������

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

void SyntaxAnalyzer::Program() {   //����
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

    if (word[ip].NAME == "const") ConstDeclare();           //����˵������
    if (word[ip].NAME == "var") VarDeclare();               //����˵������
    if (word[ip].NAME == "procedure") ProcedureDeclare();   //����˵������
    Statement();                                            //���

    //Todo: emit ��Ԫʽ

}

void SyntaxAnalyzer::ConstDeclare() {   //����˵������
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

void SyntaxAnalyzer::VarDeclare() { //����˵�����
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

void SyntaxAnalyzer::ProcedureDeclare() {   //����˵������
    cntPro++;           // ����һ��
    if (cntPro > 3) {   // ������3��procedure
        smnErr = true;
        std::cout << "procedure depth out of 3\n";
        return ;
    }

    ProcedureHead();
    SubProgram();

    if (word[ip].NAME == ";") {
        cntPro--;       // �ֺ����һ��
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

void SyntaxAnalyzer::Statement() {  //���
//    if (isexists(word[ip].NAME))
    if (word[ip+1].NAME == ":=")    //��ֵ���
        AssignmentStatement();
    if (word[ip].NAME == "if")      //�������
        ConditionStatement();
    if (word[ip].NAME == "while")   //����ѭ�����
        DoWhileStatement();
    if (word[ip].NAME == "call")    //���̵������
        CallStatement();
    if (word[ip].NAME == "read")    //�����
        ReadStatement();
    if (word[ip].NAME == "write")   //д���
        WriteStatement();
    if (word[ip].NAME == "begin")   //�������
        CompoundStatement();
}

void SyntaxAnalyzer::ConstDefine() {    //��������
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

void SyntaxAnalyzer::Identifier(std::string &id) { //��ʶ��
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

void SyntaxAnalyzer::UnsignedInt(int &num) {    //�޷�������
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

void SyntaxAnalyzer::ProcedureHead() {  //�����ײ�
    if (word[ip].NAME == "procedure") {
        ip++;

        SymbolNode snode;											// snode: record symbol
        snode.kind = "PROCEDURE";									// kind
        Identifier(snode.name);								    // name
        snode.para1 = cntPro - 1;									// para1: LEVEL=cntPro-1 ��Ϊ��һ���ж��Ѿ���ǰ����+1���������������������һ��
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

void SyntaxAnalyzer::AssignmentStatement() {    //��ֵ���
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

        // Done: emit ��Ԫʽ
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

void SyntaxAnalyzer::ConditionStatement() { //�������
    if (word[ip].NAME == "if") {
        ip++;

        Code trueCode, falseCode;
        Condition(trueCode);
        falseCode.op = "j";
        falseCode.arg1 = "-";
        falseCode.arg2 = "-";
        // Done: emit ��Ԫʽ �·�statement����
        int tnxq = quaternary.NextCodeId();
        quaternary.emit(trueCode);
        int fnxq = quaternary.NextCodeId();
        quaternary.emit(falseCode);

        if (word[ip].NAME == "then") {
            ip++;

            // Done: ���������
            quaternary.BackPatch(tnxq, quaternary.NextCodeId());
            Statement();

            // Done: ����ٳ���
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

void SyntaxAnalyzer::DoWhileStatement() {    //����ѭ�����
    if (word[ip].NAME == "while") {
        ip++;

        Code trueCode, falseCode;
        Condition(trueCode);
        falseCode.op = "j";
        falseCode.arg1 = "-";
        falseCode.arg2 = "-";

        // Done: emit��Ԫʽ���·�statement����
        int tnxq = quaternary.NextCodeId();
        quaternary.emit(trueCode);
        int fnxq = quaternary.NextCodeId();
        quaternary.emit(falseCode);

        if (word[ip].NAME == "do") {
            ip++;

            // Done: ���������
            quaternary.BackPatch(tnxq, quaternary.NextCodeId());
            Statement();

            // Done: ����ٳ���
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

void SyntaxAnalyzer::CallStatement() {    //���̵������
    if (word[ip].NAME == "call") {
        ip++;

        std::string tmp_id;
        Identifier(tmp_id);

        int pos = symbolTbl.checkPos(tmp_id, { 3 }, cntPro);
        if (pos < 0) {
            smnErr = true;
            return ;
        }

        // Done: emit��Ԫʽ
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

void SyntaxAnalyzer::ReadStatement() {  //�����
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
            // Done: emit ��Ԫʽ
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
                // Done: emit ��Ԫʽ
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

void SyntaxAnalyzer::WriteStatement() { //д���
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
            // Done: emit ��Ԫʽ
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
                // Done: emit ��Ԫʽ
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

void SyntaxAnalyzer::CompoundStatement() {  //�������
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

void SyntaxAnalyzer::Expression(std::string &ret) { //���ʽ
    bool isMinus = false;
    Code code;

    if (word[ip].NAME == "+" || word[ip].NAME == "-") {
        isMinus = true;         //ȡ����"-"
        ip++;
    }

    Item(code.arg1);
//    ret = code.arg1;

    //Done: ���ȡ�� emit��Ԫʽ
    if(isMinus) {   //ȡ��
        code.arg2 = "-";
        code.op = "uminus"; // uminus �� - ��������
        code.result = quaternary.NewTemp();
        quaternary.emit(code);
        code.arg1 = code.result;
    }

    while (word[ip].NAME == "+" || word[ip].NAME == "-") {
        code.op = word[ip].NAME;
        AddSub();
        Item(code.arg2);

        //Done: ��ȡ���ʽ���� emit��Ԫʽ
        code.result = quaternary.NewTemp();
        quaternary.emit(code);
        code.arg1 = code.result;
    }
    ret = code.arg1;
}

void SyntaxAnalyzer::Condition(Code &code) { //����
    if (word[ip].NAME == "odd") {
        ip++;
        Expression(code.arg1);
        //Done: emit��odd��ص���Ԫʽ
        code.op = "odd";
    }
    else {
        Expression(code.arg1);
        Relationship(code.op);
        Expression(code.arg2);
        //Done: ���ݹ�ϵ�����emit��Ԫʽ ��Ҫ��ȡRelationship()ʶ��������
    }
}

void SyntaxAnalyzer::Item(std::string &ret) {   //��
    Code code;

    Factor(code.arg1);
//    ret = code.arg1;

    while (word[ip].NAME == "*" || word[ip].NAME == "/") {
        //Done: ��ȡ�˳��� emit��Ԫʽ
        code.op = word[ip].NAME;
        MulDiv();
        Factor(code.arg2);
        code.result = quaternary.NewTemp();
        quaternary.emit(code);
        code.arg1 = code.result;
    }
    ret = code.arg1;
}

void SyntaxAnalyzer::Factor(std::string &ret) { //����
    if ((word[ip].NAME.at(0) <= 122 && word[ip].NAME.at(0) >= 97)) { //��ʶ��
//    if (word[ip].NAME == "IDENT") { //��ʶ��
        std::string tmp_id;
        Identifier(tmp_id);
        //Done: ��Ԫʽ
        ret = tmp_id;
    }
    else if ((word[ip].NAME.at(0) <= 57 && word[ip].NAME.at(0) >= 48)) { //�޷�������
//    else if(word[ip].NAME == "NUMBER") { //�޷�������
        int tmp_num;
        UnsignedInt(tmp_num);
        //Done: ��Ԫʽ
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

void SyntaxAnalyzer::AddSub() { //�Ӽ������
    if (word[ip].NAME == "+" || word[ip].NAME == "-")   ip++;
    else    std::cout << "syntax error in AddSub\n";
}

void SyntaxAnalyzer::MulDiv() {   //�˳������
    if (word[ip].NAME == "*" || word[ip].NAME == "/")   ip++;
    else    std::cout << "syntax error in MulDiv\n";
}

void SyntaxAnalyzer::Relationship(std::string &ret) {   //��ϵ�����
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


