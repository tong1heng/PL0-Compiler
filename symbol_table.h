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
        : name(n), kind(k), para1(p1), para2(p2) {}
    bool operator == (SymbolNode &b) const {
        if (this->name == b.name && this->kind == b.kind)
            return true;
        else
            return false;
    }
};

class SymbolTable {
public:
    SymbolTable();
    ~SymbolTable();

    bool Insert(SymbolNode n, int lev);                             // �ڷ��ű��в���һ������
    int CheckPos(std::string name, std::vector<int> kind, int lev); // �����ű����lev�Ĳ������kind��ķ���name
    SymbolNode GetNode(int i);
    int GetSize();
    bool SetPara2(int pos, int val);
    void PrintSymbolTable();

private:
    std::map<int, std::string> mp;
    std::vector<SymbolNode> table;	// ���ű�
};


#endif //MYCOMPILER_SYMBOL_TABLE_H
