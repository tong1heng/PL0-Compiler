#ifndef MYCOMPILER_TARGET_CODE_H
#define MYCOMPILER_TARGET_CODE_H

#include <vector>
#include <string>
#include <map>

struct TargetCodeNode {
    int func, lyr, amt;                         // 功能码, 层次差, 位移量
    TargetCodeNode() {
        func = 0, lyr = 0, amt = 0;
    }
    TargetCodeNode(int f, int l, int a) : func(f), lyr(l), amt(a) {}
};

class TargetCode {
public:
    TargetCode();
    ~TargetCode();

    void Emit(int f, int l, int a);             // 生成目标代码
    void PrintTargetCode();                     // 打印
    void BackPatch(int idx, int a);             // 回填
    int GetSize();
    std::vector<TargetCodeNode>& GetTargetCode();

private:
    std::vector<TargetCodeNode> code;           // 目标代码
    std::map<int, std::string> mp;              // 指令id到指令类型的映射
};


#endif //MYCOMPILER_TARGET_CODE_H
