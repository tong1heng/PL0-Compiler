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

    void Expression();          //���ʽ
    void Condition();           //����
    void Item();                //��
    void Factor();              //����
    void AddSub();              //�Ӽ������
    void MulDiv();              //�˳������
    void Relationship();        //��ϵ�����

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

    int Find(std::string str);
    std::string Find(int *addr);

    int codeIdx;
    void codeAdd(std::string str,int lev,int place,std::string con);
    void codeAdd(std::string str, int lev, int place);

};


#endif //MYCOMPILER_SYNTAX_ANALYZER_H
