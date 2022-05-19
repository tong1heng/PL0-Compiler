#include "quaternary.h"

Quaternary::Quaternary() {
    interCode.clear();
    tempId = 1;
    digits = {"0","1","2","3","4","5","6","7","8","9"};
    offset = 100;
}

Quaternary::~Quaternary() {

}

void Quaternary::emit(Code &c) {
    interCode.emplace_back(c);
}

std::string Quaternary::NewTemp() {
    std::string s;
    if(tempId < 10)
        s = "T" + digits[tempId];
    else
        s = "T" + digits[tempId / 10] + digits[tempId % 10];
    tempId++;
    return s;
}

void Quaternary::Print() {
//    printf("op\targ1\targ2\tresult\n");
    printf("ËÄÔªÊ½");
    printf("====================================================\n");
    for(int i = 0; i < interCode.size(); i++) {
        printf("(%d) (%s, %s, %s, %s)\n", i+offset, interCode[i].op.c_str(), interCode[i].arg1.c_str(),
                                                    interCode[i].arg2.c_str(), interCode[i].result.c_str());
    }
//    for (Code c : interCode) {
//        printf("%s\t%s\t%s\t%s\n", c.op.c_str(), c.arg1.c_str(), c.arg2.c_str(), c.result.c_str());
//    }
}