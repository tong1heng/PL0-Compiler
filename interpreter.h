#ifndef MYCOMPILER_INTERPRETER_H
#define MYCOMPILER_INTERPRETER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "target_code.h"

#define MAXSIZE 1e8;

class Interpreter {
public:
    Interpreter(std::vector<TargetCodeNode> &c);
    ~Interpreter();

    void Run();

private:
    std::vector<TargetCodeNode> code;		// Ŀ�����
    int *stack;                             // ջ��
    int stackSize;				            // ջ��С
    std::map<int, std::string> mp;

    // registers
    TargetCodeNode regI;	    // ָ��Ĵ���
    int regP;				    // �����ַ�Ĵ�����ָ����һ��Ҫִ�е�Ŀ��ָ��൱��CODE������±꣩
    int regT;					// ջ���Ĵ���T��ָ���˵�ǰջ�����·���ĵ�Ԫ��TҲ������S���±꣩
    int regB;					// ����ַ�Ĵ�����ָ��ÿ�����̱�����ʱ����������S�и�������������ݶ���ʼ��ַ��Ҳ��Ϊ����ַ

    void LIT();
    void LOD();
    void STO();
    void CAL();
    void INT();
    void JMP();
    void JPC();
    void OPR();
    int FindPos(int lyr);		// ��̬�� ���Ҳ��Ϊlyr�Ļ�ַ
    void PrintStack();          // ��ӡջ������
};


#endif //MYCOMPILER_INTERPRETER_H
