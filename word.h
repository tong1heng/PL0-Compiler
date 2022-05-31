#ifndef MYCOMPILER_WORD_H
#define MYCOMPILER_WORD_H

#include <string>
#include <utility>

struct Word {
    std::string SYM;
    std::string NAME;
    Word() {
        SYM = "";
        NAME = "";
    }
    Word(std::string sym, std::string name) {
        SYM = std::move(sym);
        NAME = std::move(name);
    }
};

#endif //MYCOMPILER_WORD_H
