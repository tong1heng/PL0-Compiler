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
    void SetWord(std::vector<Word>& word);          // ��ȡ�ʷ������Ľ��
    std::vector<TargetCodeNode>& GetTargetCode();   // �������ɵ�Ŀ�����

private:
    std::vector<Word> word;             // �ʷ������õ��ĵ���
    int ip;                             // ��ǰ�����ĵ�������

    /* syntax analysis */

//    Node *ASTroot;					// �﷨������
//    std::string _resStr;			    // �﷨��������ַ���
    bool syntaxError;					// �﷨��������

    void Program();                     // ����
    void SubProgram(int px);            // �ӳ���

    void ConstDeclare();                // ����˵������
    void VarDeclare();                  // ����˵������
    void ProcedureDeclare();            // ����˵������
    void Statement();                   // ���

    void ConstDefine();                 // ��������
    void Identifier(std::string &id);   // ��ʶ��
    void UnsignedInt(int &num);         // �޷�������
    void ProcedureHead();               // �����ײ�

    void AssignmentStatement();         // ��ֵ���
    void ConditionStatement();          // �������
    void DoWhileStatement();            // ����ѭ�����
    void CallStatement();               // ���̵������
    void ReadStatement();               // �����
    void WriteStatement();              // д���
    void CompoundStatement();           // �������

    void Expression(std::string &ret);  // ���ʽ
    void Condition(Code &code);         // ����
    void Item(std::string &ret);        // ��
    void Factor(std::string &ret);      // ����
    void AddSub();                      // �Ӽ������
    void MulDiv();                      // �˳������
    void Relationship(std::string &ret);//��ϵ�����

    /* semantic analysis */

    int procedureDepth;					// procedureǶ�׼���
    SymbolTable symbolTable;			// ���ű�
    int dx;							    // ��procedure�͹��캯���� = 3; + 1 when var declared
    bool semanticError;					// �������

    Quaternary quaternary;
    TargetCode targetCode;
//    std::vector<std::pair<int, SymbolNode> > incmpltCode;
};


#endif //MYCOMPILER_SYNTAX_ANALYZER_H
