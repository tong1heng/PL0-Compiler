#include "interpreter.h"

Interpreter::Interpreter(std::vector<TargetCodeNode> &c) {
    code = c;
    regP = 0;
    regT = 0;
    regB = 0;
    stackSize = MAXSIZE;
    stack = new int[stackSize];
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

Interpreter::~Interpreter() {
    delete []stack;
}

void Interpreter::PrintStack() {
    printf("\n=========regI: %s\t%d\t%d\n", mp[regI.func].c_str(), regI.lyr, regI.amt);
    for (int i = 0; i < regT; i++) {
        printf("%d\t%d\n", i, stack[i]);
    }
    printf("==================\n\n");
}

void Interpreter::Run() {
    do {
        regI = code[regP++];
//        PrintStack();
        switch (regI.func) {
            case 1:
                LIT();
                break;
            case 2:
                LOD();
                break;
            case 3:
                STO();
                break;
            case 4:
                CAL();
                break;
            case 5:
                INT();
                break;
            case 6:
                JMP();
                break;
            case 7:
                JPC();
                break;
            case 8:
                OPR();
                break;
        }
    } while (regP != 0);
}

// 将常数放到运栈顶。a域为常数。
void Interpreter::LIT() {
    stack[regT] = regI.amt;
    regT++;
}

// 将变量放到栈顶。a域为变量在所说明层中的相对位置，l为调用层与说明层的层差值。
void Interpreter::LOD() {
    int var = stack[FindPos(regI.lyr) + regI.amt];
    stack[regT] = var;
    regT++;
}

// 将栈顶的内容送到某变量单元中。a,l域的含义与LOD的相同。
void Interpreter::STO() {
    --regT;
    stack[FindPos(regI.lyr) + regI.amt] = stack[regT];
}

// 调用过程的指令。a为被调用过程的目标程序的入中地址，l为层差。
// SL：静态链，它是指向定义该过程的直接外过程运行时数据段的基地址。
// DL：动态链，它是指向调用该过程前正在运行过程的数据段的基地址。
// RA：返回地址，记录调用该过程时目标程序的断点，即当时的程序地址寄存器P的值。
void Interpreter::CAL() {
    stack[regT] = FindPos(regI.lyr);
    stack[regT + 1] = regB;
    stack[regT + 2] = regP;
    regB = regT;
    regP = regI.amt;
}

// 为被调用的过程（或主程序）在运行栈中开辟数据区。a域为开辟的个数。
void Interpreter::INT() {
    int size = regI.amt;
    regT += size;
}

// 无条件转移指令，a为转向地址。
void Interpreter::JMP() {
    regP = regI.amt;
}

// 条件转移指令，当栈顶的布尔值为非真时，转向a域的地址，否则顺序执行。
void Interpreter::JPC() {
    if (stack[--regT] == 0)
        regP = regI.amt;
}

// 关系和算术运算。具体操作由a域给出。运算对象为栈顶和次顶的内容进行运算，结果存放在次顶。a域为0时是退出数据区。
void Interpreter::OPR() {
    int a, b;
    switch (regI.amt) {
        case 0:  // 退出
            regT = regB;				// 栈顶位置指向现在过程的基址，作为退栈操作
            regP = stack[regT + 2];			// 下一条指令指针p 指向当前过程的 返回地址
            regB = stack[regT + 1];			// 基址 指向当前过程的 动态链所标识的位置
            break;
        case 1:  // +
            regT--;
            a = stack[regT - 1], b = stack[regT];
            stack[regT - 1] = a + b;
            break;
        case 2:  // -
            regT--;
            a = stack[regT - 1], b = stack[regT];
            stack[regT - 1] = a - b;
            break;
        case 3:  // *
            regT--;
            a = stack[regT - 1], b = stack[regT];
            stack[regT - 1] = a * b;
            break;
        case 4:  // /
            regT--;
            a = stack[regT - 1], b = stack[regT];
            stack[regT - 1] = a / b;
            break;
        case 5:  // ==
            regT--;
            a = stack[regT - 1], b = stack[regT];
            stack[regT - 1] = (a == b);
            break;
        case 6:  // !=
            regT--;
            a = stack[regT - 1], b = stack[regT];
            stack[regT - 1] = (a != b);
            break;
        case 7:  // <
            regT--;
            a = stack[regT - 1], b = stack[regT];
            stack[regT - 1] = (a < b);
            break;
        case 8:  // <=
            regT--;
            a = stack[regT - 1], b = stack[regT];
            stack[regT - 1] = (a <= b);
            break;
        case 9:  // >
            regT--;
            a = stack[regT - 1], b = stack[regT];
            stack[regT - 1] = (a > b);
            break;
        case 10:  // >=
            regT--;
            a = stack[regT - 1], b = stack[regT];
            stack[regT - 1] = (a >= b);
            break;
        case 11:  // -单目
            stack[regT - 1] = -stack[regT - 1];
            break;
        case 12:  // odd
            stack[regT - 1] = (stack[regT - 1] % 2);
            break;
        case 13:  // read  cin
            std::cin >> stack[regT];
            regT++;
            break;
        case 14:  // write cout
            std::cout << stack[--regT] << "\n";
            break;
    }
}

// 静态链查找层差为lyr的基址
int Interpreter::FindPos(int lyr) {
    int x = regB;
    while (lyr--)
        x = stack[x];
    return x;
}
