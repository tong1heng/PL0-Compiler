#ifndef MYCOMPILER_SYNTAX_ANALYZER_H
#define MYCOMPILER_SYNTAX_ANALYZER_H

#include "word.h"
#include "node.h"
#include "symbol_table.h"
#include "quaternary.h"
#include <string>

class SyntaxAnalyzer {
public:
    SyntaxAnalyzer();
    ~SyntaxAnalyzer();

    void Run();
    void Show();

private:

    /* syntax analysis */

    int ip;

    void Program();             //����
    void SubProgram();          //�ӳ���

    void ConstDeclare();        //����˵������
    void VarDeclare();          //����˵������
    void ProcedureDeclare();    //����˵������
    void Statement();           //���

    void ConstDefine();         //��������
    void Identifier(std::string &id);          //��ʶ��
    void UnsignedInt(int &num);         //�޷�������
    void ProcedureHead();       //�����ײ�

    void AssignmentStatement(); //��ֵ���
    void ConditionStatement();  //�������
    void DoWhileStatement();    //����ѭ�����
    void CallStatement();       //���̵������
    void ReadStatement();       //�����
    void WriteStatement();      //д���
    void CompoundStatement();   //�������

    void Expression(std::string &ret);          //���ʽ
    void Condition(Code &code);           //����
    void Item(std::string &ret);                //��
    void Factor(std::string &ret);              //����
    void AddSub();              //�Ӽ������
    void MulDiv();              //�˳������
    void Relationship(std::string &ret);        //��ϵ�����

    /* semantic analysis */

//    int procedureDepth;         //Ƕ�׵Ĺ������
//    int tableIdx;               //���ű�����
//    int dx;                     //ÿ��ľֲ���������Ե�ַ

    Node *ASTroot;					// �﷨������
    std::string _resStr;			// �﷨��������ַ���
    bool synErr;					// �﷨��������

    int cntPro;						// procedureǶ�׼���
    SymbolTable symbolTbl;			// ���ű�
    int dx;							// ��procedure�͹��캯���� = 3;  and +1 when var declared
    bool smnErr;					// �������

    Quaternary quaternary;

};


#endif //MYCOMPILER_SYNTAX_ANALYZER_H
