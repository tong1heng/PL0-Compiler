#ifndef MYCOMPILER_NODE_H
#define MYCOMPILER_NODE_H

#include <string>
#include <vector>

/* Syntax Tree */
class Node {
    std::string val;
    std::vector<Node*> children;

    Node() {}
    Node(std::string v) :val(v) {}
    void InsertChild(Node* child) {
        children.push_back(child);
    }
};


#endif //MYCOMPILER_NODE_H
