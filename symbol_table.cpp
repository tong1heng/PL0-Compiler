#include "symbol_table.h"

SymbolTable::SymbolTable() {
    mp = {
            {1, "CONSTANT"},
            {2, "VARIABLE"},
            {3, "PROCEDURE"}
    };
}

SymbolTable::~SymbolTable() {

}

int SymbolTable::CheckPos(std::string name, std::vector<int> kind, int lev) {   // 检查符号表低于lev的层次有无kind类的符号name
    int olev = lev;
    int size = table.size();
    for (int i = size - 1; i >= 0; i--) {
        bool chk = false;
        for (auto kk:kind) {
            if (mp[kk] == table[i].kind)
                chk = true;
        }

        int clev;
        if (table[i].kind == "CONSTANT") clev = table[i].para2;
        else clev = table[i].para1;

        if (name == table[i].name && chk) {
            if (olev >= clev) {
                return i;
            }
        }
        else {
            if (olev > clev) {
                olev = clev;
            }
        }
    }
    return -1;
}

bool SymbolTable::Insert(SymbolNode n, int lev) {  // 在符号表中插入一个符号
    std::vector<int> checkType;
    if (n.kind == "CONSTANT") {
        checkType = { 2, 3 };
    }
    if (n.kind == "VARIABLE") {
        checkType = { 1, 3 };
    }
    if (n.kind == "PROCEDURE") {
        checkType = { 1, 2 };
    }
    int pos = CheckPos(n.name, checkType, lev); // 检查低于lev的层次是否有同名不同类型的符号
    if (pos >= 0) return false;
    table.push_back(n);
    return true;
}

SymbolNode SymbolTable::GetNode(int i) {
    return table[i];
}

int SymbolTable::GetSize() {
    return table.size();
}

bool SymbolTable::SetPara2(int pos, int val) {
    if (pos >= table.size())
        return false;
    table[pos].para2 = val;
    return true;
}

void SymbolTable::PrintSymbolTable() {
    std::cout << "─────────────────────────────────────────────────────────────────\n";
    printf("Name\t\tKind\t\tPara1\t\tPara2\n");
    std::cout << "─────────────────────────────────────────────────────────────────\n";
//    printf("====================================================\n");
    for (SymbolNode node : table) {
        if(node.name.size() >= 8)
            printf("%s\t%s\t%d\t\t%d\n", node.name.c_str(), node.kind.c_str(), node.para1, node.para2);
        else
            printf("%s\t\t%s\t%d\t\t%d\n", node.name.c_str(), node.kind.c_str(), node.para1, node.para2);
    }
    std::cout << "─────────────────────────────────────────────────────────────────\n";
}
