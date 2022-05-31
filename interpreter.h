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
    std::vector<TargetCodeNode> code;		// 目标代码
    int *stack;                             // 栈区
    int stackSize;				            // 栈大小
    std::map<int, std::string> mp;

    // registers
    TargetCodeNode regI;	    // 指令寄存器
    int regP;				    // 程序地址寄存器，指向下一条要执行的目标指令（相当于CODE数组的下标）
    int regT;					// 栈顶寄存器T，指出了当前栈中最新分配的单元（T也是数组S的下标）
    int regB;					// 基地址寄存器，指出每个过程被调用时，在数据区S中给出它分配的数据段起始地址，也称为基地址

    void LIT();
    void LOD();
    void STO();
    void CAL();
    void INT();
    void JMP();
    void JPC();
    void OPR();
    int FindPos(int lyr);		// 静态链 查找层差为lyr的基址
    void PrintStack();          // 打印栈的内容
};


#endif //MYCOMPILER_INTERPRETER_H
