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

    void emit(Code &c);
    std::string NewTemp();              //产生编译过程中的临时变量名T1,T2等
    int NextCodeId();                   //返回将要产生但尚未产生的四元式地址
    void BackPatch(int idx, int res);   //回填
    void Print();

private:
    std::vector<Code> interCode;
    int tempId;                         //临时变量的序号
    std::vector<std::string> digits;
    int offset;
};


#endif //MYCOMPILER_QUATERNARY_H
