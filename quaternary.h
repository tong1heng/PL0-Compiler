#ifndef MYCOMPILER_QUATERNARY_H
#define MYCOMPILER_QUATERNARY_H

#include <string>
#include <vector>

struct Code {
    std::string op;
    std::string arg1;
    std::string arg2;
    std::string result;
};

class Quaternary {
public:
    Quaternary();
    ~Quaternary();

    void Emit(Code &c);                 // 生成四元式
    std::string GenTemp();              // 产生编译过程中的临时变量名T1,T2等
    int NextCodeId();                   // 返回将要产生但尚未产生的四元式地址
    void BackPatch(int idx, int res);   // 回填
    void Print();                       // 打印

private:
    std::vector<Code> interCode;        // 中间代码
    int tempId;                         // 临时变量的序号
    std::vector<std::string> digits;    // 数字 0 ~ 9
};


#endif //MYCOMPILER_QUATERNARY_H
