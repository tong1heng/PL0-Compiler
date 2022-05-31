#include "compiler.h"

Compiler::Compiler(std::string& in) {
    LA = new LexicalAnalyzer(in);
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
    SA->SetWord(LA->GetWord());
    SA->Run();
    SA->Show();
}

std::vector<TargetCodeNode> &Compiler::GetTargetCode() {
    return SA->GetTargetCode();
}
