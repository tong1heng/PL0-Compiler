#ifndef MYCOMPILER_WORD_H
#define MYCOMPILER_WORD_H

#include <string>

class Word {
public:
    std::string SYM;
    std::string NAME;
    Word() {
        SYM = "";
        NAME = "";
    }
};

#endif //MYCOMPILER_WORD_H
