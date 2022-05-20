#include "symbol_table.h"

int SymbolTable::checkPos(std::string name, std::vector<int> kind, int lev) {
    int levSr = lev;
    int size = symbolTbl.size();
    for (int i = size - 1; i >= 0; i--) {
        bool chk = false;
        for (auto kk:kind) {
            if (mp[kk] == symbolTbl[i].kind)
                chk = true;
        }

        int laim;
        if (symbolTbl[i].kind == "CONSTANT") laim = symbolTbl[i].para2;
        else laim = symbolTbl[i].para1;

        if (name == symbolTbl[i].name && chk) {

            if (levSr >= laim) {
                return i;
            }
        }
        else {
            if (levSr > laim) {
                levSr = laim;
            }
        }
    }
    return -1;
}

bool SymbolTable::insert(SymbolNode n, int lev) {  // check高层次是否 同名 不同类型
    std::vector<int> checktype;
    if (n.kind == "CONSTANT") {
        checktype = { 2,3 };
    }
    if (n.kind == "VARIABLE") {
        checktype = { 1,3 };
    }
    if (n.kind == "PROCEDURE") {
        checktype = { 1,2 };
    }
    int pos = checkPos(n.name, checktype, lev);
    if (pos >= 0) return false;
    symbolTbl.push_back(n);
    return true;
}

void SymbolTable::PrintSymbolTable() {
    std::cout << "─────────────────────────────────────────────────────────────────\n";
    printf("Name\t\tKind\t\tPara1\t\tPara2\n");
    std::cout << "─────────────────────────────────────────────────────────────────\n";
//    printf("====================================================\n");
    for (SymbolNode node : symbolTbl) {
        printf("%s\t\t%s\t\t%d\t\t%d\n", node.name.c_str(), node.kind.c_str(), node.para1, node.para2);
    }
    std::cout << "─────────────────────────────────────────────────────────────────\n";
}
