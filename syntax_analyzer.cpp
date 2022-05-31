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

void SyntaxAnalyzer::SetWord(std::vector<Word>& w) {    // ��ȡ�ʷ������Ľ��
    word = w;
}

std::vector<TargetCodeNode> &SyntaxAnalyzer::GetTargetCode() {  // �������ɵ�Ŀ�����
    return targetCode.GetTargetCode();
}

void SyntaxAnalyzer::Program() {   // ����
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

void SyntaxAnalyzer::SubProgram(int px) { // px��ʶ��ǰ�ӳ���Ĺ�����p�ڷ��ű��е�λ���±ꣻ-1��ʶmain
    int nxq = quaternary.NextCodeId();
    int varNum = 0, bpIdx = targetCode.GetSize();	// ��¼��һ��jmp�Ĵ����к�

    if(px == -1) {
        // ������Ԫʽ
        Code code;
        code.op = "j";
        code.arg1 = "-";
        code.arg2 = "-";
        code.result = "";                   // jmp �ȴ�����
        quaternary.Emit(code);
        // ����Ŀ�����
        targetCode.Emit(6, 0, 0);   // jmp 0 a �ȴ�����
    }

    dx = 3;

    if (word[ip].NAME == "const") ConstDeclare();           // ����˵������
    if (word[ip].NAME == "var") {   // ����˵������
        int varPre = symbolTable.GetSize();
        VarDeclare();
        varNum = symbolTable.GetSize() - varPre;
    }
    if (word[ip].NAME == "procedure") ProcedureDeclare();   // ����˵������

    // Done: Emit ��Ԫʽ & Ŀ�����
    if(px == -1) {  // �ӳ���main
        quaternary.BackPatch(nxq, quaternary.NextCodeId());     // �����һ��jmp���
        targetCode.BackPatch(bpIdx, targetCode.GetSize());   // �����һ��jmp���
    }
    else {  // ��main�ӳ���
//        symbolTable.SetPara2(px, quaternary.NextCodeId());   // ������ű��е�λ��
        symbolTable.SetPara2(px, targetCode.GetSize());   // ������ű��е�λ��
//        // incomplete code
//        SymbolNode tmp_n = symbolTable.GetNode(px);
//        for (int i = 0; i < incmpltCode.size(); i++) {
//            if (incmpltCode[i].second == tmp_n) {
//                targetCode.BackPatch(incmpltCode[i].first, tmp_n.para2);
//            }
//        }
    }

    targetCode.Emit(5, 0, 3 + varNum);             // INT 0 a ��ջ�п���������

    Statement();                                            // ���

    // Done: Emit return code after each procedure
    // ��Ԫʽ
    Code retCode;
    retCode.op = "ret";
    retCode.arg1 = "-";
    retCode.arg2 = "-";
    retCode.result = "-";
    quaternary.Emit(retCode);
    // Ŀ�����
    targetCode.Emit(8, 0, 0);                       // OPR 0 0 �ӳ��򷵻�
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
            syntaxError = true;
            std::cout << "syntax error in ConstDeclare\n";
        }
    }
    else {
        syntaxError = true;
        std::cout << "syntax error in ConstDeclare\n";
    }
}

