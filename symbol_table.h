#ifndef MYCOMPILER_SYMBOL_TABLE_H
#define MYCOMPILER_SYMBOL_TABLE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

struct SymbolNode {
    std::string name, kind;
    int para1, para2;
    SymbolNode() {
        para1 = 0;
        para2 = 0;
    }
    SymbolNode(std::string &n, std::string &k, int p1, int p2)
        :name(n), kind(k), para1(p1), para2(p2) {}
    bool operator == (SymbolNode &b) const {
        if (this->name == b.name && this->kind == b.kind)
            return true;
        else
            return false;
    }
};

class SymbolTable {
public:
    SymbolTable(){
        mp = {
                {1, "CONSTANT"},
                {2, "VARIABLE"},
                {3, "PROCEDURE"}
        };
        valid = true;
    }
    bool insert(SymbolNode n, int lev);
    int checkPos(std::string name, std::vector<int> kind, int lev);
    SymbolNode getNode(int i) { return symbolTbl[i]; }
    int getSize() { return symbolTbl.size(); }
    bool setPara2(int pos, int val) {
        if (pos >= symbolTbl.size()) return false;
        symbolTbl[pos].para2 = val;
        return true;
    }

    // for test;
    void PrintSymbolTable();

private:
    std::map<int, std::string> mp;
    std::vector<SymbolNode> symbolTbl;	// ·ûºÅ±í
    bool valid;
};


#endif //MYCOMPILER_SYMBOL_TABLE_H
