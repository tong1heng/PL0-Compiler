#include "compiler.h"

Compiler::Compiler() {
    LA = new LexicalAnalyzer();
    SA = new SyntaxAnalyzer();
}

Compiler::~Compiler() {
    delete LA;
    delete SA;
}

void Compiler::LexicalAnalysis() {
    LA->Run();
    LA->Show();
}

void Compiler::SyntaxAnalysis() {
    SA->Run();
    SA->Show();
}
