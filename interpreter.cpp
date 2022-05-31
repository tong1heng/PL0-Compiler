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

// �������ŵ���ջ����a��Ϊ������
void Interpreter::LIT() {
    stack[regT] = regI.amt;
    regT++;
}

// �������ŵ�ջ����a��Ϊ��������˵�����е����λ�ã�lΪ���ò���˵����Ĳ��ֵ��
void Interpreter::LOD() {
    int var = stack[FindPos(regI.lyr) + regI.amt];
    stack[regT] = var;
    regT++;
}

// ��ջ���������͵�ĳ������Ԫ�С�a,l��ĺ�����LOD����ͬ��
void Interpreter::STO() {
    --regT;
    stack[FindPos(regI.lyr) + regI.amt] = stack[regT];
}

// ���ù��̵�ָ�aΪ�����ù��̵�Ŀ���������е�ַ��lΪ��
// SL����̬��������ָ����ù��̵�ֱ�����������ʱ���ݶεĻ���ַ��
// DL����̬��������ָ����øù���ǰ�������й��̵����ݶεĻ���ַ��
// RA�����ص�ַ����¼���øù���ʱĿ�����Ķϵ㣬����ʱ�ĳ����ַ�Ĵ���P��ֵ��
void Interpreter::CAL() {
    stack[regT] = FindPos(regI.lyr);
    stack[regT + 1] = regB;
    stack[regT + 2] = regP;
    regB = regT;
    regP = regI.amt;
}

// Ϊ�����õĹ��̣���������������ջ�п�����������a��Ϊ���ٵĸ�����
void Interpreter::INT() {
    int size = regI.amt;
    regT += size;
}

// ������ת��ָ�aΪת���ַ��
void Interpreter::JMP() {
    regP = regI.amt;
}

// ����ת��ָ���ջ���Ĳ���ֵΪ����ʱ��ת��a��ĵ�ַ������˳��ִ�С�
void Interpreter::JPC() {
    if (stack[--regT] == 0)
        regP = regI.amt;
}

// ��ϵ���������㡣���������a��������������Ϊջ���ʹζ������ݽ������㣬�������ڴζ���a��Ϊ0ʱ���˳���������
void Interpreter::OPR() {
    int a, b;
    switch (regI.amt) {
        case 0:  // �˳�
            regT = regB;				// ջ��λ��ָ�����ڹ��̵Ļ�ַ����Ϊ��ջ����
            regP = stack[regT + 2];			// ��һ��ָ��ָ��p ָ��ǰ���̵� ���ص�ַ
            regB = stack[regT + 1];			// ��ַ ָ��ǰ���̵� ��̬������ʶ��λ��
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
        case 11:  // -��Ŀ
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

// ��̬�����Ҳ��Ϊlyr�Ļ�ַ
int Interpreter::FindPos(int lyr) {
    int x = regB;
    while (lyr--)
        x = stack[x];
    return x;
}
