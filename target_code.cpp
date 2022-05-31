#include "target_code.h"

TargetCode::TargetCode() {
    code.clear();
    mp = {
            {1, "LIT"},
            {2, "LOD"},
            {3, "STO"},
            {4, "CAL"},
            {5, "INT"},
            {6, "JMP"},
            {7, "JPC"},
            {8, "OPR"}
    };
}

TargetCode::~TargetCode() {

}

void TargetCode::Emit(int f, int l, int a) {    // 生成目标代码
    code.emplace_back(f, l, a);
}

void TargetCode::PrintTargetCode() {    // 打印
    printf("Target codes are as follows:\n");
    printf("odr\tf\tl\ta\n");
//    printf("===================================\n");
    int i = 0;
    for (TargetCodeNode lan : code) {
        printf("%d\t%s\t%d\t%d\n", i++, mp[lan.func].c_str(), lan.lyr, lan.amt);
    }
}

void TargetCode::BackPatch(int idx, int a) {    // 回填
    code[idx].amt = a;
}

int TargetCode::GetSize() {
    return code.size();
}

std::vector<TargetCodeNode> &TargetCode::GetTargetCode() {
    return code;
}
