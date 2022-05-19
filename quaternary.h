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
    std::string NewTemp();      //产生新变量名T1,T2等
    void Print();

private:
    std::vector<Code> interCode;
    int tempId;
    std::vector<std::string> digits;
    int offset;
};


#endif //MYCOMPILER_QUATERNARY_H