void SyntaxAnalyzer::VarDeclare() { //����˵�����
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

void SyntaxAnalyzer::ProcedureDeclare() {   //����˵������
    procedureDepth++;           // ����һ��
    if (procedureDepth > 3) {   // ������3��procedure
        semanticError = true;
        std::cout << "procedure depth out of 3\n";
        return ;
    }

    ProcedureHead();
    int pos = symbolTable.GetSize() - 1;	// ��ǰprocedure��ʶ���ڷ��ű��е�λ��
    SubProgram(pos);

    if (word[ip].NAME == ";") {
        procedureDepth--;       // �ֺ����һ��
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

void SyntaxAnalyzer::Identifier(std::string &id) { //��ʶ��
    if(word[ip].SYM == "IDENT") {
        id = word[ip].NAME;
        ip++;
    }
    else {
        syntaxError = true;
        std::cout << "syntax error in Identifier\n";
    }
}

void SyntaxAnalyzer::UnsignedInt(int &num) {    //�޷�������
    if(word[ip].SYM == "NUMBER") {
        num = stoi(word[ip].NAME);
        ip++;
    }
    else {
        syntaxError = true;
        std::cout << "syntax error in Identifier\n";
    }
}

void SyntaxAnalyzer::ProcedureHead() {  //�����ײ�
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

void SyntaxAnalyzer::AssignmentStatement() {    // ��ֵ���
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

        // Done: Emit ��Ԫʽ & Ŀ�����
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

void SyntaxAnalyzer::ConditionStatement() { //�������
    if (word[ip].NAME == "if") {
        ip++;

        Code trueCode, falseCode;
        Condition(trueCode);
        falseCode.op = "j";
        falseCode.arg1 = "-";
        falseCode.arg2 = "-";

        // Done: Emit ��Ԫʽ �·�statement����
        int tnxq = quaternary.NextCodeId();
        quaternary.Emit(trueCode);
        int fnxq = quaternary.NextCodeId();
        quaternary.Emit(falseCode);
        // Done: Emit Ŀ�����
        int bpIdx = targetCode.GetSize();
        targetCode.Emit(7, 0, 0);   // JPC 0 a �·�statement֮����a

        if (word[ip].NAME == "then") {
            ip++;

            // Done: ������Ԫʽ�����
            quaternary.BackPatch(tnxq, quaternary.NextCodeId());

            Statement();

            // Done: ������Ԫʽ�ٳ���
            quaternary.BackPatch(fnxq,quaternary.NextCodeId());

            // Done: ����Ŀ�����
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

void SyntaxAnalyzer::DoWhileStatement() {    // ����ѭ�����
    if (word[ip].NAME == "while") {
        ip++;
        int qConIdx = quaternary.NextCodeId();
        int tConIdx = targetCode.GetSize();
        Code trueCode, falseCode;
        Condition(trueCode);
        falseCode.op = "j";
        falseCode.arg1 = "-";
        falseCode.arg2 = "-";

        // Done: Emit ��Ԫʽ �·�statement����
        int tnxq = quaternary.NextCodeId();
        quaternary.Emit(trueCode);
        int fnxq = quaternary.NextCodeId();
        quaternary.Emit(falseCode);

        // Done: Emit Ŀ�����
        int bpIdx = targetCode.GetSize();
        targetCode.Emit(7, 0, 0);   // JPC 0 a �·�statement֮����a

        if (word[ip].NAME == "do") {
            ip++;

            // Done: ���������
            quaternary.BackPatch(tnxq, quaternary.NextCodeId());

            Statement();

            // Done: ��ת��������� ����ٳ���
            Code jmpCode;
            jmpCode.op = "j";
            jmpCode.arg1 = "-";
            jmpCode.arg2 = "-";
            jmpCode.result = std::to_string(qConIdx);
            quaternary.Emit(jmpCode);
            quaternary.BackPatch(fnxq,quaternary.NextCodeId());

            // Done: ��ת��������� ����Ŀ�����
            targetCode.Emit(6, 0, tConIdx);    // JMP 0 a ��ת�������ж����
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

void SyntaxAnalyzer::CallStatement() {    // ���̵������
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
//        if (tmp_n.para2 == 0) {  // ��ǰcall��procedure��û�з����һ��λ�ã�����code��a�ȴ�tmp_n��Ϣ����
//            incmpltCode.push_back(std::make_pair(targetCode.GetSize(), tmp_n));
//        }

        // Done: Emit ��Ԫʽ & Ŀ�����
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

void SyntaxAnalyzer::ReadStatement() {  // �����
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
            // Done: Emit ��Ԫʽ & Ŀ�����
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
                // Done: Emit ��Ԫʽ & Ŀ�����
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

void SyntaxAnalyzer::WriteStatement() { // д���
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
            // Done: Emit ��Ԫʽ
            Code code;
            code.op = "write";
            code.arg1 = "-";
            code.arg2 = "-";
            code.result = tmp_n.name;
            quaternary.Emit(code);
            // Done: Emit Ŀ�����
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
                // Done: Emit ��Ԫʽ
                Code tcode;
                tcode.op = "write";
                tcode.arg1 = "-";
                tcode.arg2 = "-";
                tcode.result = temp_n.name;
                quaternary.Emit(tcode);
                // Done: Emit Ŀ�����
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
            syntaxError = true;
            std::cout << "syntax error in CompoundStatement\n";
        }
    }
    else {
        syntaxError = true;
        std::cout << "syntax error in CompoundStatement\n";
    }
}

void SyntaxAnalyzer::Expression(std::string &ret) { // ���ʽ
    bool isMinus = false;
    Code code;

    if (word[ip].NAME == "+" || word[ip].NAME == "-") {
        isMinus = true;         // ȡ����"-"
        ip++;
    }

    Item(code.arg1);
//    ret = code.arg1;

    if(isMinus) {   // ȡ��
        // Done: ���ȡ�� Emit ��Ԫʽ
        code.arg2 = "-";
        code.op = "uminus"; // uminus �� - ��������
        code.result = quaternary.GenTemp();
        quaternary.Emit(code);
        code.arg1 = code.result;
        // Done: ���ȡ�� Emit Ŀ�����
        targetCode.Emit(8, 0, 11);  // OPR 0 11 for uminus
    }

    while (word[ip].NAME == "+" || word[ip].NAME == "-") {
        code.op = word[ip].NAME;
        AddSub();
        Item(code.arg2);

        // Done: ��ȡ���ʽ���� Emit ��Ԫʽ
        code.result = quaternary.GenTemp();
        quaternary.Emit(code);
        code.arg1 = code.result;
        // Done: Emit Ŀ�����
        if(code.op == "+") targetCode.Emit(8, 0, 1);
        if(code.op == "-") targetCode.Emit(8, 0, 2);
    }
    ret = code.arg1;
}

void SyntaxAnalyzer::Condition(Code &code) { // ����
    if (word[ip].NAME == "odd") {
        ip++;
        Expression(code.arg1);
        // Done: ���ݹ�ϵ�����emit��Ԫʽ ��ȡodd
        code.op = "odd";
        // Done: Emit Ŀ�����
        targetCode.Emit(8, 0, 12);      // OPR 0 12 for odd
    }
    else {
        Expression(code.arg1);
        Relationship(code.op);              // Done: ���ݹ�ϵ�����emit��Ԫʽ ��ȡRelationship()ʶ��������
        Expression(code.arg2);

        // Done: Emit Ŀ�����
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

void SyntaxAnalyzer::Item(std::string &ret) {   //��
    Code code;

    Factor(code.arg1);
//    ret = code.arg1;

    while (word[ip].NAME == "*" || word[ip].NAME == "/") {
        // Done: ��ȡ�˳��� emit��Ԫʽ
        code.op = word[ip].NAME;
        MulDiv();
        Factor(code.arg2);
        code.result = quaternary.GenTemp();
        quaternary.Emit(code);
        code.arg1 = code.result;
        // Done: Emit Ŀ�����
        if(code.op == "*") targetCode.Emit(8, 0, 3);
        if(code.op == "/") targetCode.Emit(8, 0, 4);
    }
    ret = code.arg1;
}

void SyntaxAnalyzer::Factor(std::string &ret) { // ����
//    if ((word[ip].NAME.at(0) <= 122 && word[ip].NAME.at(0) >= 97)) { // ��ʶ��
    if (word[ip].SYM == "IDENT") { // ��ʶ��
        std::string tmp_id;
        Identifier(tmp_id);

        int pos = symbolTable.CheckPos(tmp_id, {1, 2}, procedureDepth);
        if (pos < 0) {
            semanticError = true;
            return ;
        }

        // Done: ��Ԫʽ
        ret = tmp_id;
        // Done: Emit Ŀ�����
        SymbolNode sym = symbolTable.GetNode(pos);
        if (sym.kind == "CONSTANT") {
            targetCode.Emit(1, 0, sym.para1);  // LIT 0 para1
        }
        else if (sym.kind == "VARIABLE") {
            targetCode.Emit(2, procedureDepth - sym.para1, sym.para2);  // LOD lev-para1 para2
        }
    }
//    else if ((word[ip].NAME.at(0) <= 57 && word[ip].NAME.at(0) >= 48)) { // �޷�������
    else if(word[ip].SYM == "NUMBER") { // �޷�������
        int tmp_num;
        UnsignedInt(tmp_num);
        // Done: ��Ԫʽ
        ret = std::to_string(tmp_num);
        // Done: Emit Ŀ�����
        targetCode.Emit(1, 0, tmp_num);  // LIT 0 ����
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
        syntaxError = true;
        std::cout << "syntax error in Relationship\n";
    }
}


