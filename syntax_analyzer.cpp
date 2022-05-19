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

void SyntaxAnalyzer::ProcedureDeclare() {   //����˵������
    cntPro++;           // ����һ��
    if (cntPro > 3) {   // ������3��procedure
        smnErr = true;
        std::cout << "procedure depth out of 3\n";
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

        SymbolNode snode;													// snode: record symbol
        snode.kind = "PROCEDURE";											// kind
        Identifier(snode.name);										    // name
        snode.para1 = cntPro - 1;											// para1: LEVEL=cntPro-1 ��Ϊ��һ���ж��Ѿ���ǰ����+1���������������������һ��
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

void SyntaxAnalyzer::AssignmentStatement() {    //��ֵ���
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
        // Todo: emit ��Ԫʽ

    }
    else {
        synErr = true;
        std::cout << "syntax error in AssignmentStatement\n";
    }
}

void SyntaxAnalyzer::ConditionStatement() { //�������
    if (word[ip].NAME == "if") {
        ip++;
        Condition();

        // Todo: emit ��Ԫʽ �·�statement֮����

        if (word[ip].NAME == "then") {
            ip++;
            Statement();

            // Todo: ����
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
        Condition();

        // Todo: emit��Ԫʽ���·�����

        if (word[ip].NAME == "do") {
            ip++;
            Statement();

            // Todo: ����
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
        if (pos < 0)    smnErr = true;

        // Todo: ��Ԫʽ�ͷ���
        SymbolNode tmp_n = symbolTbl.getNode(pos);

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
            }
            SymbolNode tmp_n = symbolTbl.getNode(pos);
            // Todo: emit ��Ԫʽ


            while (word[ip].NAME == ",") {
                ip++;

                std::string temp_id;
                Identifier(temp_id);

                int tpos = symbolTbl.checkPos(temp_id, { 2 }, cntPro);
                if (tpos < 0) {
                    smnErr = true;
                }
                SymbolNode temp_n = symbolTbl.getNode(tpos);
                // Todo: emit ��Ԫʽ

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
            }
            SymbolNode tmp_n = symbolTbl.getNode(pos);
            // Todo: emit ��Ԫʽ

            while (word[ip].NAME == ",") {
                ip++;

                std::string temp_id;
                Identifier(temp_id);

                int tpos = symbolTbl.checkPos(temp_id, { 1, 2 }, cntPro);
                if (tpos < 0) {
                    smnErr = true;
                }
                SymbolNode temp_n = symbolTbl.getNode(tpos);
                // Todo: emit ��Ԫʽ

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

void SyntaxAnalyzer::Expression() { //���ʽ
    if (word[ip].NAME == "+" || word[ip].NAME == "-") {
        ip++;
    }
    Item();

    //Todo: ��ȡ���ʽ���� emit��Ԫʽ

    while (word[ip].NAME == "+" || word[ip].NAME == "-") {
        AddSub();
        Item();

        //Todo: ��ȡ���ʽ���� emit��Ԫʽ

    }
}

void SyntaxAnalyzer::Condition() { //����
    if (word[ip].NAME == "odd") {
        ip++;
        Expression();
        //Todo: emit��odd��ص���Ԫʽ
    }
    else {
        Expression();
        Relationship();
        Expression();

        //Todo: ���ݹ�ϵ�����emit��Ԫʽ ��Ҫ��ȡRelationship()ʶ��������
    }
}

void SyntaxAnalyzer::Item() {   //��
    Factor();
    while (word[ip].NAME=="*"|| word[ip].NAME == "/") {
        MulDiv();
        Factor();

        //Todo: ��ȡ * or / ���� emit��Ԫʽ

    }
}

void SyntaxAnalyzer::Factor() { //����
    if ((word[ip].NAME.at(0) <= 122 && word[ip].NAME.at(0) >= 97)) { //��ʶ��
//    if (word[ip].NAME == "IDENT") { //��ʶ��
        std::string tmp_id;
        Identifier(tmp_id);
        //Todo: ��Ԫʽ

    }
    else if ((word[ip].NAME.at(0) <= 57 && word[ip].NAME.at(0) >= 48)) { //�޷�������
//    else if(word[ip].NAME == "NUMBER") { //�޷�������
        int tmp_num;
        UnsignedInt(tmp_num);
        //Todo: ��Ԫʽ

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

void SyntaxAnalyzer::AddSub() { //�Ӽ������
    if (word[ip].NAME == "+" || word[ip].NAME == "-")   ip++;
    else    std::cout << "syntax error in AddSub\n";
}

void SyntaxAnalyzer::MulDiv() {   //�˳������
    if (word[ip].NAME == "*" || word[ip].NAME == "/")   ip++;
    else    std::cout << "syntax error in MulDiv\n";
}

void SyntaxAnalyzer::Relationship() {   //��ϵ�����
    if (word[ip].NAME == "=" || word[ip].NAME == "#" || word[ip].NAME=="<" ||
        word[ip].NAME == "<=" ||word[ip].NAME == ">" || word[ip].NAME == ">=") {
        ip++;
    }
    else {
        synErr = true;
        std::cout << "syntax error in Relationship\n";
    }
}





////���code����������
//void SyntaxAnalyzer::codeAdd(std::string str,int lev,int place,std::string con) {
//    code[codeIdx].funcCode = str;
//    code[codeIdx].levelDiff = lev;
//    code[codeIdx].displacement = place;
//    code[codeIdx].content = con;
//    codeIdx++;
//}
////���code��������
//void SyntaxAnalyzer::codeAdd(std::string str, int lev, int place) {
//    code[codeIdx].funcCode = str;
//    code[codeIdx].levelDiff = lev;
//    code[codeIdx].displacement = place;
//    codeIdx++;
//}
